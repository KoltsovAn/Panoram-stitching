#include "recovery_form.h"
#include "ui_recovery_form.h"

Recovery_form::Recovery_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Recovery_form)
{
    ui->setupUi(this);
}

Recovery_form::~Recovery_form()
{
    delete ui;
}

void Recovery_form::on_FileButton_clicked()
{
    ui->TxtFilePath->setText(QFileDialog::getOpenFileName());
}

void Recovery_form::on_CatalogButton_clicked()
{
    ui->CatalogPath->setText(QFileDialog::getExistingDirectory());
}

void Recovery_form::on_CanctlButton_clicked()
{
    this->close();
    emit finish();
}

void Recovery_form::on_OkButton_clicked()
{
    if(ui->TxtFilePath->text().isNull())
    {
        QMessageBox MB;
        MB.setText("Необходимо указать путь к файлу!");
        MB.exec();
        return;
    }
    if(ui->CatalogPath->text().isNull())
    {
        QMessageBox MB;
        MB.setText("Необходимо указать путь к каталогу!");
        MB.exec();
        return;
    }
    pprd = new QProgressDialog("Идет обработка изображений...", 0, 0, 2*ui->HorizontalValue->value()*ui->VertycalValue->value());
    pprd->setMinimumDuration(0);
    pprd->setCancelButton(0);
    pprd->setWindowTitle("Please Wait");
    pprd->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    int id=0;
    pprd->setValue(id);
    qApp->processEvents();
    std::vector<QString> layer;
    QFile *txtFile = new QFile(ui->TxtFilePath->text());
    if(!(txtFile->open(QFile::ReadOnly | QFile::Text)))
    {
        QMessageBox MB;
        MB.setText("Неверно указан файл!");
        MB.exec();
        pprd->setValue(pprd->maximum());
        delete pprd;
        return;
    }
    QTextStream stream(txtFile);
    for(int i = 0; i < ui->VertycalValue->value(); i++)
    {
        std::vector<fileDescription> line;
        for(int k = 0; k < ui->HorizontalValue->value(); k++)
        {
            QStringList str = stream.readLine().split("\t ");
            QString SX = str.at(1);
            SX = SX.split("=").at(1);
            QString SY = str.at(2);
            SY = SY.split("=").at(1);
            line.push_back(fileDescription(str.at(0), SX.toInt(), SY.toInt()));
            pprd->setValue(++id);
            qApp->processEvents();
        }
        AllDescription.push_back(line);
    }
    if(ui->panorama_2D->isChecked())
        layer.push_back(ui->CatalogPath->text());
    else
    {
        QDir d(ui->CatalogPath->text());
        d.setFilter(QDir::Dirs);
        QStringList Layers=d.entryList();
        if(Layers.size() == 0)
        {
            QMessageBox MB;
            MB.setText("В папке отсутствуют каталоги с панорамой!");
            MB.exec();
            pprd->setValue(pprd->maximum());
            AllDescription.clear();
            delete pprd;
            return;
        }
        Layers.pop_front();
        Layers.pop_front();
        for(QString &str: Layers)
            layer.push_back(ui->CatalogPath->text()+"/"+str);

    }

    int Hsize = AllDescription.size();
    int Vsize = AllDescription.at(0).size();
    QImage firstIMG(layer.at(0)+AllDescription.at(0).at(0).getFileName());
    int W = firstIMG.width();
    int H = firstIMG.height();
    QImage map((AllDescription.at(Hsize-1).at(Vsize-1).getX()+W)/16,
               (AllDescription.at(Hsize-1).at(Vsize-1).getY()+H)/16, QImage::Format_ARGB32);
    for (int h=0;h<Vsize;h++)
    {
        for(int w=0;w<Hsize;w++)
        {
            firstIMG.load(layer.at(0)+AllDescription.at(w).at(h).getFileName());
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
    emit recovery_finish(AllDescription, layer, map);
}
