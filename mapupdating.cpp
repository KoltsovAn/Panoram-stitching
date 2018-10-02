#include "mapupdating.h"

MapUpdating::MapUpdating(QImage &im, QPointF point1, QPointF point2, QObject *parent) :
    map(im), From(point1), To(point2), QObject(parent)
{
    connect(this, SIGNAL(UpdRun()), this, SLOT(UpdateRun()));
}

void MapUpdating::updateStart()
{
    emit UpdRun();
}

void MapUpdating::UpdateRun()
{
    qDebug()<<From<<To;
    for (int y = From.y(); y < To.y(); y++)
    {
        QRgb *row = (QRgb*)map.scanLine(y);
        for (int x = From.x(); x < To.x(); x++)
            ((unsigned char*)&row[x])[3] = 255;
    }
    emit UpdatingFinish();
}
