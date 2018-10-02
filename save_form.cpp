#include "save_form.h"
#include "ui_save_form.h"

Save_Form::Save_Form(QGraphicsScene *Scene, size_t Width, size_t Height, QWidget *parent):
    scene(Scene), W(Width), H(Height), QWidget(parent), ui(new Ui::Save_Form)
{
    ui->setupUi(this);
    ui->FormatFrame->addItem(".bmp");
    ui->FormatFrame->addItem(".jpg");
    ui->SizeFrame->addItem(QString::number(Width)+"x"+QString::number(Height));
    ui->SizeFrame->addItem(QString::number(Width/2)+"x"+QString::number(Height/2));
    ui->SizeFrame->addItem(QString::number(Width/4)+"x"+QString::number(Height/4));
    ui->SizeFrame->addItem(QString::number(Width/8)+"x"+QString::number(Height/8));
}

Save_Form::~Save_Form()
{
    delete ui;
}

void Save_Form::on_CancelButton_clicked()
{
    this->close();
    emit finish();
}

void Save_Form::on_OkButton_clicked()
{
    if(ui->CatalogPath->text().isNull())
    {
        QMessageBox MB;
        MB.setText("Необходимо указать путь!");
        MB.exec();
        return;
    }

    this->close();

    QProgressDialog* pprd = new QProgressDialog("Идет сохранение...", 0, 0, 100);
    pprd->setMinimumDuration(0);
    pprd->setCancelButton(0);
    pprd->setWindowTitle("Please Wait");
    pprd->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    //int N=1.5+(scene->width()*scene->height())/(W*H);
    int N=(scene->width()/W)+0.5;
    N*=((scene->height()/H)+0.5);
//    qDebug()<<N;
    int Maxpprd=(N+0.1*N);
    int level=1;
    while(N>1)
    {
        N/=4;
        level++;
        Maxpprd+=N;
    }
    pprd->setMaximum(Maxpprd);
    Maxpprd=0;

    int numb=0;
    numb=ui->SizeFrame->currentIndex();
    level+=numb;
    numb++;
    QString saveDir=ui->CatalogPath->text()+"/"+QString::number(level);
    QDir().mkdir(saveDir);

    QImage saveImg(W/numb,H/numb, QImage::Format_RGB32);
    QPainter painter(&saveImg);
    QFile *position = new QFile(ui->CatalogPath->text()+"/PositionOriginalFrame.txt");
    position->open(QFile::WriteOnly | QFile::Text);
    QTextStream stream(position);
    emit OriginalImagePositionSave(stream);

    for(int y = 0; y <= scene->height(); y+=H/numb)
    {
        for(int x = 0;x <= scene->width(); x+=W/numb)
        {
            emit UpdateLoadZone(QPointF(x, y), QPointF(x+2*numb, y+2*numb));
            saveImg.fill(QColor(Qt::white).rgb());
            scene->render(&painter,QRectF(0,0,W/numb,H/numb),QRectF(x,y,W/numb,H/numb));
            saveImg.save(saveDir+"/y="+QString::number(y)+
                         " x="+QString::number(x)+ui->FormatFrame->currentText());
            pprd->setValue(++Maxpprd);
            qApp->processEvents();
        }
    }

    QString openDir;

    int SH=scene->height();
    int WH=scene->width();

    while(level!=1)
    {
        level--;
        openDir=saveDir;
        saveDir=ui->CatalogPath->text()+"/"+QString::number(level);
        QDir().mkdir(saveDir);
        for(int savePosY=0, openPosY=0; openPosY<SH;openPosY+=H/numb*2)
        {
            for(int savePosX=0, openPosX=0; openPosX<WH;openPosX+=W/numb*2)
            {
                    derevo((openDir+"/y="+ QString::number(openPosY)+" x="+QString::number(openPosX)+
                            ui->FormatFrame->currentText()),&saveImg,H/numb,W/numb,0,0);

                    derevo((openDir+"/y="+ QString::number(openPosY)+" x="+QString::number(openPosX+W/numb)+
                            ui->FormatFrame->currentText()),&saveImg,H/numb,W/numb,W/numb/2,0);

                    derevo((openDir+"/y="+ QString::number(openPosY+H/numb)+" x="+QString::number(openPosX)+
                            ui->FormatFrame->currentText()),&saveImg,H/numb,W/numb,0,H/numb/2);

                    derevo((openDir+"/y="+ QString::number(openPosY+H/numb)+" x="+
                            QString::number(openPosX+W/numb)+
                            ui->FormatFrame->currentText()),&saveImg,H/numb,W/numb,W/numb/2,H/numb/2);

                saveImg.save(saveDir+"/y="+QString::number(savePosY)+" x="+QString::number(savePosX)+
                             ui->FormatFrame->currentText());
                savePosX+=W/numb;
                pprd->setValue(++Maxpprd);
                qApp->processEvents();
            }
            savePosY+=H/numb;
        }
        SH/=2; WH/=2;
    }

    position->close();
    delete pprd;
    emit finish();
}

void Save_Form::on_SelectCatalog_clicked()
{
    ui->CatalogPath->setText(QFileDialog::getExistingDirectory(this, tr("Выберите директорию."),
                                                               "/home"));
}

void Save_Form::derevo(QString fileName, QImage *saveImg, int sizeH, int sizeV, int poX, int poY)
{

    QRgb rgb1,rgb2,rgb3,rgb4;
    int r,g,b;
    QImage orgImg;
    if(orgImg.load(fileName)==true)
        for(int orgY=0, posY=poY; orgY < sizeH; orgY+=2)
        {
            for(int orgX=0, posX=poX; orgX<sizeV; orgX+=2)
            {
                rgb1=orgImg.pixel(orgX,orgY);
                rgb2=orgImg.pixel(orgX+1,orgY);
                rgb3=orgImg.pixel(orgX,orgY+1);
                rgb4=orgImg.pixel(orgX+1,orgY+1);
                r=(qRed(rgb1)+qRed(rgb2)+qRed(rgb3)+qRed(rgb4))/4;
                g=(qGreen(rgb1)+qGreen(rgb2)+qGreen(rgb3)+qGreen(rgb4))/4;
                b=(qBlue(rgb1)+qBlue(rgb2)+qBlue(rgb3)+qBlue(rgb4))/4;
                saveImg->setPixel(posX,posY,qRgb(r,g,b));
                posX++;
            }
            posY++;
        }
    else
        for(int posY=0; posY<sizeH/2; posY++)
            for(int posX=0; posX<sizeV/2; posX++)
                saveImg->setPixel(posX,posY,Qt::black);

}
