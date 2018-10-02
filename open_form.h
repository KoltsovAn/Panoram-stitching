#ifndef OPEN_FORM_H
#define OPEN_FORM_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>


#include "minimalstitching.h"
#include "gradient_stitching.h"
#include "minimalstitching_3d.h"
#include "gradient_stitching_3d.h"

namespace Ui {
class Open_Form;
}

class Open_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Open_Form(QWidget *parent = 0);
    ~Open_Form();

private slots:
    void on_SelectCatalog_clicked();
    void on_CancelButton_clicked();

    void on_OkButton_clicked();
    void FS(std::vector<std::vector<fileDescription>> &AD, std::vector<QString>& Ls, QImage&m);
    void ErrorFileStitching();

signals:
    void finish();
    void finish_stitching(std::vector<std::vector<fileDescription>>&, std::vector<QString>& , QImage&);

private:
    Ui::Open_Form *ui;

    Stitching_Frames *stitching = 0;
};

#endif // OPEN_FORM_H
