#include "iteminfo.h"

itemInfo::itemInfo(QString _fileName, QGraphicsItem *itemm):
    fileName(_fileName), item(itemm)
{

}
QGraphicsItem *itemInfo::getItem()
{
    return(this->item);
}

void itemInfo::setItem(QGraphicsItem *GI)
{
    item = GI;
}

QString itemInfo::getFileName()
{
    return (this->fileName);
}

fileDescription::fileDescription(QString _fileName, int X, int Y) : fileName(_fileName), posX(X), posY(Y)
{

}

QString fileDescription::getFileName()
{
    return(this->fileName);
}

void fileDescription::UpdatePosition(QPoint delta)
{
    posX = posX+delta.x();
    posY = posY+delta.y();
}

int fileDescription::getX()
{
    return(this->posX);
}

int fileDescription::getY()
{
    return(this->posY);
}



last_pos::last_pos(QGraphicsItem *itemm, int x, int y):item(itemm), posX(x), posY(y)
{

}
