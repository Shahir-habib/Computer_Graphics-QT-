#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QLCDNumber>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graph->setMouseTracking(true);
    ui->graph->installEventFilter(this);

    QPixmap canvas = ui->graph->pixmap(Qt::ReturnByValue);
    if (canvas.isNull()) {
        canvas = QPixmap(ui->graph->size());
        canvas.fill(Qt::white);
        ui->graph->setPixmap(canvas);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::colorPoint(int x, int y, int r, int g, int b, int gridOffset) {

    QPixmap canvas = ui->graph->pixmap();
    QPainter painter(&canvas);
    QPen pen= QPen(QColor(r,g,b),gridOffset);
    painter.setPen(pen);
    painter.drawRect(x, y, gridOffset,0);
    ui->graph->setPixmap(canvas);
}

void MainWindow::on_gridlines_clicked()
{
    int gridOffset = ui->gridoffset->value();
    int width = ui->graph->width();
    int height = ui->graph->height();

    if (gridOffset <= 0) {
        return;
    }

    QImage gridImage(width, height, QImage::Format_ARGB32);
    gridImage.fill(Qt::gray);

    QPainter painter(&gridImage);
    painter.setPen(QPen(Qt::blue, 1));

    for (int x = 0; x < width; x += gridOffset) {
        painter.drawLine(x, 0, x, height);
    }

    for (int y = 0; y < height; y += gridOffset) {
        painter.drawLine(0, y, width, y);
    }

    ui->graph->setPixmap(QPixmap::fromImage(gridImage));
}


void MainWindow::on_axis_clicked()
{
    int x = ui->graph->width();
    int y = ui->graph->height();
    int height =y;
    int width=x;

    int gridOffset = (ui->gridoffset->value() == 0) ? 1 : ui->gridoffset->value();
    x=x/2;
    y=y/2;
    int gridX = (x / gridOffset) * gridOffset;
    int gridY = (y / gridOffset) * gridOffset;
    gridY+=gridOffset/2;

    for(int i=0;i<height;i+=gridOffset){
        colorPoint(gridX, i, 250, 0, 0, gridOffset);
    }
    for(int i=0;i<width;i+=gridOffset){
        colorPoint(i, gridY, 250, 0, 0, gridOffset);
    }

}
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graph && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *cursor = static_cast<QMouseEvent*>(event);
        int x = cursor->pos().x();
        int y = cursor->pos().y();

        int gridOffset = (ui->gridoffset->value() == 0) ? 1 : ui->gridoffset->value();

        int gridX = (x / gridOffset) * gridOffset;
        int gridY = (y / gridOffset) * gridOffset;
        gridY+=gridOffset/2;
        ui->x->setText(QString::number(gridX/gridOffset +1));
        ui->y->setText(QString::number(gridY/gridOffset ));

        colorPoint(gridX, gridY, 50, 100, 25, gridOffset);
    }
    return QMainWindow::eventFilter(watched, event);
}



void MainWindow::on_polarcircle_clicked()
{
    int centerX = ui->x->toPlainText().toInt();
    int centerY = ui->y->toPlainText().toInt();
    int radius = ui->Radius->value();
    int go = ui->gridoffset->value();
    QPixmap pixmap = ui->graph->pixmap() ;
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(25, 50, 120));

    QElapsedTimer timer;
    timer.start();
    const int numPoints = 100;
    float angle;
    float xPos, yPos;

    for (int i = 0; i < numPoints; ++i) {
        angle = i * 2 * M_PI / numPoints;
        xPos = centerX + radius * cos(angle);
        yPos = centerY + radius * sin(angle);
        int gridX = static_cast<int>(round(xPos));
        int gridY = static_cast<int>(round(yPos));
        painter.drawRect((gridX-1) * go, (gridY) * go, go, go);
    }
    ui->graph->setPixmap(pixmap);
    qint64 elapsedTime = timer.nsecsElapsed()/1000;
    ui->polartime->display(QString::number(elapsedTime) );
}


void MainWindow::on_BresenhamCircle_clicked()
{
    int centerX = ui->x->toPlainText().toInt();
    int centerY = ui->y->toPlainText().toInt();
    int radius = ui->Radius->value();
    int go = ui->gridoffset->value();
    int x = 0;
    int y = radius;
    float p = 3 - 2*radius;
    QPixmap canvas = ui->graph->pixmap();
    QPainter painter(&canvas);
    QPen pen= QPen(QColor(100,125,41),go);
    painter.setPen(pen);

    QElapsedTimer timer;
    timer.start();
    while (x <= y) {

        painter.drawRect((centerX + x-1)*go, (centerY + y+0.5)*go,  go,0);
        painter.drawRect((centerX - x-1)*go, (centerY + y+0.5)*go,  go,0);
        painter.drawRect((centerX + x-1)*go, (centerY - y+0.5)*go,  go,0);
        painter.drawRect((centerX - x-1)*go, (centerY - y+0.5)*go,  go,0);
        painter.drawRect((centerX + y-1)*go, (centerY + x+0.5)*go,  go,0);
        painter.drawRect((centerX - y-1)*go, (centerY + x+0.5)*go,  go,0);
        painter.drawRect((centerX + y-1)*go, (centerY - x+0.5)*go,  go,0);
        painter.drawRect((centerX - y-1)*go, (centerY - x+0.5)*go,  go,0);
        if(p<0){
            x++;
            p = p+ 4*x +6;
        }
        else{
            y--;x++;
            p = p+4*(x-y) +10;
        }
    }
    qint64 elapsedTime = timer.nsecsElapsed()/1000;
    ui->Bcrtime->display(QString::number(elapsedTime) );
    ui->graph->setPixmap(canvas);
}


