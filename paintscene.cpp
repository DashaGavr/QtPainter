#include "paintscene.h"

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
}

paintScene::~paintScene()
{
}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    static int k = 0;
    // При нажатии кнопки мыши отрисовываем эллипс
    addEllipse(event->scenePos().x() - 2,
               event->scenePos().y() - 2,
               6,
               6,
               QPen(Qt::NoPen),
               QBrush(Qt::black));
    // Сохраняем координаты точки нажатия
    previousPoint = event->scenePos();
    points[k++] = previousPoint;
    count = k;
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Отрисовываем линии с использованием предыдущей координаты
    /*addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(Qt::red,10,Qt::SolidLine,Qt::RoundCap));*/
    // Обновляем данные о предыдущей координате
    //previousPoint = event->scenePos();
}
