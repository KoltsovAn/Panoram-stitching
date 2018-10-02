#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QObject>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QPoint>
#include <vector>
#include <list>
#include <algorithm>


#include <QDebug>

#include "iteminfo.h"

enum TypeOfMouseEvent{CtrlEvent, ShiftEvent, OneItemMoveEvent};

class myGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit myGraphicsScene(/*QObject *parent = nullptr*/);
    int getX();
    int getY();
    void InitializationAllDescription(std::vector<std::vector<fileDescription>> &AD, std::vector<QString> &Ls);
    QString GetCurentDir();
    QString getName(int x, int y);
    void LayerChange(int index);
    int getPosX(int x, int y);
    int getPosY(int x, int y);
    int horizontalSize();
    int verticalSize();


public slots:
    void ViewZoneChange(QPointF point1, QPointF point2);
    void SavePositionOriginalFrame(QTextStream &stream);
    void UpdatePositionAllDescription(TypeOfMouseEvent event);

private:
    std::list<itemInfo> itemInScene ;
    std::vector<std::vector<fileDescription>> AllDescription;
    std::vector<QString> Layers;
    QString CurentLayer;
    QSize SizeOfFrame;
    QPoint firstpoint;
    QPoint secondPoint;

    void ViewNewItem(size_t x, size_t y);

signals:
    void mousePressed();
    void mouseReleased();
    void wheel(int del);
    void move();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // MYGRAPHICSSCENE_H
