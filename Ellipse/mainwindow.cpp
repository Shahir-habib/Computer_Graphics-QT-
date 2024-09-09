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
    gridImage.fill(Qt::yellow);

    QPainter painter(&gridImage);
    painter.setPen(QPen(Qt::green, 1));

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


void MainWindow::on_polarellipse_clicked()
{
    int xr = ui->a->value();
    int yr = ui->b->value();
    int xc = ui->x->toPlainText().toInt();
    int yc = ui->y->toPlainText().toInt();
    int go = ui->gridoffset->value();
    double dth = 1.0 / ((xr + yr) / 2.0);
    double th = 0;
    const double pi = 3.1416;

    QPixmap pixmap = ui->graph->pixmap() ;
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(25, 50, 120));

    QElapsedTimer timer;
    timer.start();
    while (th <= 2 * pi) {
        int x = static_cast<int>(xr * std::cos(th));
        int y = static_cast<int>(yr * std::sin(th));

        // Plot the points
        painter.drawRect((xc + x)*go, (yc + y)*go,go,go);
        painter.drawRect((xc - x)*go, (yc + y)*go,go,go);
        painter.drawRect((xc + x)*go, (yc - y)*go,go,go);
        painter.drawRect((xc - x)*go, (yc - y)*go,go,go);

        th += dth;
    }
    ui->graph->setPixmap(pixmap);
    qint64 elapsedTime = timer.nsecsElapsed()/1000;
    ui->ptime->display(QString::number(elapsedTime) );
}




void MainWindow::on_bresenhamellipse_clicked()
{
    int xr = ui->a->value();
    int yr = ui->b->value();
    int xc = ui->x->toPlainText().toInt();
    int yc = ui->y->toPlainText().toInt();
    int go = ui->gridoffset->value();
    int x = 0;
    int y = yr;
    int d1 = (yr * yr) - (xr * xr * yr) + (0.25 * xr * xr);
    int dx = 2 * yr * yr * x;
    int dy = 2 * xr * xr * y;

    QPixmap pixmap = ui->graph->pixmap() ;
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(25, 50, 120));

    QElapsedTimer timer;
    timer.start();

    painter.drawRect((xc + x)*go, (yc + y)*go,go,go);
    painter.drawRect((xc - x)*go, (yc + y)*go,go,go);
    painter.drawRect((xc + x)*go, (yc - y)*go,go,go);
    painter.drawRect((xc - x)*go, (yc - y)*go,go,go);

    // Region 1
    while (dx < dy) {
        x++;
        dx += 2 * yr * yr;
        if (d1 < 0) {
            d1 += dx + (yr * yr);
        } else {
            y--;
            dy -= 2 * xr * xr;
            d1 += dx - dy + (yr * yr);
        }
        painter.drawRect((xc + x)*go, (yc + y)*go,go,go);
        painter.drawRect((xc - x)*go, (yc + y)*go,go,go);
        painter.drawRect((xc + x)*go, (yc - y)*go,go,go);
        painter.drawRect((xc - x)*go, (yc - y)*go,go,go);
    }

    // Region 2
    int d2 = (yr * yr) * ((x + 0.5) * (x + 0.5)) + (xr * xr) * ((y - 1) * (y - 1)) - (xr * xr * yr * yr);
    while (y >= 0) {
        y--;
        dy -= 2 * xr * xr;
        if (d2 > 0) {
            d2 += (xr * xr) - dy;
        } else {
            x++;
            dx += 2 * yr * yr;
            d2 += dx - dy + (xr * xr);
        }
        painter.drawRect((xc + x)*go, (yc + y)*go,go,go);
        painter.drawRect((xc - x)*go, (yc + y)*go,go,go);
        painter.drawRect((xc + x)*go, (yc - y)*go,go,go);
        painter.drawRect((xc - x)*go, (yc - y)*go,go,go);
    }
    ui->graph->setPixmap(pixmap);
    qint64 elapsedTime = timer.nsecsElapsed()/1000;
    ui->btime->display(QString::number(elapsedTime));
}

