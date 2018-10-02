#ifndef ITEMINFO_H
#define ITEMINFO_H

#include <QGraphicsPixmapItem>
#include <QPoint>


class itemInfo
{
public:
    itemInfo(QString _fileName,QGraphicsItem *itemm);
    QGraphicsItem *getItem();
    void setItem(QGraphicsItem* GI);
    QString getFileName();
private:
    QString fileName;
    QGraphicsItem *item;

};

class fileDescription
{
public:
    fileDescription(QString _fileName, int X, int Y);
    QString getFileName();
    void UpdatePosition(QPoint delta);
    int getX();
    int getY();
private:
    QString fileName;
    int posX;
    int posY;
};

class last_pos
{
public:
    last_pos(QGraphicsItem *itemm, int x, int y);

    QGraphicsItem *item;
    int posX;
    int posY;
};


#endif // ITEMINFO_H
