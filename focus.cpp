#include "focus.h"

using namespace std;

QString focus_img::start()
{
    for(QString d: dirs)
    {
        QImage orgImg;
        if(orgImg.load(d+"/"+fileName))
        {
            vector< vector< double> > RedIntegralImg(orgImg.width(), vector< double> (orgImg.height(), 0));
            vector< vector< double> > GreenIntegralImg(orgImg.width(), vector< double> (orgImg.height(), 0));
            vector< vector< double> > BlueIntegralImg(orgImg.width(), vector< double> (orgImg.height(), 0));
            QColor pixRGB;
            pixRGB= orgImg.pixelColor(X1,Y1);
            RedIntegralImg.at(X1).at(Y1)=pixRGB.red();
            GreenIntegralImg.at(X1).at(Y1)=pixRGB.green();
            BlueIntegralImg.at(X1).at(Y1)=pixRGB.blue();
            for(int x=1; x<orgImg.width(); x++)
            {
                pixRGB= orgImg.pixelColor(x,0);
                RedIntegralImg.at(x).at(0)=pixRGB.red()+RedIntegralImg.at(x-1).at(0);
                GreenIntegralImg.at(x).at(0)=pixRGB.green()+GreenIntegralImg.at(x-1).at(0);
                BlueIntegralImg.at(x).at(0)=pixRGB.blue()+BlueIntegralImg.at(x-1).at(0);
            }
            for(int y=1; y<orgImg.width(); y++)
            {
                pixRGB= orgImg.pixelColor(0,y);
                RedIntegralImg.at(0).at(y)=pixRGB.red()+RedIntegralImg.at(0).at(y-1);
                GreenIntegralImg.at(0).at(y)=pixRGB.green()+GreenIntegralImg.at(0).at(y-1);
                BlueIntegralImg.at(0).at(y)=pixRGB.blue()+BlueIntegralImg.at(0).at(y-1);
            }
            for(int x=2; x<orgImg.width(); x++)
                for(int y=2; y<orgImg.height(); y++)
                {
                    pixRGB= orgImg.pixelColor(x,y);
                    RedIntegralImg.at(x).at(y)=pixRGB.red()+RedIntegralImg.at(x).at(y-1)+
                            RedIntegralImg.at(x-1).at(y)-RedIntegralImg.at(x-1).at(y-1);
                    GreenIntegralImg.at(x).at(y)=pixRGB.green()+GreenIntegralImg.at(x).at(y-1)+
                            GreenIntegralImg.at(x-1).at(y)-GreenIntegralImg.at(x-1).at(y-1);
                    BlueIntegralImg.at(x).at(y)=pixRGB.blue()+BlueIntegralImg.at(x).at(y-1)+
                            BlueIntegralImg.at(x-1).at(y)-BlueIntegralImg.at(x-1).at(y-1);
                }
            QImage FImg, SImg;
            FImg= orgImg.copy(0,0,orgImg.width(),orgImg.height());
            SImg= orgImg.copy(0,0,orgImg.width(),orgImg.height());
            for(int x=X1+4; x<X2-4; x++)
                for(int y=Y1+4; y<Y2-4; y++)
                {
                    pixRGB.setRed((RedIntegralImg.at(x-4).at(y-4)+RedIntegralImg.at(x+4).at(y+4)-
                                   RedIntegralImg.at(x+4).at(y-4)-RedIntegralImg.at(x-4).at(y+4))/81);
                    pixRGB.setGreen((GreenIntegralImg.at(x-4).at(y-4)+GreenIntegralImg.at(x+4).at(y+4)-
                                     GreenIntegralImg.at(x+4).at(y-4)-GreenIntegralImg.at(x-4).at(y+4))/81);
                    pixRGB.setBlue((BlueIntegralImg.at(x-4).at(y-4)+BlueIntegralImg.at(x+4).at(y+4)-
                                    BlueIntegralImg.at(x+4).at(y-4)-BlueIntegralImg.at(x-4).at(y+4))/81);
                    FImg.setPixelColor(x,y,pixRGB);
                }
            for(int x=7; x<orgImg.width()-7; x++)
                for(int y=7; y<orgImg.height()-7; y++)
                {
                    pixRGB.setRed((RedIntegralImg.at(x-7).at(y-7)+RedIntegralImg.at(x+7).at(y+7)-
                                   RedIntegralImg.at(x+7).at(y-7)-RedIntegralImg.at(x-7).at(y+7))/225);
                    pixRGB.setGreen((GreenIntegralImg.at(x-7).at(y-7)+GreenIntegralImg.at(x+7).at(y+7)-
                                     GreenIntegralImg.at(x+7).at(y-7)-GreenIntegralImg.at(x-7).at(y+7))/225);
                    pixRGB.setBlue((BlueIntegralImg.at(x-7).at(y-7)+BlueIntegralImg.at(x+7).at(y+7)-
                                    BlueIntegralImg.at(x+7).at(y-7)-BlueIntegralImg.at(x-7).at(y+7))/225);
                    SImg.setPixelColor(x,y,pixRGB);
                }
            double K=0.0;
            for(int x=X1; x<X2; x++)
                for(int y=Y1; y<Y2; y++)
                {
                    pixRGB=FImg.pixelColor(x,y);
                    double B1 = 1.0*(pixRGB.red()+pixRGB.green()+pixRGB.blue())/3;
                    pixRGB=SImg.pixelColor(x,y);
                    double B2 = 1.0*(pixRGB.red()+pixRGB.green()+pixRGB.blue())/3;
                    K += (B1-B2);
                }
            K /= (orgImg.width()*orgImg.height());
            if(MaxFocusValue < K)
            {
                MaxFocusValue= K;
                MaxFocusDir= d;
            }
        }
    }
    return MaxFocusDir;
}
