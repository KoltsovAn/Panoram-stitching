#ifndef GRADIENT_STITCHING_3D_H
#define GRADIENT_STITCHING_3D_H

#include "stitching_frames_3d.h"
#include "gradient_method_stitching.h"
#include "focus.h"

class Gradient_stitching_3D : public Stitching_Frames_3D
{
public:
    Gradient_stitching_3D(QString path, int MaxV=512, int MinV=1, int MaxH=256, int MinH=1);
    virtual void start() override;
};

#endif // GRADIENT_STITCHING_3D_H
