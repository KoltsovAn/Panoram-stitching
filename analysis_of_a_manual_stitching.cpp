#include "analysis_of_a_manual_stitching.h"

Analysis_of_a_manual_stitching::Analysis_of_a_manual_stitching(QString firstIMGfileName, QString secondIMGfileName,
                                                               QPoint fromP1, QPoint toP1,
                                                               QPoint fromP2, QPoint toP2) :
    FromFirstIMG(fromP1), ToFirstIMG(toP1), FromSecondIMG(fromP2), ToSecondIMG(toP2)
{
    firstIMG = new QImage(firstIMGfileName);
    firstIMG->convertToFormat(QImage::Format_RGB32);
    secondIMG = new QImage(secondIMGfileName);
    secondIMG->convertToFormat(QImage::Format_RGB32);
    firstIMGbits = firstIMG->bits();
    secondIMGbits = secondIMG->bits();
}

void Analysis_of_a_manual_stitching::Calculate()
{

    int middleR = 0, middleG = 0, middleB = 0;

    for(int yFirst = FromFirstIMG.y(), ySecond = FromSecondIMG.y();
        yFirst < ToFirstIMG.y() && ySecond < ToSecondIMG.y(); yFirst++, ySecond++)
        for(int firstIMGindex = (yFirst*FromFirstIMG.x()*3), secondIMGindex = (ySecond*FromSecondIMG.x()*3);
            firstIMGindex < (yFirst*ToFirstIMG.x()*3) && secondIMGindex < (ySecond*ToSecondIMG.x()*3);
            firstIMGindex+=3, secondIMGindex+=3)
        {
            middleR += ((double)firstIMGbits[firstIMGindex]-(double)secondIMGbits[secondIMGindex]);
            middleG += ((double)firstIMGbits[firstIMGindex+1]-(double)secondIMGbits[secondIMGindex+1]);
            middleB += ((double)firstIMGbits[firstIMGindex+2]-(double)secondIMGbits[secondIMGindex+2]);
        }
    middleR /= ((ToFirstIMG.x()-FromFirstIMG.x())*(ToFirstIMG.y()-FromFirstIMG.y()));
    middleG /= ((ToFirstIMG.x()-FromFirstIMG.x())*(ToFirstIMG.y()-FromFirstIMG.y()));
    middleB /= ((ToFirstIMG.x()-FromFirstIMG.x())*(ToFirstIMG.y()-FromFirstIMG.y()));

    emit ValuesCalculated(middleR, middleG, middleB);
}

void Analysis_of_a_manual_stitching::ValueChange(QPoint delta)
{
    m_mutex.lock();
    if(FromFirstIMG.x() != 1 && FromFirstIMG.x() != firstIMG->width())
        FromFirstIMG.setX(FromFirstIMG.x()-delta.x());
    if(FromFirstIMG.y() != 1 && FromFirstIMG.y() != firstIMG->height())
        FromFirstIMG.setX(FromFirstIMG.y()-delta.y());
    if(ToFirstIMG.x() != 1 && ToFirstIMG.x() != firstIMG->width())
        ToFirstIMG.setX(ToFirstIMG.x()-delta.x());
    if(ToFirstIMG.y() != 1 && ToFirstIMG.y() != firstIMG->height())
        ToFirstIMG.setY(ToFirstIMG.y()-delta.y());
    if(FromSecondIMG.x() != 1 && FromSecondIMG.x() != secondIMG->width())
        FromSecondIMG.setX(FromSecondIMG.x()-delta.x());
    if(FromSecondIMG.y() != 1 && FromSecondIMG.y() != secondIMG->height())
        FromSecondIMG.setY(FromSecondIMG.y()-delta.y());
    if(ToSecondIMG.x() != 1 && ToSecondIMG.x() != secondIMG->width())
        ToSecondIMG.setX(ToSecondIMG.x()-delta.x());
    if(ToSecondIMG.y() != 1 && ToSecondIMG.y() != secondIMG->height())
        ToSecondIMG.setY(ToSecondIMG.y()-delta.y());
    Calculate();
    m_mutex.unlock();
}
