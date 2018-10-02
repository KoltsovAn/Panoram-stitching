#ifndef FOCUS_H
#define FOCUS_H

#include <QString>
#include <QImage>
#include <vector>
#include <QColor>
#include <QPoint>
#include <limits>

class focus_img
{
    const QString fileName;
    const QStringList &dirs;
    QString MaxFocusDir;
    double MaxFocusValue ;
    const int X1;
    const int Y1;
    const int X2;
    const int Y2;
public:
    focus_img(QString fn, QStringList &d, QPoint from, QPoint to): fileName(fn), dirs(d),
                                                                X1(from.x()), Y1(from.y()), X2(to.x()), Y2(to.y())
    {MaxFocusValue = std::numeric_limits<double>::max();
        MaxFocusValue = -MaxFocusValue;};
    QString start();
};

#endif // FOCUS_H
