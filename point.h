#ifndef POINT_H
#define POINT_H

#include <QPoint>
#include <vector>
#include <utility>
#include <cmath>
#include <QDebug>

class Point
{
public:
    Point(QPoint p, int dw);
    bool ComparePoint(Point SecondPoint);
    int GetDeltaWindow();
    QPoint GetCenterMass();
    QPoint GetSameCenter();
    void SetGradient(std::vector<double> &g);
    std::vector<double> &GetGradient();
    bool operator < (Point &scnd);
private:
    QPoint CenterOfMass;
    int DeltaWindow;
    std::vector<double> gradient;
    QPoint SamePointCenterMass;

};

#endif // POINT_H
