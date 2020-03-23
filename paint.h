#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QPointF>
#include <QString>
#include <QPushButton>
#include <cmath>
#include <paintscene.h>
#include <iostream>

namespace Ui {
class Paint;
}

class Paint : public QWidget
{
    Q_OBJECT

public:
    QPointF A, P;
    double AreaP, PerimeterP, distant;
    bool ConvexP = false;
    explicit Paint(QWidget *parent = 0);
    ~Paint();

    bool Convex(QPointF *points, int n);
    double Area(QPointF * points, int n);
    double Perimeter(QPointF * points, int n);
    QPointF NN(QPointF * POINTS, int n);
    void qsortP (QPointF *points, int n);

private:
    Ui::Paint *ui;
    QTimer *timer;      /* Определяем таймер для подготовки актуальных размеров
                         * графической сцены */
    paintScene *scene;  // Объявляем кастомную графическую сцену

private:
    /* Переопределяем событие изменения размера окна
     * для пересчёта размеров графической сцены */
    void resizeEvent(QResizeEvent * event);

private slots:
    void slotTimer();
public slots:
    void OK();
};



#endif // PAINT_H
