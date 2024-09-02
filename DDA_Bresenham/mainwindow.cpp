#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QLCDNumber>

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
void MainWindow::colorPoint(int x, int y, int r, int g, int b, int gridOffset) {

    QPixmap canvas = ui->graph->pixmap();
    QPainter painter(&canvas);
    QPen pen= QPen(QColor(r,g,b),gridOffset);
    painter.setPen(pen);
    painter.drawRect(x, y, gridOffset,0);
    ui->graph->setPixmap(canvas);
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

    // Ensure the grid offset is valid
    if (gridOffset <= 0) {
        return; // Invalid grid offset, do nothing
    }

    // Create a QImage for drawing the grid
    QImage gridImage(width, height, QImage::Format_ARGB32);
    gridImage.fill(Qt::white); // Set background color of the grid

    // Create a QPainter to draw on the QImage
    QPainter painter(&gridImage);
    painter.setPen(QPen(Qt::gray, 1)); // Set the grid line color and thickness

    // Draw vertical grid lines
    for (int x = 0; x < width; x += gridOffset) {
        painter.drawLine(x, 0, x, height);
    }

    // Draw horizontal grid lines
    for (int y = 0; y < height; y += gridOffset) {
        painter.drawLine(0, y, width, y);
    }

    // Convert the QImage to a QPixmap and set it on the graph
    ui->graph->setPixmap(QPixmap::fromImage(gridImage));
}


void MainWindow::on_axis_clicked()
{
    int x = ui->graph->width();
    int y = ui->graph->height();
    int height =y;
    int width=x;

    // Get the grid offset, defaulting to 1 if it's 0
    int gridOffset = (ui->gridoffset->value() == 0) ? 1 : ui->gridoffset->value();
    x=x/2;
    y=y/2;
    // Calculate the top-left corner of the nearest grid cell
    int gridX = (x / gridOffset) * gridOffset;
    int gridY = (y / gridOffset) * gridOffset;
    gridY+=gridOffset/2;
    // Color the grid cell
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

        // Get the grid offset, defaulting to 1 if it's 0
        int gridOffset = (ui->gridoffset->value() == 0) ? 1 : ui->gridoffset->value();

        // Calculate the top-left corner of the nearest grid cell
        int gridX = (x / gridOffset) * gridOffset;
        int gridY = (y / gridOffset) * gridOffset;
        gridY+=gridOffset/2;
        ui->x->setText(QString::number(gridX/gridOffset +1));
        ui->y->setText(QString::number(gridY/gridOffset ));

        // Color the grid cell
        colorPoint(gridX, gridY, 50, 100, 25, gridOffset);
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_firstpoint_clicked()
{
    ui->px1->setText(ui->x->toPlainText());
    ui->py1->setText(ui->y->toPlainText());
}


void MainWindow::on_secondpoint_clicked()
{
    ui->px2->setText(ui->x->toPlainText());
    ui->py2->setText(ui->y->toPlainText());
}


void MainWindow::on_dda_clicked()
{
    int x1 = ui->px1->toPlainText().toInt();
    int y1 = ui->py1->toPlainText().toInt();
    int x2 = ui->px2->toPlainText().toInt();
    int y2 = ui->py2->toPlainText().toInt();
    int gridOffset = ui->gridoffset->value();

    // Ensure valid points
    if (x1 == x2 && y1 == y2) return;

    // Retrieve the pixmap and ensure it's not null
    QPixmap pixmap = ui->graph->pixmap() ;
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);  // No border for the rectangles
    painter.setBrush(QColor(25, 50, 120));

    QElapsedTimer timer;
    timer.start();
    // Calculate differences
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Determine the number of steps required
    int steps = std::max(abs(dx), abs(dy));

    // Calculate the increment in x and y per step
    float xIncrement = static_cast<float>(dx) / steps;
    float yIncrement = static_cast<float>(dy) / steps;

    // Initialize the starting point
    float x = x1;
    float y = y1;

    // Draw the line using the DDA algorithm
    for (int i = 0; i <= steps; ++i) {
        // Calculate the pixel position considering grid offset
        int gridX = static_cast<int>(round(x));
        int gridY = static_cast<int>(round(y));

        // Draw a rectangle to cover the grid cell
        painter.drawRect((gridX-1) * gridOffset, (gridY) * gridOffset, gridOffset, gridOffset);

        // Increment x and y
        x += xIncrement;
        y += yIncrement;
    }
    ui->graph->setPixmap(pixmap);
    qint64 elapsedTime = timer.elapsed(); // Time in milliseconds

    // Display the elapsed time on QLCDNumber
    ui->ddatime->display(QString::number(elapsedTime) );
}


void MainWindow::on_bresenham_clicked()
{
    int x1 = ui->px1->toPlainText().toInt();
    int y1 = ui->py1->toPlainText().toInt();
    int x2 = ui->px2->toPlainText().toInt();
    int y2 = ui->py2->toPlainText().toInt();
    int go = ui->gridoffset->value();
    // Ensure valid points
    if (x1 == x2 && y1 == y2) return;

    int x0 = x1;
    int y0 = y1;

    // Calculate deltas and steps
    int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy;
    int e2;

    QPixmap canvas = ui->graph->pixmap();
    QPainter painter(&canvas);
    QPen pen= QPen(QColor(40,100,25),go);
    painter.setPen(pen);

    QElapsedTimer timer;
    timer.start();

    // Bresenham's line algorithm
    while (true) {
        // Draw the pixel at (x0, y0)
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
    qint64 elapsedTime = timer.elapsed(); // Time in milliseconds

    // Display the elapsed time on QLCDNumber
    ui->graph->setPixmap(canvas);
    ui->bresentime->display(QString::number(elapsedTime));
}

