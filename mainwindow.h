#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QListWidgetItem>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QProgressDialog>
#include <QThread>
#include <QPoint>

#include "mygraphicsscene.h"
#include "open_form.h"
#include "save_form.h"
#include "mapupdating.h"
#include "analysis_of_a_manual_stitching.h"
#include "recovery_form.h"

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

    Open_Form *OpenForm = nullptr;
    Save_Form *SaveForm = nullptr;
    Recovery_form *RecoveryForm = nullptr;

    QGraphicsScene *mapScene = nullptr;
    QImage *mapImage = nullptr;
    QGraphicsPixmapItem *pixMapItem = nullptr;
    myGraphicsScene *scene = nullptr;
    QList<QGraphicsItem *> foundItems;

    QGraphicsRectItem *rectItem = nullptr;
    QThread *MapUpdateThread = 0;
    MapUpdating *MapUpd = 0;

    QThread *ManualStitchingThread = nullptr;
    Analysis_of_a_manual_stitching *ManualStitching = nullptr;

    std::vector< std::vector < last_pos > > last_item_pos;
    std::vector< std::vector < last_pos > > next_item_pos;
    int rollback = -1;

    int W,H;

    QPoint FirstP = QPoint(0, 0);
    bool Alt = false;
    bool Ctrl = false;
    bool Shift = false;
    bool oneItemMove = false;
    bool limitFrame = false;

    TypeOfMouseEvent button;

private slots:
    void on_action_triggered();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_action_2_triggered();

    void Pressed();
    void Released();
    void Wheel(int delta);
    void move();
    void viewChange();
    void on_checkBox_stateChanged(int);
    void on_last_clicked();
    void on_next_clicked();
    void on_checkBox_2_stateChanged(int);

    void SaveFormFinish();
    void OpenFormFinish();
    void OpenFormFinishStitching(std::vector<std::vector<fileDescription>> &AD, std::vector<QString> &Ls, QImage &m);

    void UpdateMap(QPointF point1, QPointF point2);
    void MapUpdatingFinish();
//    void on_manualStitching_toggled(bool checked);
//    void SetValuesStitching(double MiddleR, double MiddleG, double MiddleB);

    void on_NumberOfLayer_currentIndexChanged(int index);

    void on_action_3_triggered();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void ViewZoneChange(QPointF point1, QPointF point2);
    void MapZoneChange(QPointF point1, QPointF point2);
    void UpdateSceneAllDescription(TypeOfMouseEvent event);
};

#endif // MAINWINDOW_H
