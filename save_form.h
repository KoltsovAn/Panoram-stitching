#ifndef SAVE_FORM_H
#define SAVE_FORM_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QProgressDialog>
#include <QFileDialog>
#include <QImage>
#include <QPainter>
#include <QPointF>
#include <QTextStream>

#include <QDebug>
namespace Ui {
class Save_Form;
}

class Save_Form : public QWidget
{
    Q_OBJECT

public:
    Save_Form(QGraphicsScene *Scene, size_t Width, size_t Height, QWidget *parent = 0);
    ~Save_Form();

private slots:
    void on_CancelButton_clicked();
    void on_OkButton_clicked();
    void on_SelectCatalog_clicked();

signals:
    void finish();
    void UpdateLoadZone(QPointF point1, QPointF point2);
    void OriginalImagePositionSave(QTextStream &stream);

private:
    Ui::Save_Form *ui;
    QGraphicsScene *scene = 0;

    int W;
    int H;

    void derevo(QString fileName, QImage *saveImg,int sizeH, int sizeV, int poX, int poY);
};

#endif // SAVE_FORM_H
