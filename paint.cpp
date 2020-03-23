#include "paint.h"
#include "ui_paint.h"


Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);

    scene = new paintScene();           // Инициализируем графическую сцену
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену
    scene->count = 0;
    timer = new QTimer();               // Инициализируем таймер
    connect(timer, &QTimer::timeout, this, &Paint::slotTimer);
    timer->start(100);                  // Запускаем таймер
    scene->points = new QPointF[100];
    scene->points[0] = scene->previousPoint;
    ui->pushButton->setMouseTracking(true);
    ui->pushButton->setVisible(true);
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(OK()));
};

Paint::~Paint()
{
    delete ui;
};

void Paint::slotTimer()
{
    /* Переопределяем размеры графической сцены в зависимости
     * от размеров окна*/
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
};

void Paint::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QWidget::resizeEvent(event);
}

void Paint::OK()
{
    //отрисовка по массиву поинтс самого многоугольника и вычисления
    int i = 0;
    QTextStream Qcout(stdout);
    QPen pen(QBrush(Qt::black), 4);
    QString boolstr("Convex");
    A = scene->previousPoint;
    for (i = 0; i < scene->count - 2; i++) {
        scene->addLine(scene->points[i].x(),scene->points[i].y(),scene->points[i+1].x(),scene->points[i+1].y(),pen);
    }
    scene->addLine(scene->points[i].x(),scene->points[i].y(),scene->points[0].x(),scene->points[0].y(),pen);

    ConvexP = Convex(scene->points, scene->count);
    if (!ConvexP)
       boolstr = "Concave";
    Qcout << boolstr <<endl;
    AreaP = Area(scene->points, scene->count);
    Qcout << "Area = " << AreaP << endl;
    PerimeterP = Perimeter(scene->points,scene->count);
    Qcout << "perimeter = " << PerimeterP << endl;
    P = NN(scene->points, scene->count);
    scene->addEllipse(P.x() - 2, P.y() - 2, 8, 8, QPen(Qt::NoPen), QBrush(Qt::green));
};

QPointF Paint::NN(QPointF * POINTS, int n)
{
    QTextStream Qcout(stdout);
    int i;
    double t, x, y;
    QPointF temp;

    double dist = (pow(POINTS[0].x() - A.x(),2)+pow(POINTS[0].y() - A.y(),2)), disttmp;
    Qcout << "point A = (" << A.x() << "," << A.y()<<")" << endl;

    for (i = 0; i <= n - 2; i++)
    {
        t = ((POINTS[i]).x() - (POINTS[(i+1) % (n-1)]).x())*((POINTS[i]).x() - A.x());
        t += ((POINTS[i]).y() - POINTS[(i+1) % (n-1)].y())*(POINTS[i].y() - A.y());
        t /= (pow((POINTS[i].x() - POINTS[(i+1) % (n-1)].x()),2) + pow((POINTS[i].y() - POINTS[(i+1) % (n-1)].y()),2));
        if (t > 1) {
            disttmp = pow(POINTS[(i+1) % (n-1)].x() - A.x(),2)+pow(POINTS[(i+1) % (n-1)].y() - A.y(),2);
            if (disttmp < dist) {
                    dist = disttmp;
                    temp = POINTS[(i+1) % (n-1)];
             }
        }
        else if (t < 0) {
            disttmp = pow(POINTS[i].x() - A.x(),2)+pow(POINTS[i].y() - A.y(),2);
            if (disttmp < dist) {
                    dist = disttmp;
                    temp = POINTS[i];
             }
        }
        else {
            x = POINTS[i].x() + t*(POINTS[(i+1) % (n-1)].x() - POINTS[i].x());
            y = POINTS[i].y() + t*(POINTS[(i+1) % (n-1)].y() - POINTS[i].y());
            disttmp = pow(x-A.x(),2)+ pow(y - A.y(),2);
            if (disttmp < dist) {
                dist = disttmp;
                temp = QPointF(x, y);
            }
        }

    };
    Qcout<< "Projection A on polygion is P = (" << temp.x() << "," << temp.y() << ")" << endl;
    return temp;
};

void Paint::qsortP (QPointF * points, int n)
{
    QPointF tmp, p;
    int i = 0, j = n - 1;

    p = points[n>>1];
    do {
        while (points[i].x() < p.x()) i++;
        while (points[j].x() > p.x()) j--;

        if (i <= j) {
          tmp = points[i];
          points[i] = points[j];
          points[j] = tmp;
          i++;
          j--;
        }
    } while (i <= j);
    if ( j > 0 ) qsortP(points, j + 1);
    if ( n > i ) qsortP(points + i, n-i);
};

bool Paint::Convex(QPointF *points, int n)
{
   int i,j,k;
   int flag = 0;
   double z;

   if (n < 3)
      return true;

   for (i = 0; i < n - 2; i++) {
      j = (i + 1) % n;
      k = (i + 2) % n;
      z  = (points[j].x() - points[i].x()) * (points[k].y() - points[j].y());
      z -= (points[j].y() - points[i].y()) * (points[k].x() - points[j].x());
      if (z < 0)
         flag |= 1;
      else if (z > 0)
         flag |= 2;
      if (flag == 3)
         return false;
   }
   if (flag != 0)
      return true;
   else
      return false;
};

double Paint::Area(QPointF * points, int n)
{
    int i;
    double S1 = 0, S2 = 0;
    for (i = 0; i < n - 1; i++){
        S1 += points[i].x() * points[i+1].y();
        S2 += points[i+1].x() * points[i].y();
    }
    S1 -= S2;
    S1 += points[n-1].x()*points[0].y();
    S1 -= points[0].x() * points[n-1].y();
    return S1;
};

double Paint::Perimeter(QPointF * points, int n)
{
    double p = 0;
    int i;
    for (i = 0; i < n - 1; ++i) {
        p += sqrt((points[i].x() - points[i+1].x())*(points[i].x() - points[i+1].x()) + (points[i].y() - points[i+1].y())*(points[i].y() - points[i+1].y()));
    }

    p += sqrt((points[0].x() - points[n-1].x())*(points[0].x() - points[n-1].x()) + (points[0].y() - points[n-1].y())*(points[0].y() - points[n-1].y()));
    return p;
};

