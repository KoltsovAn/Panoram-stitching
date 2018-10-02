#ifndef GRADIENT_STITCHING_H
#define GRADIENT_STITCHING_H

#include "gradient_method_stitching.h"
#include "stitching_frames.h"

class Gradient_stitching : public Stitching_Frames
{
public:
    Gradient_stitching(QString path, int MaxV=512, int MinV=1, int MaxH=256, int MinH=1);
    virtual void start() override;
};

#endif // GRADIENT_STITCHING_H
