#ifndef ANALYSIS_OF_A_MANUAL_STITCHING_H
#define ANALYSIS_OF_A_MANUAL_STITCHING_H

#include <QObject>
#include <QImage>
#include <QPoint>
#include <QString>
#include <QMutex>

class Analysis_of_a_manual_stitching : public QObject
{
    Q_OBJECT
public:
    explicit Analysis_of_a_manual_stitching(QString firstIMGfileName, QString secondIMGfileName,
                                            QPoint fromP1, QPoint toP1,
                                            QPoint fromP2, QPoint toP2/*,QObject *parent = nullptr*/);

private:
    QPoint FromFirstIMG;
    QPoint ToFirstIMG;
    QPoint FromSecondIMG;
    QPoint ToSecondIMG;
    QImage *firstIMG = nullptr;
    QImage *secondIMG = nullptr;
    uchar *firstIMGbits = nullptr;
    uchar *secondIMGbits = nullptr;
    QMutex m_mutex;

    void Calculate();

signals:
    void ValuesCalculated(double R, double G, double B);

public slots:
    void ValueChange(QPoint delta);
};

#endif // ANALYSIS_OF_A_MANUAL_STITCHING_H
