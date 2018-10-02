#include "gradient_method_stitching.h"

gradient_method_stitching::gradient_method_stitching(QImage *FirstFileIMG, QImage *SecondFileIMG,
                                                     int MinH, int MaxH, int MinV, int MaxV):
    FirstIMG(FirstFileIMG), SecondImg(SecondFileIMG),
    MinimalHorizontal(MinH), MaximumHorizontal(MaxH), MinimumVertycal(MinV), MaximumVerycal(MaxV)
{
    FirstIMG->convertToFormat(QImage::Format_RGB32);
    SecondImg->convertToFormat(QImage::Format_RGB32);
}

void gradient_method_stitching::start()
{
    MeddianFilter(FirstIMG);
    MeddianFilter(SecondImg);
    FindObject(FirstIMG, FindObjectFirstIMG,
               QPoint(FirstIMG->width()-MaximumHorizontal, FirstIMG->height()-MaximumHorizontal),
               QPoint(FirstIMG->width()-MinimalHorizontal, FirstIMG->height()-MinimalHorizontal));
    FindObject(SecondImg, FindObjectSecondIMG,
               QPoint(MinimalHorizontal, MinimumVertycal),
               QPoint(MaximumHorizontal, MaximumVerycal));
    for(size_t i=0; i < FindObjectFirstIMG.size(); i++)
        CalculateGradient(FirstIMG, FindObjectFirstIMG.at(i));
    for(size_t i=0; i < FindObjectSecondIMG.size(); i++)
        CalculateGradient(SecondImg, FindObjectSecondIMG.at(i));
    for(size_t i = 0 ; i < FindObjectFirstIMG.size(); i++)
    {
        size_t k = 0;
        while(!(FindObjectFirstIMG.at(i) < FindObjectSecondIMG.at(k)) && k < FindObjectSecondIMG.size()-1)
            k++;
    }
    std::vector<Point> DefinedPoint;
    for(size_t i = 0; i < FindObjectFirstIMG.size(); i++)
        if(FindObjectFirstIMG.at(i).GetSameCenter() != QPoint(0, 0))
            DefinedPoint.push_back(FindObjectFirstIMG.at(i));
    FindObjectFirstIMG.swap(DefinedPoint);
}

std::vector<Point> &gradient_method_stitching::GetObject()
{
    return FindObjectFirstIMG;
}

void gradient_method_stitching::MeddianFilter(QImage *IMG) //меддианый фильтр
{
    std::vector<int> R;
    std::vector<int> G;
    std::vector<int> B;

    for(int y = 1; y < IMG->height()-1; y++)
        for(int x = 1; x < IMG->width()-1; x++)
        {
            QRgb rgb;
            for(int y0 = y-1; y0 < y+2; y0++)
                for(int x0 = x-1; x0 < x+2; x0++)
                {
                    rgb = IMG->pixel(x0, y0); //получение значений пикселей в области 3х3
                    R.push_back(qRed(rgb));
                    G.push_back(qGreen(rgb));
                    B.push_back(qBlue(rgb));
                }
            std::sort(R.begin(), R.end());
            std::sort(G.begin(), G.end());
            std::sort(B.begin(), B.end());
            rgb = qRgb(R.at(R.size()/2), G.at(G.size()/2), B.at(B.size()/2)); //значение по середине присваеваем текущему пикселю
            IMG->setPixel(x, y, rgb);
            R.clear();
            G.clear();
            B.clear();
        }

}

int gradient_method_stitching::DefinitionTresholdBinarization(QImage *IMG)
{
    std::vector<int> temp(256, 0);

    for(int x=0;x<IMG->width();++x)
        for(int y=0;y<IMG->height();++y)
        {
            QRgb rgb = IMG->pixel(x, y);
            temp.at((qGreen(rgb)+qRed(rgb)+qBlue(rgb))/3)++;// заполнение гистограммы
        }

    for(int i=0;i<254;i++)
    {
        temp.at(i)=temp.at(i+1)-temp.at(i);//из следующего вычитаем текущее
    }
    std::vector<int> max;
    for(size_t i = 40; i < 244; i++)
    {
        int count=0;
        for(int j=0;j<10;j++)//проверка в окне равным 10
            if(temp.at(i+j)>0)
                count++;
        if(temp.at(i)>0 && count>5)
        {
            max.push_back(i);
            i+=10;
        }
    }
    int k=0;
    for(int i: max)
        k+=i;
    k/=max.size(); // определение среднего значения локальных максимумов
    return k;
}

void gradient_method_stitching::FindObject(QImage *IMG, std::vector<Point> &Container, QPoint From, QPoint To)
{
    int Treshold = DefinitionTresholdBinarization(IMG);// определить порог бинаризации
    QImage *BinarizationIMG = new QImage(IMG->width(), IMG->height(), QImage::Format_RGB32);
    for(int x = 0; x < IMG->width(); ++x)
        for(int y = 0; y < IMG->height(); ++y)// бинаризация
        {
            QRgb rgb = IMG->pixel(x, y);
            if(((qGreen(rgb)+qRed(rgb)+qBlue(rgb))/3) < Treshold)
                BinarizationIMG->setPixel(x,y,qRgb(0,0,0));
            else
                BinarizationIMG->setPixel(x,y,qRgb(255,255,255));
        }
    for(int x = From.x(); x < To.x(); x++)
        for(int y = From.y(); y < To.y(); y++)
            if(BinarizationIMG->pixel(x, y) == qRgb(0, 0, 0))// если найден объект то рассчитать его значения
                Container.push_back(CalculateObject(BinarizationIMG, QPoint(x, y)));
}

Point gradient_method_stitching::CalculateObject(QImage *IMG, QPoint p)
{
    std::vector<QPoint> vector;
    vector.push_back(p);
    int xmin = p.x();//задаем начальные значения
    int xmax = p.x();
    int ymin = p.y();
    int ymax = p.y();
    int N = 0, CenterY = 0, CenterX = 0;
    while (!vector.empty())
    {
        QPoint p = vector.back();
        vector.pop_back();
        QRgb rgb = IMG->pixel(p);
        if((qRed(rgb) == 0) && (qGreen(rgb) == 0) && qBlue(rgb) == 0)//если пиксель черный, то
        {
            if(p.ry() > ymax)// проверка его координат на максимальное и минимальное значение координат объекта
                ymax = p.ry();
            if(p.rx() < xmin)
                xmin = p.rx();
            if(p.rx() > xmax)
                xmax = p.rx();
            IMG->setPixel(p, qRgb(255, 255, 255));
            for(int y0 = p.ry()-1; y0 < p.ry()+2; ++y0)
                for(int x0 = p.rx()-1; x0 < p.rx()+2; ++x0)
                {
                    rgb=IMG->pixel(x0, y0);
                    if((qRed(rgb) == 0) && (qGreen(rgb) == 0) && qBlue(rgb) == 0)//проверка соседних пиксель на то что они черные
                        vector.push_back(QPoint(x0, y0));
                }
            ++N;
            CenterX += p.x();
            CenterY += p.y();
        }
    }
    CenterX /= N;// получаем центр масс
    CenterY /= N;
    ymax = std::max(CenterY-ymin, ymax-CenterY);//определяем максимально расстояние от центра масс од края объекта
    xmax = std::max(CenterX-xmin, xmax-CenterX);

    return Point(QPoint(CenterX, CenterY), std::max(ymax, xmax));
}

void gradient_method_stitching::CalculateGradient(QImage *IMG, Point &p)
{
    std::vector<double> GradientVector;
    for(int x = p.GetCenterMass().x()-p.GetDeltaWindow(); x < p.GetCenterMass().x()+p.GetDeltaWindow(); x++)
        for(int y = p.GetCenterMass().y()-p.GetDeltaWindow(); y < p.GetCenterMass().y()+p.GetDeltaWindow(); y++)
        {
            int GradientH = 0, GradientV = 0;//получаем все значение градиента в окне
            for(size_t i = 0; i < 3; i++)
                for(size_t k = 0; k < 3; k++)
                {
                    QRgb rgb;
                    rgb = IMG->pixel(x-1+k,y-1+i);
                    GradientH += ((qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3)*Gx.at(i).at(k);
                    GradientV += ((qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3)*Gy.at(i).at(k);
                }
            GradientVector.push_back(std::sqrt(std::pow(GradientH, 2)+std::pow(GradientV, 2)));
        }
    std::vector<double> out(64, 0);
    std::partial_sort_copy(GradientVector.begin(), GradientVector.end(),//сохраняем 64 максимальных значения градиента
                           out.begin(), out.end(),
                           std::greater<double>());
    p.SetGradient(out);
}

