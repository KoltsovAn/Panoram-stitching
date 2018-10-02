#include "mygraphicsscene.h"

myGraphicsScene::myGraphicsScene() : QGraphicsScene()
{

}

int myGraphicsScene::getX()
{
    return(this->firstpoint.x());
}

int myGraphicsScene::getY()
{
    return(this->firstpoint.y());
}

void myGraphicsScene::InitializationAllDescription(std::vector<std::vector<fileDescription> > &AD,
                                                   std::vector<QString> &Ls)
{
    this->clear();
    itemInScene.clear();
    AllDescription = AD;
    Layers = Ls;
    CurentLayer = Layers.at(0);
    size_t k=4, m=4;
    if(AllDescription.at(0).size()<k)
        k=AllDescription.size();
    if(AllDescription.size()<m)
        m=AllDescription.size();
    for(size_t y = 0; y < k; y++)
        for(size_t x = 0; x < m; x++)
            ViewNewItem(x, y);
    SizeOfFrame = QPixmap(CurentLayer+AllDescription.at(0).at(0).getFileName()).size();
    this->setSceneRect(0 , 0,
    AllDescription.at(AllDescription.size()-1).at(AllDescription.at(AllDescription.size()-1).size()-1).getX()
                       + SizeOfFrame.width(),
    AllDescription.at(AllDescription.size()-1).at(AllDescription.at(AllDescription.size()-1).size()-1).getY()
            + SizeOfFrame.height());
    //    qDebug() << "инициализация завершенеа";
}

QString myGraphicsScene::GetCurentDir()
{
    return CurentLayer;
}

void myGraphicsScene::SavePositionOriginalFrame(QTextStream &stream)
{
    for(size_t x = 0; x < AllDescription.size(); x++)
        for(size_t y = 0; y<AllDescription.at(AllDescription.at(x).size()-1).size(); y++)
            stream << AllDescription.at(x).at(y).getFileName() << '\t' << " x="
                   << AllDescription.at(x).at(y).getX() << '\t' <<" y="
                   << AllDescription.at(x).at(y).getY() << endl;
}

void myGraphicsScene::UpdatePositionAllDescription(TypeOfMouseEvent event)
{
    size_t xFrom = firstpoint.x()/SizeOfFrame.width();
    size_t yFrom = firstpoint.y()/SizeOfFrame.height();
    qDebug()<< firstpoint <<secondPoint<< xFrom<<";"<<yFrom;
//    if(yFrom < AllDescription.size()-1)
//        while (yFrom < AllDescription.size()-1 && firstpoint.y() > AllDescription.at(yFrom+1).at(0).getY())
//            yFrom++;
//    if(xFrom < AllDescription.at(yFrom).size()-1)
//        while (xFrom < AllDescription.at(yFrom).size()-1 && firstpoint.x() > AllDescription.at(yFrom).at(xFrom+1).getX())
//            xFrom++;
    firstpoint.setX(secondPoint.x()-firstpoint.x());
    firstpoint.setY(secondPoint.y()-firstpoint.y());
    switch (event) {
    case CtrlEvent:
        for (size_t y = yFrom; y < AllDescription.size(); y++)
            for(size_t x = 0; x < AllDescription.at(y).size(); x++)
                AllDescription.at(y).at(x).UpdatePosition(firstpoint);
        qDebug() << "Ctrl update: "<< xFrom<<";"<<yFrom;
        break;
    case ShiftEvent:
        for(size_t x = xFrom; x < AllDescription.at(yFrom).size(); x++)
            AllDescription.at(yFrom).at(x).UpdatePosition(firstpoint);
        qDebug() << "Shift update: "<< xFrom<<";"<<yFrom;
        break;
    case OneItemMoveEvent:
        AllDescription.at(yFrom).at(xFrom).UpdatePosition(firstpoint);
        qDebug() << "One Item update: "<< xFrom<<";"<<yFrom;
        break;
    default:
        break;
    }
}

QString myGraphicsScene::getName(int x, int y)
{
    return(this->AllDescription[y][x].getFileName());
}

void myGraphicsScene::LayerChange(int index)
{
    qDebug()<< "я пытаюсь обновить слой";
    if(index < 0 || index > Layers.size()-1)
        return;
    CurentLayer = Layers.at(index);
    for(itemInfo &II: itemInScene)
    {
        QPointF pos = II.getItem()->pos();
        qDebug()<< "сохранил координаты";
        this->removeItem(II.getItem());
        delete II.getItem();
        qDebug()<< "удалил старый объект";
        QGraphicsPixmapItem *item =
                new QGraphicsPixmapItem(CurentLayer+II.getFileName());
        item->setPos(pos);
        qDebug()<< "добавил новый и установил старые коррдинаты";
        II.setItem(item);
        this->addItem(item);
        qDebug()<< "добавил на сцену";
    }
}

int myGraphicsScene::getPosX(int x, int y)
{
    return(this->AllDescription[y][x].getX());
}

int myGraphicsScene::getPosY(int x, int y)
{
    return(this->AllDescription[y][x].getY());
}

int myGraphicsScene::verticalSize()
{
    return(AllDescription.size());
}

void myGraphicsScene::ViewZoneChange(QPointF point1, QPointF point2)
{
//    qDebug()<<point1.x()<<";"<<point1.y();
//    qDebug()<<point2.x()<<";"<<point2.y();
    if(point1.x() < 0 || point1.y() < 0 || point2.x() < 0 || point2.y() <0)
        return;
    size_t xFrom, yFrom, xTo, yTo;
    xFrom = point1.x()/SizeOfFrame.width();
    yFrom = point1.y()/SizeOfFrame.height();
    xTo = point2.x()/SizeOfFrame.width();
    yTo = point2.y()/SizeOfFrame.height();
    if(xTo < AllDescription.size()-1)
        xTo++;
    if(xTo > AllDescription.size()-1)
        xTo = AllDescription.size()-1;
//    qDebug()<<xTo;
    if(yTo < AllDescription.at(xTo).size()-1)
        yTo++;
    if(yTo > AllDescription.at(xTo).size()-1)
        yTo = AllDescription.at(xTo).size()-1;
//    qDebug()<<"From:("<<xFrom<<";"<<yFrom<<"); To:("<<xTo<<";"<<yTo<<");";
    for (int i=0; i < 2; i++)
        for (size_t y = yFrom; y <= yTo; y++)
            for(size_t x = xFrom; x <= xTo; x++)
            {
                qApp->processEvents();
                ViewNewItem(x, y);
            }
}

void myGraphicsScene::ViewNewItem(size_t x, size_t y)
{
    auto FindItemIterator = std::find_if(itemInScene.begin(), itemInScene.end(),
                                      [&](itemInfo& II){return (II.getFileName() ==
                                                                        AllDescription.at(y).at(x).getFileName());});
    if (FindItemIterator != itemInScene.cend())
        return;
    if(itemInScene.size() == 64)
    {
        this->removeItem(itemInScene.front().getItem());
        delete itemInScene.front().getItem();
        itemInScene.pop_front();
    }

    QGraphicsPixmapItem *item;
    item = new QGraphicsPixmapItem(CurentLayer+AllDescription.at(y).at(x).getFileName());
    item->setPos(AllDescription.at(y).at(x).getX(), AllDescription.at(y).at(x).getY());
    this->addItem(item);
    itemInScene.push_back(itemInfo(AllDescription.at(y).at(x).getFileName(), item));
}

int myGraphicsScene::horizontalSize()
{
    return(AllDescription.at(0).size());
}

void myGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    firstpoint.setX(event->scenePos().x());
    firstpoint.setY(event->scenePos().y());

    emit mousePressed();
}

void myGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    secondPoint.setX(event->scenePos().x());
    secondPoint.setY(event->scenePos().y());
    emit mouseReleased();
}

void myGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
//    qDebug()<<"move Event";
    this->secondPoint.setX(event->scenePos().x());
    this->secondPoint.setY(event->scenePos().y());
    //emit move();
}

void myGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    emit wheel(event->delta());
}
