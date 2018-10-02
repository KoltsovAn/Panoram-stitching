#include "point.h"

Point::Point(QPoint p, int dw) : CenterOfMass(p), DeltaWindow(dw)
{
    SamePointCenterMass.setX(0);
    SamePointCenterMass.setY(0);
}

int Point::GetDeltaWindow()
{
    return DeltaWindow;
}

QPoint Point::GetCenterMass()
{
    return CenterOfMass;
}

QPoint Point::GetSameCenter()
{
    return SamePointCenterMass;
}

void Point::SetGradient(std::vector<double> &g)
{
    gradient.swap(g);
//    qDebug()<<"save gradient";
//    for(std::pair<double, QPoint> p: gradient)
//        qDebug()<<p.first;
}

std::vector<double> &Point::GetGradient()
{
    return gradient;
}

bool Point::operator <(Point &scnd)
{
    std::vector<double> scndGRD = scnd.GetGradient();
    int N = 0;
//    qDebug()<<"1:"<<gradient.size()<<":"<<scndGRD.size();
    for(size_t i = 0; i < gradient.size(); i++)
    {
//        qDebug()<<gradient.at(i).first;
//        qDebug()<<scndGRD.at(i).first;
        if(std::abs(gradient.at(i)-scndGRD.at(i)) < gradient.at(i)*0.01)
            N++;
    }
    if(gradient.size()*0.85 < N)
    {
//        qDebug()<<N;
        SamePointCenterMass = scnd.GetCenterMass();
        return true;
    }
    else return false;
}
