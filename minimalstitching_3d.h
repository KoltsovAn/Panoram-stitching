#ifndef MINIMALSTITCHING_3D_H
#define MINIMALSTITCHING_3D_H
#include "stitching_frames_3d.h"


class MinimalStitching_3D : public Stitching_Frames_3D
{
public:
    MinimalStitching_3D(QString path, int MaxV, int MinV, int MaxH, int MinH);
    virtual void start() override;
private:
    int cshivkaHorizontal(QString first, QString second, int xMin, int xMax);
    int cshivkaVertical(QString first, QString second, int yMin, int yMax);
};

#endif // MINIMALSTITCHING_3D_H
