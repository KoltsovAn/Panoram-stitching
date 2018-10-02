#ifndef STITCHING_FRAMES_3D_H
#define STITCHING_FRAMES_3D_H

#include "stitching_frames.h"
#include "focus.h"


class Stitching_Frames_3D : public Stitching_Frames
{
protected:
    Stitching_Frames_3D(QString path, int MaxV, int MinV, int MaxH, int MinH) :
        Stitching_Frames(path, MaxV, MinV, MaxH, MinH)
    {
         QDir d(path);
         d.setFilter(QDir::Dirs);
         Layers=d.entryList();
         if(Layers.size() == 0)
         {
             QMessageBox MB;
             MB.setText("В папке отсутствуют каталоги с панорамой!");
             MB.exec();
             emit ErrorStitching();
             return;
         }
         Layers.pop_front();
         Layers.pop_front();
         for(QString &str: Layers)
             str = path+"/"+str;
    };
    QStringList Layers;
public:
        virtual void start() override = 0;
};

#endif // STITCHING_FRAMES_3D_H
