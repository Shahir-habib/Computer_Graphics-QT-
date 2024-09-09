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
    QPixmap canvas = ui->graph->pixmap();
    QPainter painter(&canvas);
    QPen pen= QPen(QColor(70,70,255),gridOffset);
    painter.setPen(pen);

    for(int i=0;i<height;i+=gridOffset){
        painter.drawRect(gridX, i, gridOffset,0);
    }
    for(int i=0;i<width;i+=gridOffset){
        painter.drawRect(i, gridY, gridOffset,0);
    }
    painter.drawRect(x, y, gridOffset,0);
    ui->graph->setPixmap(canvas);
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

        QPixmap canvas = ui->graph->pixmap();
        QPainter painter(&canvas);
        QPen pen= QPen(QColor(70,70,255),gridOffset);
        painter.setPen(pen);

        painter.drawRect(gridX, gridY,gridOffset,0);
        ui->graph->setPixmap(canvas);
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_scanlinefill_clicked()
{

}


void MainWindow::on_floodfill_clicked()
{

}


void MainWindow::on_boundaryfill_clicked()
{

}


void MainWindow::bresenhamLine(int x1,int y1,int x2,int y2,QPainter & painter)
{
    int go = ui->gridoffset->value();
    int x0 = x1;
    int y0 = y1;

    // Calculate deltas and steps
    int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy;
    int e2;


    while (true) {
        painter.drawRect((x0-1)*go,(y0+0.5)*go,go,0);

        if (x0 == x2 && y0 == y2) break;

        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }

}

void MainWindow::on_draw_clicked()
{
    int sides = ui->n->value();
    if (sides < 3) return;
    int go = ui->gridoffset->value();
    int height = ui->graph->height();
    std::vector<std::pair<int,int>> vertices;
    // int s = 2*sides;
    int xc=ui->x->toPlainText().toInt(),yc=ui->y->toPlainText().toInt();
    // double radius = s/(2*(sin(M_PI/sides)));
    double angleStep = 2 * M_PI / sides;

    double radius =  height/ (5.0 * go);
    double s = 2 * radius * std::sin(M_PI / sides);


    for (int i = 0; i < sides; ++i) {
        double angle = i * angleStep;
        int x = static_cast<int>(xc + radius * std::cos(angle));
        int y = static_cast<int>(yc + radius * std::sin(angle));
        vertices.push_back({x,y});
    }
    QPixmap canvas = ui->graph->pixmap();
    QPainter painter(&canvas);
    QPen pen= QPen(QColor(40,100,25),go);
    painter.setPen(pen);


    // Draw edges
    for (int i = 0; i < sides; ++i) {
        int x1 = vertices[i].first, y1 = vertices[i].second;
        int x2 = vertices[(i + 1) % sides].first,y2 = vertices[(i + 1) % sides].second; // Wrap around to the first vertex
        bresenhamLine(x1,y1,x2,y2, painter);
    }
    ui->graph->setPixmap(canvas);
}

