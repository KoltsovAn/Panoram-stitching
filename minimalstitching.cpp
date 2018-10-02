#include "minimalstitching.h"

MinimalStitching::MinimalStitching(QString path, int MaxV, int MinV, int MaxH, int MinH) :
    Stitching_Frames(path, MaxV, MinV, MaxH, MinH)
{

}

int MinimalStitching::cshivkaHorizontal(QImage firstImg, QImage secondImg, int xMin, int xMax)
{
    int minX=0;
    double k,min=std::numeric_limits<double>::max();
    QRgb firstRGB, secondRGB;

    for (int n=xMax; n!=xMin; --n)
    {
        k=0.0;
        for(int y=0; y<firstImg.height(); ++y)
            for(int x=0; x<n; ++x)
            {
                firstRGB=firstImg.pixel(firstImg.width()-n+x,y);
                secondRGB=secondImg.pixel(x,y);

                k+=(abs(qRed(firstRGB)-qRed(secondRGB))+abs(qBlue(firstRGB)-qBlue(secondRGB))+abs(qGreen(firstRGB)-qGreen(secondRGB)))*1.0/(3*n);
            }

        if (k<min)
        {
            min=k;
            minX=n;
        }
    }

    return minX;
}

int MinimalStitching::cshivkaVertical(QImage firstImg, QImage secondImg, int yMin, int yMax)
{
    int minY=0;
    double k,min=std::numeric_limits<double>::max();
    QRgb firstRGB, secondRGB;

    for (int n=yMax; n!=yMin; --n)
    {
        k=0.0;
        for(int x=0; x<firstImg.width(); ++x)
            for(int y=0; y<n; ++y)
            {
                firstRGB=firstImg.pixel(x,firstImg.height()-n+y);
                secondRGB=secondImg.pixel(x,y);

                k+=(abs(qRed(firstRGB)-qRed(secondRGB))+abs(qBlue(firstRGB)-qBlue(secondRGB))+abs(qGreen(firstRGB)-qGreen(secondRGB)))*1.0/(3*n);
            }

        if (k<min)
        {
            min=k;
            minY=n;
        }
    }

    return minY;
}

void MinimalStitching::start()
{
    QDir d(DirPath);
    d.setFilter(QDir::Files);
    d.setNameFilters(QStringList()<<"*.bmp");
    QStringList SizePanoram=d.entryList();
    if (SizePanoram.size()==0)
    {
        QMessageBox MB;
        MB.setText("В папке отсутствуют файлы формата '*.bmp'!");
        MB.exec();
        emit ErrorStitching();
        return;
    }

    pprd = new QProgressDialog("Идет обработка изображений...", 0, 0, 3*SizePanoram.size());
    pprd->setMinimumDuration(0);
    pprd->setCancelButton(0);
    pprd->setWindowTitle("Please Wait");
    pprd->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    int id=0;
    pprd->setValue(id);
    qApp->processEvents();

    QString firstName, secondName;
    qreal yPos=0, xPos=0, lineXPos=0, lineYPos=0;
    QImage firstIMG, secondIMG;
    QList<int> pointX, pointY;

    firstName=DirPath+"/y=0 x=0.bmp";
    firstIMG.load(firstName);
    int W=firstIMG.width();
    int H=firstIMG.height();

    while (firstIMG.load(firstName))
    {
        pointX.push_back(lineXPos);
        xPos+=W;
        lineXPos+=W;
        secondName=DirPath+"/y="+QString::number(yPos)+" x="+QString::number(xPos)+".bmp";

        while (secondIMG.load(secondName))
        {
            int xx=cshivkaHorizontal(firstIMG,secondIMG,MinHorizontalCrossing,MaxHorizontalCrossing);

            lineXPos-=xx;
            pointX.push_back(lineXPos);
            firstIMG=secondIMG;
            lineXPos+=W;
            xPos+=W;
            secondName=DirPath+"/y="+QString::number(yPos) +" x="+QString::number(xPos)+".bmp";
            pprd->setValue(++id);
            qApp->processEvents();
        }
        lineXPos=0;
        xPos=0;
        yPos+=H;
        firstName=DirPath+"/y="+QString::number(yPos)+" x=0.bmp";
    }

    firstName=DirPath+"/y=0 x=0.bmp";
    firstIMG.load(firstName);
    yPos=0;
    yPos+=H;
    lineYPos+=H;
    secondName=DirPath+"/y="+QString::number(yPos) +" x=0.bmp";
    pointY.push_back(0);
    while (secondIMG.load(secondName))
    {
        int xx=cshivkaVertical(firstIMG,secondIMG,MinVertycalCrossing,MaxVertycalCrossing);

        lineYPos-=xx;
        pointY.push_back(lineYPos);
        firstIMG=secondIMG;
        lineYPos+=H;
        yPos+=H;
        secondName=DirPath+"/y="+QString::number(yPos) +" x=0.bmp";
        pprd->setValue(++id);
        qApp->processEvents();
     }


    firstName=DirPath+"/y=0 x=0.bmp";
    xPos=yPos=0;
    QList<int>::Iterator itX=pointX.begin();
    QList<int>::Iterator itY=pointY.begin();
    while (firstIMG.load(firstName))
    {
        int yy=*itY;
        int xx=*itX;
        std::vector<fileDescription> line;
        line.push_back(fileDescription("/y="+QString::number(yPos)+" x="+QString::number(xPos)+".bmp",xx,yy));
        xPos+=W;
        itX++;
        firstName=DirPath+"/y="+QString::number(yPos)+" x="+QString::number(xPos)+".bmp";
        while (firstIMG.load(firstName))
        {
            xx=*itX;
            line.push_back(fileDescription("/y="+QString::number(yPos)+" x="+QString::number(xPos)+".bmp",xx,yy));
            xPos+=W;
            itX++;
            firstName=DirPath+"/y="+QString::number(yPos) +" x="+QString::number(xPos)+".bmp";
            pprd->setValue(++id);
            qApp->processEvents();
        }
        AllDescription.push_back(line);
        xPos=0;
        yPos+=H;
        itY++;
        firstName=DirPath+"/y="+QString::number(yPos)+" x=0.bmp";
    }

    int Hsize = AllDescription.size();
    int Vsize = AllDescription.at(0).size();
    QImage map((AllDescription.at(Hsize-1).at(Vsize-1).getX()+W)/16,
               (AllDescription.at(Hsize-1).at(Vsize-1).getY()+H)/16, QImage::Format_ARGB32);
    for (int h=0;h<Vsize;h++)
    {
        for(int w=0;w<Hsize;w++)
        {
            firstIMG.load(DirPath+AllDescription.at(w).at(h).getFileName());
            int yStart=AllDescription.at(w).at(h).getY()/16;
            int xStart=AllDescription.at(w).at(h).getX()/16;
            for(int y0=0,savePixelY=yStart;y0<H;y0+=16)
            {
                for(int x0=0,savePixelX=xStart;x0<W;x0+=16)
                {
                    int r=0,g=0,b=0;
                    for(int y=y0;y<y0+16;y++)
                        for(int x=x0;x<x0+16;x++)
                        {
                            if(x<W && y<H && x>0 && y>0)
                            {
                            QColor firstRGB=firstIMG.pixel(x,y);
                            r=(r+firstRGB.red())/2;
                            g=(g+firstRGB.green())/2;
                            b=(b+firstRGB.blue())/2;
                            }
                        }
                    map.setPixel(savePixelX,savePixelY,qRgba(r,g,b,31));
                    savePixelX++;
                }
                savePixelY++;
            }
            pprd->setValue(++id);
            qApp->processEvents();
        }
    }

    std::vector<QString> layer(1, DirPath);
    emit finish(AllDescription, layer, map);
}
