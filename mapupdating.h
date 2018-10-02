#ifndef MAPUPDATING_H
#define MAPUPDATING_H

#include <QImage>
#include <QObject>
#include <QPointF>

#include <QDebug>

class MapUpdating : public QObject
{
    Q_OBJECT
public:
    MapUpdating(QImage &im, QPointF point1, QPointF point2, QObject *parent = 0);
    void updateStart();
private slots:
    void UpdateRun();
private:
    QImage &map;
    QPointF From;
    QPointF To;
signals:
    void UpdRun();
    void UpdatingFinish();
};

#endif // MAPUPDATING_H
