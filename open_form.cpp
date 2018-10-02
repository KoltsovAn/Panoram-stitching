#include "open_form.h"
#include "ui_open_form.h"

Open_Form::Open_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Open_Form)
{
    ui->setupUi(this);
}

Open_Form::~Open_Form()
{
    delete stitching;
    stitching = 0;
    delete ui;
}

void Open_Form::on_SelectCatalog_clicked()
{
    ui->CatalogPath->setText(QFileDialog::getExistingDirectory(this, tr("Выберите директорию."),
                                                               "/home", QFileDialog::DontResolveSymlinks));
}

void Open_Form::on_CancelButton_clicked()
{
    emit finish();
}

void Open_Form::on_OkButton_clicked()
{
    if(ui->CatalogPath->text().isNull())
    {
        QMessageBox MB;
        MB.setText("Необходимо указать путь!");
        MB.exec();
        return;
    }
    if(ui->Panorama2D->isChecked())
    {
        if(ui->MinimalStitching->isChecked())
            stitching = new MinimalStitching(ui->CatalogPath->text(),
                                             ui->MaxVertycalCrossing->value(), ui->MinVertycalCrossing->value(),
                                             ui->MaxHorizontalCrossing->value(), ui->MinHorizontalCrossing->value());
        else
            stitching = new Gradient_stitching(ui->CatalogPath->text(),
                                               ui->MaxVertycalCrossing->value(), ui->MinVertycalCrossing->value(),
                                               ui->MaxHorizontalCrossing->value(), ui->MinHorizontalCrossing->value());
    }
    else
    {
        if(ui->MinimalStitching->isChecked())
            stitching = new MinimalStitching_3D(ui->CatalogPath->text(),
                                                ui->MaxVertycalCrossing->value(), ui->MinVertycalCrossing->value(),
                                                ui->MaxHorizontalCrossing->value(), ui->MinHorizontalCrossing->value());
        else
            stitching = new Gradient_stitching_3D(ui->CatalogPath->text(),
                                                  ui->MaxVertycalCrossing->value(), ui->MinVertycalCrossing->value(),
                                                  ui->MaxHorizontalCrossing->value(), ui->MinHorizontalCrossing->value());
    }
    connect(stitching, SIGNAL(finish(std::vector<std::vector<fileDescription>>&,std::vector<QString>&,  QImage&)),
            this, SLOT(FS(std::vector<std::vector<fileDescription>>&, std::vector<QString>&, QImage&)));
    connect(stitching, SIGNAL(ErrorStitching()), this, SLOT(ErrorFileStitching()));
    this->close();
    stitching->start();
}

void Open_Form::FS(std::vector<std::vector<fileDescription> > &AD, std::vector<QString> &Ls, QImage &m)
{
    emit finish_stitching(AD, Ls, m);
}

void Open_Form::ErrorFileStitching()
{
    delete stitching;
    stitching =0;
    this->show();
}
