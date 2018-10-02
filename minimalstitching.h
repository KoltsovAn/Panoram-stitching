#ifndef MINIMALSTITCHING_H
#define MINIMALSTITCHING_H

#include "stitching_frames.h"

class MinimalStitching : public Stitching_Frames
{
public:
    MinimalStitching(QString path, int MaxV=512, int MinV=1, int MaxH=256, int MinH=1);
    virtual void start() override;
private:
    int cshivkaHorizontal(QImage firstImg, QImage secondImg, int xMin, int xMax);
    int cshivkaVertical(QImage firstImg, QImage secondImg, int yMin, int yMax);
};

#endif // MINIMALSTITCHING_H
