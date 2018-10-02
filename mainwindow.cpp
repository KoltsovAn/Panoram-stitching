#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new myGraphicsScene();
    mapScene= new QGraphicsScene();
    ui->Picture->setScene(scene);
    ui->map->setScene(mapScene);

    connect(scene,SIGNAL(mousePressed()),this,SLOT(Pressed()));
    connect(scene,SIGNAL(mouseReleased()),this,SLOT(Released()));
    connect(scene,SIGNAL(wheel(int)),this,SLOT(Wheel(int)));
    connect(scene,SIGNAL(move()),this,SLOT(move()));
    connect(ui->Picture->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(viewChange()), Qt::QueuedConnection);
    connect(ui->Picture->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(viewChange()), Qt::QueuedConnection);
    connect(ui->Picture->verticalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(viewChange()), Qt::QueuedConnection);
    connect(this, SIGNAL(ViewZoneChange(QPointF,QPointF)), scene, SLOT(ViewZoneChange(QPointF,QPointF)), Qt::QueuedConnection);
    connect(this, SIGNAL(MapZoneChange(QPointF,QPointF)), this, SLOT(UpdateMap(QPointF,QPointF)));
    connect(this, SIGNAL(UpdateSceneAllDescription(TypeOfMouseEvent)),
            scene, SLOT(UpdatePositionAllDescription(TypeOfMouseEvent)), Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    if(OpenForm != nullptr || RecoveryForm != nullptr)
        return;
    OpenForm =new Open_Form();
    connect(OpenForm,SIGNAL(finish_stitching(std::vector<std::vector<fileDescription> >&, std::vector<QString>& , QImage&)),
            this,SLOT(OpenFormFinishStitching(std::vector<std::vector<fileDescription> >&, std::vector<QString>& , QImage&)));
    connect(OpenForm, SIGNAL(finish()), this, SLOT(OpenFormFinish()));
    OpenForm->show();
}

void MainWindow::on_action_2_triggered()
{
    if (SaveForm != nullptr || RecoveryForm != nullptr)
        return;
    SaveForm = new Save_Form(scene, W, H);
    connect(SaveForm, SIGNAL(finish()), this, SLOT(SaveFormFinish()));
    connect(SaveForm, SIGNAL(UpdateLoadZone(QPointF,QPointF)), scene, SLOT(ViewZoneChange(QPointF,QPointF)));
    connect(SaveForm, SIGNAL(OriginalImagePositionSave(QTextStream&)),
            scene, SLOT(SavePositionOriginalFrame(QTextStream&)));
    SaveForm->show();
}

void MainWindow::on_action_3_triggered()
{
    if(SaveForm != nullptr || OpenForm != nullptr)
        return;
    RecoveryForm = new Recovery_form();
    connect(RecoveryForm, SIGNAL(finish()), this, SLOT(OpenFormFinish()));
    connect(RecoveryForm, SIGNAL(recovery_finish(std::vector<std::vector<fileDescription> >&,std::vector<QString>&,QImage&)),
            this, SLOT(OpenFormFinishStitching(std::vector<std::vector<fileDescription> >&,std::vector<QString>&,QImage&)));
    RecoveryForm->show();
}

void MainWindow::on_pushButton_8_clicked()
{
//        qDebug()<<"scale!";
    ui->Picture->scale(1/1.5,1/1.5);
    QPolygonF polygon =ui->Picture->mapToScene(0,0,ui->Picture->viewport()->width(),ui->Picture->viewport()->height());
    if((2*W)<(polygon[2].x()-polygon[0].x()) && (2*H)<(polygon[2].y()-polygon[0].y()))
        ui->Picture->scale(1.5,1.5);
//        qDebug()<<"scale end!";
}

void MainWindow::on_pushButton_9_clicked()
{
//    qDebug()<<"scale!";
    ui->Picture->scale(1.5,1.5);
    QPolygonF polygon =ui->Picture->mapToScene(0,0,ui->Picture->viewport()->width(),ui->Picture->viewport()->height());
    if(W/2>(polygon[2].x()-polygon[0].x()) && H/2>(polygon[2].y()-polygon[0].y()))
        ui->Picture->scale(1/1.5,1/1.5);
//    qDebug()<<"scale end";
}

void MainWindow::Pressed()
{
//    qDebug()<<"pressd:";
    if (!limitFrame)
    {
        if(Ctrl)
        {
//            qDebug()<<"Ctrl";
            if(rollback != -1)
            {
                next_item_pos.pop_back();
                for(int k = last_item_pos.size()-1;k!=rollback;k--)
                {
                    last_item_pos.pop_back();
                    next_item_pos.pop_back();
                }
                rollback = -1;
            }
            foundItems = scene->items(QPolygonF()
                                      << QPointF(-10, scene->getY())
                                      << QPointF(scene->width(), scene->getY())
                                      << QPointF(scene->width(), scene->height())
                                      << QPointF(-10, scene->height()));
            if(!foundItems.isEmpty())
            {
                std::vector<last_pos> lp;
                foreach(QGraphicsItem* item, foundItems)
                {
                    item->setSelected(true);
                    lp.push_back(last_pos(item,item->pos().x(),item->pos().y()));
                }
                last_item_pos.push_back(lp);
            }
        }
        if (Shift)
        {
            //                qDebug()<<"Shift";
            if(rollback != -1)
            {
                next_item_pos.pop_back();
                for(int k = last_item_pos.size()-1;k!=rollback;k--)
                {
                    last_item_pos.pop_back();
                    next_item_pos.pop_back();
                }
                rollback = -1;
            }
            foundItems = scene->items(QPolygonF()
                                      << QPointF(scene->getX(), scene->getY())
                                      << QPointF(scene->width(), scene->getY())
                                      << QPointF(scene->width(), scene->getY()+5)
                                      << QPointF(scene->getX(), scene->getY()+5));
            if(!foundItems.isEmpty())
            {
                std::vector<last_pos> lp;
                foreach(QGraphicsItem* item, foundItems)
                {
                    item->setSelected(true);
                    lp.push_back(last_pos(item,item->pos().x(),item->pos().y()));
                }
                last_item_pos.push_back(lp);
            }
        }
//        if (Alt)
//        {
//            if(FirstP == QPoint(0, 0))
//            {
//                FirstP.setX(scene->getX());
//                FirstP.setY(scene->getY());
//            }
//            else
//            {
//                delete ManualStitching;
//                int FirstImgX = scene->getPosX(FirstP.x()/W, FirstP.y()/H);
//                int FirstImgY = scene->getPosY(FirstP.x()/W, FirstP.y()/H);
//                int SecondImgX = scene->getPosX(scene->getX()/W, scene->getY()/H);
//                int SecondImgY = scene->getPosY(scene->getX()/W, scene->getY()/H);
//                ManualStitching = new Analysis_of_a_manual_stitching(scene->getName(FirstP.x()/W, FirstP.y()/H),
//                                                                     scene->getName(scene->getX()/W, scene->getY()/H),
//                                                                     QPoint(SecondImgX-FirstImgX, SecondImgY-FirstImgY),
//                                                                     QPoint(W, H), QPoint(0,0),
//                                                                     QPoint(FirstImgX+W-SecondImgX, FirstImgY+H-SecondImgY));
//            }
//        }
    }
}

void MainWindow::Released()
{
//    qDebug() <<"Release";
    if (!limitFrame)
    {
    if(!foundItems.isEmpty())
    {
        std::vector<last_pos> lp;
        foreach(QGraphicsItem* item, foundItems)
        {
            item->setSelected(false);
            lp.push_back(last_pos(item,item->pos().x(),item->pos().y()));
        }
        next_item_pos.push_back(lp);
        foundItems.clear();
    }
    if(Ctrl)
        emit UpdateSceneAllDescription(CtrlEvent);
    if(Shift)
        emit UpdateSceneAllDescription(ShiftEvent);
    if(oneItemMove)
        emit UpdateSceneAllDescription(OneItemMoveEvent);
    }
}

void MainWindow::Wheel(int delta)
{
//        qDebug()<<"scale!";
    if (delta>0)
    {
        ui->Picture->scale(1.5,1.5);
        QPolygonF polygon =ui->Picture->mapToScene(0,0,ui->Picture->viewport()->width(),ui->Picture->viewport()->height());
        if(W/2.5>(polygon[2].x()-polygon[0].x()) && H/2.5>(polygon[2].y()-polygon[0].y()))
            ui->Picture->scale(1/1.5,1/1.5);
    }
    else
    {
        ui->Picture->scale(1/1.5,1/1.5);
        QPolygonF polygon =ui->Picture->mapToScene(0,0,ui->Picture->viewport()->width(),ui->Picture->viewport()->height());
        if((3*W)<(polygon[2].x()-polygon[0].x()) && (3*H)<(polygon[2].y()-polygon[0].y()))
            ui->Picture->scale(1.5,1.5);
    }
//        qDebug()<<"scale end";
}

void MainWindow::move()
{
//    qDebug()<<"move";
//    ui->Picture->verticalScrollBar()->setValue(
//                ui->Picture->verticalScrollBar()->value()+
//                scene->getY()-scene->secondPoint.y());
//    ui->Picture->horizontalScrollBar()->setValue(
//                ui->Picture->horizontalScrollBar()->value()+
//                scene->getX()-scene->secondPoint.x());
}

void MainWindow::viewChange()
{
//    qDebug()<< "Изменение зоны видимости!";
    QPointF point1=ui->Picture->mapToScene(0,0);
    if(point1.x() < 0)
        point1.setX(0);
    if(point1.y() < 0)
        point1.setY(0);
    QPointF point2=ui->Picture->mapToScene(ui->Picture->viewport()->width(),ui->Picture->viewport()->height());
    if(point2.x() > scene->width()-1)
        point2.setX(scene->width()-1);
    if(point2.y() > scene->height()-1)
        point2.setY(scene->height()-1);
    ui->pix->setText(QString::number((point2.x()-point1.x())/ui->Picture->width(),'g',1));
//    qDebug()<< "сцена!";
    emit ViewZoneChange(point1, point2);
//    qDebug() <<"Карта!";
    emit MapZoneChange(point1, point2);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
//    qDebug()<<"KeyPress:";
    if (!limitFrame)
    {
        if(oneItemMove)
        {
            if(!Shift && !Ctrl && !Alt)
                switch (event->key()) {
                case Qt::Key_Shift:
//                    qDebug()<<"Shift";
                    Shift=true;
                    foundItems = scene->items(QPolygonF()
                                              << QPointF(0,0)
                                              << QPointF(scene->width(), 0)
                                              << QPointF(scene->width(), scene->height())
                                              << QPointF(0, scene->height()));
                    if(!foundItems.isEmpty())
                        foreach(QGraphicsItem* item, foundItems)
                            item->setFlag(QGraphicsItem::ItemIsMovable,true);
                    foundItems.clear();
                    break;
                case Qt::Key_Control:
//                    qDebug()<<"Ctrl";
                    Ctrl=true;
                    foundItems = scene->items(QPolygonF()
                                              << QPointF(0,0)
                                              << QPointF(scene->width(), 0)
                                              << QPointF(scene->width(), scene->height())
                                              << QPointF(0, scene->height()));
                    if(!foundItems.isEmpty())
                        foreach(QGraphicsItem* item, foundItems)
                            item->setFlag(QGraphicsItem::ItemIsMovable,true);

                    foundItems.clear();
                    break;
                default:
                    break;
                }
        }
        else
            if(!Shift && !Ctrl && !Alt)
                switch (event->key()) {
                case Qt::Key_Shift:
//                    qDebug()<<"Shift";
                    Shift=true;
                    foundItems = scene->items(QPolygonF()
                                              << QPointF(0,0)
                                              << QPointF(scene->width(), 0)
                                              << QPointF(scene->width(), scene->height())
                                              << QPointF(0, scene->height()));
                    if(!foundItems.isEmpty())
                        foreach(QGraphicsItem* item, foundItems)
                        {
                            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
                            item->setFlag(QGraphicsItem::ItemIsMovable,true);
                        }
                    foundItems.clear();
                    break;
                case Qt::Key_Control:
//                    qDebug()<<"Ctrl";
                    Ctrl=true;
                    foundItems = scene->items(QPolygonF()
                                              << QPointF(0,0)
                                              << QPointF(scene->width(), 0)
                                              << QPointF(scene->width(), scene->height())
                                              << QPointF(0, scene->height()));
                    if(!foundItems.isEmpty())
                    {
                        foreach(QGraphicsItem* item, foundItems)
                        {
                            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
                            item->setFlag(QGraphicsItem::ItemIsMovable,true);
                        }
                    }

                    foundItems.clear();
                    break;
                default:
                    break;
                }
    }
//    if (ui->manualStitching->isChecked())
//        if(!Shift && !Ctrl && !Alt)
//            if(event->key() == Qt::Key_Alt)
//            {
//                //                    qDebug()<<"Alt";
//                Alt = true;
//                foundItems = scene->items(QPolygonF()
//                                          << QPointF(0,0)
//                                          << QPointF(scene->width(), 0)
//                                          << QPointF(scene->width(), scene->height())
//                                          << QPointF(0, scene->height()));
//                if(!foundItems.isEmpty())
//                    foreach(QGraphicsItem* item, foundItems)
//                    {
//                        item->setFlag(QGraphicsItem::ItemIsSelectable,true);
//                        item->setSelected(true);
//                    }
//                foundItems.clear();
//            }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
//    qDebug()<<"KeyRelease:";
    if (!limitFrame)
        switch (event->key()) {
        case Qt::Key_Shift:
            if(!Shift)
                break;
//            qDebug()<<"Shift";
            Shift=false;
            foundItems = scene->items(QPolygonF()
                                      << QPointF(0,0)
                                      << QPointF(scene->width(), 0)
                                      << QPointF(scene->width(), scene->height())
                                      << QPointF(0, scene->height()));
            if(!foundItems.isEmpty())
            {
                foreach(QGraphicsItem* item, foundItems)
                {
                    item->setFlag(QGraphicsItem::ItemIsSelectable,false);
                    item->setFlag(QGraphicsItem::ItemIsMovable,false);
                }
            }
            foundItems.clear();
            break;
        case Qt::Key_Control:
            if(!Ctrl)
                break;
//            qDebug()<<"Ctrl";
            Ctrl=false;
            foundItems = scene->items(QPolygonF()
                                      << QPointF(0,0)
                                      << QPointF(scene->width(), 0)
                                      << QPointF(scene->width(), scene->height())
                                      << QPointF(0, scene->height()));
            if(!foundItems.isEmpty())
            {
                foreach(QGraphicsItem* item, foundItems)
                {
                    item->setFlag(QGraphicsItem::ItemIsSelectable,false);
                    item->setFlag(QGraphicsItem::ItemIsMovable,false);
                }
            }
            foundItems.clear();
            break;
//        case Qt::Key_Alt:
//            if(!Alt)
//                break;
//            Alt = false;
//            foundItems = scene->items(QPolygonF()
//                                      << QPointF(0,0)
//                                      << QPointF(scene->width(), 0)
//                                      << QPointF(scene->width(), scene->height())
//                                      << QPointF(0, scene->height()));
//            if(!foundItems.isEmpty())
//                foreach(QGraphicsItem* item, foundItems)
//                {
//                    item->setFlag(QGraphicsItem::ItemIsSelectable,false);
//                    item->setSelected(false);
//                }
//            foundItems.clear();
        default:
            break;
        }
}

void MainWindow::on_checkBox_stateChanged(int)
{
    oneItemMove=!oneItemMove;
}

void MainWindow::on_checkBox_2_stateChanged(int)
{
    if (!limitFrame)
    {
    foundItems = scene->items(QPolygonF()
                              << QPointF(0, 0)
                              << QPointF(scene->width(), 0)
                              << QPointF(scene->width(), scene->height())
                              << QPointF(0, scene->height()));
    if(!foundItems.isEmpty())
        foreach(QGraphicsItem* item, foundItems)
        {
            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            item->setSelected(true);
        }
    foundItems.clear();
    }
    else
    {
        foundItems = scene->items(QPolygonF()
                                  << QPointF(0, 0)
                                  << QPointF(scene->width(), 0)
                                  << QPointF(scene->width(), scene->height())
                                  << QPointF(0, scene->height()));
        if(!foundItems.isEmpty())
            foreach(QGraphicsItem* item, foundItems)
            {
                item->setFlag(QGraphicsItem::ItemIsSelectable,false);
                item->setSelected(false);
            }
        foundItems.clear();
    }
    limitFrame=!limitFrame;
}

void MainWindow::SaveFormFinish()
{
    QPointF point1=ui->Picture->mapToScene(0,0);
    QPointF point2=ui->Picture->mapToScene(ui->Picture->viewport()->width(),ui->Picture->viewport()->height());
    ui->pix->setText(QString::number((point2.x()-point1.x())/ui->Picture->width(),'g',1));
    emit ViewZoneChange(point1, point2);
    delete SaveForm;
    SaveForm = 0;
}

void MainWindow::OpenFormFinish()
{
    if(OpenForm != nullptr)
    {
        delete OpenForm;
        OpenForm = nullptr;
    }
    if(RecoveryForm != nullptr)
    {
        delete RecoveryForm;
        RecoveryForm = nullptr;
    }
}

void MainWindow::OpenFormFinishStitching(std::vector<std::vector<fileDescription> > &AD, std::vector<QString> &Ls, QImage &m)
{
    delete mapImage;
    mapImage = new QImage(m);
    scene->InitializationAllDescription(AD, Ls);
    ui->NumberOfLayer->clear();
    if(Ls.size() == 1)
    {
        ui->Layer_label->hide();
        ui->NumberOfLayer->hide();
    }
    else
    {
        ui->Layer_label->show();
        ui->NumberOfLayer->show();
        for(QString s: Ls)
            ui->NumberOfLayer->addItem(s.split("/").back());
    }
    QPixmap pm;
    pm.convertFromImage(*mapImage);
    delete pixMapItem;
    pixMapItem = new QGraphicsPixmapItem(pm);
    pixMapItem->setPos(0, 0);
    mapScene->setSceneRect(0, 0 , pm.width(), pm.height());
    mapScene->addItem(pixMapItem);
    ui->map->fitInView(0,0,mapScene->width(),mapScene->height());
//    qDebug()<<pm.size();

    QPointF point1=ui->Picture->mapToScene(0,0);
    QPointF point2=ui->Picture->mapToScene(ui->Picture->viewport()->width(),ui->Picture->viewport()->height());
    delete rectItem;
    rectItem = new QGraphicsRectItem(point1.x()/(scene->width()/mapImage->width()),
                                     point1.y()/(scene->height()/mapImage->height()),
                                     (point2.x()-point1.x())/(scene->width()/mapImage->width()),
                                     (point2.y()-point1.y())/(scene->height()/mapImage->height()));
    rectItem->setPen(QPen(Qt::red, 10));
    rectItem->setZValue(10);
//    qDebug()<<point2;
//    qDebug()<<(1.0*ui->map->viewport()->width()/ui->Picture->viewport()->width());
//    qDebug()<<(1.0*ui->map->viewport()->height()/ui->Picture->viewport()->height());

    mapScene->addItem(rectItem);
//    qDebug()<<rectItem;

    QImage I(Ls.at(0)+AD.at(0).at(0).getFileName());
    W=I.width();
    H=I.height();

    if(OpenForm != nullptr)
    {
        delete OpenForm;
        OpenForm = nullptr;
    }
    if(RecoveryForm != nullptr)
    {
        delete RecoveryForm;
        RecoveryForm = nullptr;
    }
//    qDebug()<< "инициализация сцены завершена";
}

void MainWindow::UpdateMap(QPointF point1, QPointF point2)
{
//    qDebug()<<"меняю положение квадрата на карте";
//    qDebug()<< point1<<point2;
//    qDebug()<<scene->width()<<";"<<scene->height();
//    qDebug()<<mapImage->width()<<";"<<mapImage->height();
    if(point1.x() >= 0 && point1.y() >= 0 && point2.x() >= 0 && point2.y() >= 0 )
    {
        if(rectItem != nullptr)
            rectItem->setRect(point1.x()/(scene->width()/mapImage->width()),
                              point1.y()/(scene->height()/mapImage->height()),
                             (point2.x()-point1.x())/(scene->width()/mapImage->width()),
                             (point2.y()-point1.y())/(scene->height()/mapImage->height()));
        if(MapUpdateThread == 0)
        {
//            qDebug()<< "Создание нового потока обновления карты!";
            MapUpdateThread = new QThread;
            MapUpdateThread->start();
//            qDebug()<<QPoint(point1.x()/(scene->width()/mapImage->width()),
//                             point1.y()/(scene->height()/mapImage->height()));
//            qDebug()<<QPoint(point2.x()/(scene->width()/mapImage->width()),
//                             point2.y()/(scene->height()/mapImage->height()));
            MapUpd = new MapUpdating(*mapImage, QPoint(point1.x()/(scene->width()/mapImage->width()),
                                                       point1.y()/(scene->height()/mapImage->height())),
                                     QPoint(point2.x()/(scene->width()/mapImage->width()),
                                            point2.y()/(scene->height()/mapImage->height())));
            MapUpd->moveToThread(MapUpdateThread);
            connect(MapUpd, SIGNAL(UpdatingFinish()), this, SLOT(MapUpdatingFinish()));
            MapUpd->updateStart();
        }
    }
//        qDebug()<< "положение изменено. новое положение:";
}

void MainWindow::MapUpdatingFinish()
{
//    qDebug()<<"Загрузка новой карты.";
    //mapScene->clear();
    QPixmap pm;
    pm.convertFromImage(*mapImage);
    mapScene->removeItem(pixMapItem);
    delete pixMapItem;
    pixMapItem = new QGraphicsPixmapItem(pm);
//    pixMapItem->setPos(0, 0);
    mapScene->addItem(pixMapItem);
    //ui->map->fitInView(0,0,mapScene->width(),mapScene->height());
    //mapScene->addItem(rectItem);
    delete MapUpd;
    MapUpd = 0;
    MapUpdateThread->quit();
    MapUpdateThread->wait();
    delete MapUpdateThread;
    MapUpdateThread = 0;
//    qDebug()<<"Карта обнавлена.";
}

void MainWindow::on_last_clicked()
{
    if (rollback !=0)
    {
        if (rollback == -1)
        {
            rollback = last_item_pos.size()-1;
            foundItems = scene->items(QPolygonF()
                                      << QPointF(0,0)
                                      << QPointF(scene->width(), 0)
                                      << QPointF(scene->width(), scene->height())
                                      << QPointF(0, scene->height()));
            if(!foundItems.isEmpty())
            {
                std::vector<last_pos> lp;
                foreach(QGraphicsItem* item, foundItems)
                {
                    lp.push_back(last_pos(item,item->pos().x(),item->pos().y()));
                }
                next_item_pos.push_back(lp);
            }
            foundItems.clear();
            foreach (last_pos lp,last_item_pos[rollback])
            {
                lp.item->setPos(lp.posX,lp.posY);
            }

        }
        else
        {
            rollback--;
            foreach (last_pos lp,last_item_pos[rollback])
            {
                lp.item->setPos(lp.posX,lp.posY);
            }
        }
    }
}

void MainWindow::on_next_clicked()
{

    if(rollback != next_item_pos.size()-1)
    if(rollback != -1)
    {
        rollback++;
        foreach (last_pos lp,next_item_pos[rollback])
        {
            lp.item->setPos(lp.posX,lp.posY);
        }
    }
}

void MainWindow::on_NumberOfLayer_currentIndexChanged(int index)
{
    scene->LayerChange(index);
}
//void MainWindow::on_manualStitching_toggled(bool checked)
//{
//    if(checked)
//    {
//        ui->R_label->show();
//        ui->G_label->show();
//        ui->B_label->show();
//        ui->R_label_value->show();
//        ui->G_label_value->show();
//        ui->B_label_value->show();
//    }
//    else
//    {
//        ui->R_label->hide();
//        ui->G_label->hide();
//        ui->B_label->hide();
//        ui->R_label_value->hide();
//        ui->G_label_value->hide();
//        ui->B_label_value->hide();
//    }
//}

//void MainWindow::SetValuesStitching(double MiddleR, double MiddleG, double MiddleB)
//{
//    ui->R_label_value->setText(QString::number(MiddleR));
//    ui->G_label_value->setText(QString::number(MiddleG));
//    ui->B_label_value->setText(QString::number(MiddleB));
//}


