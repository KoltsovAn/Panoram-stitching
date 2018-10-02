#ifndef RECOVERY_FORM_H
#define RECOVERY_FORM_H

#include <QWidget>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QTextStream>
#include <QProgressDialog>
#include <QDir>
#include "iteminfo.h"

namespace Ui {
class Recovery_form;
}

class Recovery_form : public QWidget
{
    Q_OBJECT

public:
    explicit Recovery_form(QWidget *parent = 0);
    ~Recovery_form();

private slots:
    void on_FileButton_clicked();

    void on_CatalogButton_clicked();

    void on_CanctlButton_clicked();

    void on_OkButton_clicked();

private:
    Ui::Recovery_form *ui;

    std::vector<std::vector<fileDescription>> AllDescription;
    QProgressDialog *pprd;
signals:
    void finish();
    void recovery_finish(std::vector<std::vector<fileDescription>>&, std::vector<QString>& , QImage&);

};

#endif // RECOVERY_FORM_H
