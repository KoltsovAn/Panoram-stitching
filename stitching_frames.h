#ifndef STITCHING_FRAMES_H
#define STITCHING_FRAMES_H

#include <QObject>
#include <QString>
#include <vector>
#include <QImage>
#include <QDir>
#include <QProgressDialog>
#include <QMessageBox>
#include <QApplication>
#include "iteminfo.h"

#include <QDebug>

class Stitching_Frames : public QObject
{
    Q_OBJECT
protected:
    explicit Stitching_Frames(QString path, int MaxV=512, int MinV=1, int MaxH=256, int MinH=1, QObject *parent = nullptr) :
         QObject(parent), DirPath(path), MaxVertycalCrossing(MaxV), MinVertycalCrossing(MinV),
         MaxHorizontalCrossing(MaxH), MinHorizontalCrossing(MinH) {};


    QString DirPath;
    int MaxVertycalCrossing;
    int MinVertycalCrossing;
    int MaxHorizontalCrossing;
    int MinHorizontalCrossing;

    std::vector<std::vector<fileDescription>> AllDescription;

    QProgressDialog *pprd;

public:
    virtual void start() = 0;
    virtual ~Stitching_Frames()
    {    pprd->setValue(pprd->maximum());
         delete pprd;
    };

signals:
    void finish(std::vector<std::vector<fileDescription>>&, std::vector<QString>&, QImage&);
    void ErrorStitching();

};

#endif // STITCHING_FRAMES_H
