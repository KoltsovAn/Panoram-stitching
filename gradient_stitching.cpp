#include "gradient_stitching.h"

Gradient_stitching::Gradient_stitching(QString path, int MaxV, int MinV, int MaxH, int MinH):
    Stitching_Frames(path, MaxV, MinV, MaxH, MinH)
{

}

void Gradient_stitching::start()
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

    QImage *firstIMG, *secondIMG;
    firstIMG = new QImage;
    secondIMG = new QImage;
    firstIMG->load(DirPath+"/y=0 x=0.bmp");
    int W = firstIMG->width();
    int H = firstIMG->height();
    int loadX = 0, loadY = 0;
    std::vector< std::vector< int> > X;
    while (firstIMG->load(DirPath+"/y="+QString::number(loadY)+" x=0.bmp"))
    {
        std::vector<int> line(1, 0);
        loadX = W;
        while (secondIMG->load(DirPath+"/y="+QString::number(loadY)+" x="+QString::number(loadX)+".bmp"))
        {
            gradient_method_stitching GMS(firstIMG, secondIMG,
                                          MinHorizontalCrossing, MaxHorizontalCrossing,
                                          0, 2048);
            GMS.start();
            std::vector<Point> Object = GMS.GetObject();
            int deltaX = 0, N = 0;
            QPoint CMK(0, 0), CMSPK(0, 0);
            for (size_t i = 0; i < Object.size(); i++)
            {
                QPoint CM = Object.at(i).GetCenterMass();
                QPoint CMSP = Object.at(i).GetSameCenter();
                if(std::abs(CM.y()-CMSP.y()) < 50)
                {
                    deltaX +=firstIMG->width()-CM.x()-CMSP.x();
                    N++;
                    CMK = Object.at(i).GetCenterMass();
                    CMSPK = Object.at(i).GetSameCenter();
                }
            }
            if(CMK != QPoint(0, 0))
                deltaX = CMK.x()-CMSPK.x()/*+deltaX/N*/;
            else
                deltaX = firstIMG->width();
            line.push_back(line.at(line.size()-1)+deltaX);
            loadX += W;
            delete firstIMG;
            firstIMG = secondIMG;
            secondIMG = new QImage;
            pprd->setValue(++id);
            qApp->processEvents();
        }
        X.push_back(line);
        loadY +=H;
    }
    loadX = 0;
    loadY = H;
    std::vector<int> Y(1, 0);
    while(secondIMG->load(DirPath+"/y="+QString::number(loadY)+" x=0.bmp"))
    {
        loadX = 0;
        QPoint CMK(0, 0);
        QPoint CMSPK(0, 0);
        int deltaY = 0, N = 0;
        while(CMK == QPoint(0, 0) &&
              secondIMG->load(DirPath+"/y="+QString::number(loadY)+" x="+QString::number(loadX)+".bmp"))
        {
            firstIMG->load(DirPath+"/y="+QString::number(loadY-H)+" x="+QString::number(loadX)+".bmp");
            gradient_method_stitching GMS(firstIMG, secondIMG,
                                          0, 2048,
                                          MinVertycalCrossing, MaxVertycalCrossing);
            GMS.start();
            std::vector<Point> Object = GMS.GetObject();
            if(Object.size() != 0)
                for (size_t i = 0; i < Object.size(); i++)
                {
                    QPoint CM = Object.at(i).GetCenterMass();
                    QPoint CMSP = Object.at(i).GetSameCenter();
                    if(std::abs(CM.x()-CMSP.x()) < 50)
                    {
                        deltaY +=firstIMG->height()-CM.y()-CMSP.y();
                        N++;
                        CMK = Object.at(i).GetCenterMass();
                        CMSPK = Object.at(i).GetSameCenter();
                    }
                }
            loadX += W;
        }
        if(CMK != QPoint(0, 0))
            deltaY = CMK.y()-CMSPK.y()/*+deltaY/N*/;
        else
            deltaY = firstIMG->height();
        Y.push_back(Y.at(Y.size()-1)+deltaY);
        pprd->setValue(++id);
        qApp->processEvents();
        loadY += W;
        loadX = 0;
    }
    qDebug()<<"сшивка закончена";
    for(size_t i = 0; i < X.size(); i++)
    {
        std::vector<fileDescription> line;
        for(size_t k = 0; k < X.at(i).size(); k++)
        {
            line.push_back(fileDescription("/y="+QString::number(H*i)+
                                           " x="+QString::number(W*k)+".bmp", X.at(i).at(k), Y.at(i) ));
            pprd->setValue(++id);
            qApp->processEvents();
        }
        AllDescription.push_back(line);
    }
    qDebug()<< "AllDescription создан";
    int Hsize = AllDescription.size();
    int Vsize = AllDescription.at(0).size();
    QImage map((AllDescription.at(Hsize-1).at(Vsize-1).getX()+W)/16,
               (AllDescription.at(Hsize-1).at(Vsize-1).getY()+H)/16, QImage::Format_ARGB32);
    for (int h=0;h<Vsize;h++)
    {
        for(int w=0;w<Hsize;w++)
        {
            firstIMG->load(DirPath+AllDescription.at(w).at(h).getFileName());
            int yStart=AllDescription.at(h).at(w).getY()/16;
            int xStart=AllDescription.at(h).at(w).getX()/16;
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
                                QColor firstRGB=firstIMG->pixel(x,y);
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
    qDebug()<< "карта создана";
    std::vector<QString> layer(1, DirPath);

    emit finish(AllDescription, layer, map);
}
