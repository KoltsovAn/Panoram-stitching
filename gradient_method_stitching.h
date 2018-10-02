#ifndef GRADIENT_METHOD_STITCHING_H
#define GRADIENT_METHOD_STITCHING_H

#include <QImage>
#include <QRgb>
#include <QString>
#include <algorithm>

#include "point.h"


class gradient_method_stitching
{
public:
    gradient_method_stitching(QImage *FirstFileIMG, QImage *SecondFileIMG,
                              int MinH, int MaxH, int MinV, int MaxV);
    void start();
    std::vector<Point> &GetObject();

private:
    QImage *FirstIMG;
    QImage *SecondImg;
    int MinimalHorizontal;
    int MaximumHorizontal;
    int MinimumVertycal;
    int MaximumVerycal;

    std::vector<Point> FindObjectFirstIMG;
    std::vector<Point> FindObjectSecondIMG;

    void MeddianFilter(QImage *IMG);
    int DefinitionTresholdBinarization(QImage *IMG);
    void FindObject(QImage *IMG, std::vector<Point> &Container, QPoint From, QPoint To);
    Point CalculateObject(QImage *IMG, QPoint p);
    void CalculateGradient(QImage *IMG, Point &p);
};

const std::vector< std::vector< int> > Gx = {{-1, -2, -1},
                                       { 0,  0,  0},
                                       {-1, -2, -1}};

const std::vector< std::vector< int> > Gy = {{-1, 0, -1},
                                       {-2, 0, -2},
                                       {-1, 0, -1}};

#endif // GRADIENT_METHOD_STITCHING_H
