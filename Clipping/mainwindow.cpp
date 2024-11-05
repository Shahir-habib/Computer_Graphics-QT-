#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <QColor>
#include <QVector>
#include <QThread>
#define DELAY delay(10)
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
int centerX,centerY,go,wh,ww;
vector<pair<int,int>> vertices;
vector<pair<int,int>> cvertices;
QImage emptyimage;
// Defining region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

QVector<QColor> colorVector {
    QColor(255, 0, 0),    // Red
    QColor(0, 255, 0),    // Green
    QColor(0, 0, 255),    // Blue
    QColor(255, 255, 0),  // Yellow
    QColor(255, 0, 255),  // Magenta
    QColor(0, 255, 255),  // Cyan
    QColor(192, 192, 192),// Silver
    QColor(128, 0, 0),    // Maroon
    QColor(128, 128, 0),  // Olive
    QColor(0, 128, 128)   // Teal
};
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
        canvas.fill(QColor(20, 20, 20));
        ui->graph->setPixmap(canvas);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graph && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *cursor = static_cast<QMouseEvent*>(event);
        int x = cursor->pos().x();
        int y = cursor->pos().y();

        int gridOffset = ui->gridoffset->value();

        int gridX = (x / gridOffset) * gridOffset;
        int gridY = (y / gridOffset) * gridOffset;
        gridY+=gridOffset/2;
        // ui->x->setText(QString::number((gridX/gridOffset +1) - (centerX/gridOffset +1)));
        // ui->y->setText(QString::number((centerY/gridOffset) - (gridY/gridOffset )));
        //vertices.push_back({gridX/gridOffset +1,gridY/gridOffset});
        vertices.push_back({(gridX/gridOffset +1) - (centerX/gridOffset +1),(centerY/gridOffset) - (gridY/gridOffset )});
        QPixmap canvas = ui->graph->pixmap();
        QImage image = canvas.toImage();        // Convert it to QImage for pixel access
        QPainter painter(&image);
        QPen pen;QColor c;
        if(false){
            int r = rand() %10;
            pen= QPen(colorVector[r],gridOffset);
        }
        else{
            pen = QPen(Qt::green,gridOffset);
        }
        painter.setPen(pen);
        painter.drawRect(gridX, gridY,gridOffset,0);
        ui->graph->setPixmap(QPixmap::fromImage(image));
    }
    return QMainWindow::eventFilter(watched, event);
}
void MainWindow::on_gridlines_clicked()
{
    int gridOffset = ui->gridoffset->value();
    int width = ui->graph->width();
    int height = ui->graph->height();
    go = gridOffset;
    if (gridOffset <= 0) {
        return;
    }

    QImage gridImage(width, height, QImage::Format_ARGB32);
    gridImage.fill(QColor(20, 20, 20));

    QPainter painter(&gridImage);
    painter.setPen(QPen(Qt::gray, 1));

    for (int x = 0; x < width; x += gridOffset) {
        painter.drawLine(x, 0, x, height);
    }

    for (int y = 0; y < height; y += gridOffset) {
        painter.drawLine(0, y, width, y);
    }
    go = gridOffset;
    centerX = (ui->graph->width() / (2 * gridOffset)) * gridOffset;
    centerY = (ui->graph->height() /(2* gridOffset)) * gridOffset;
    centerY += gridOffset/2;
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
    QPen pen= QPen(QColor(200,200,200),gridOffset);
    painter.setPen(pen);

    for(int i=0;i<=height;i+=gridOffset){
        painter.drawRect(gridX, i, gridOffset,0);
    }
    for(int i=0;i<width;i+=gridOffset){
        painter.drawRect(i, gridY, gridOffset,0);
    }
    ui->graph->setPixmap(canvas);
}


void MainWindow::bresenhamline(int x1,int y1,int x2,int y2,QPainter &painter){
    int go = ui->gridoffset->value();
    int screenX1 = (x1 + (centerX/go + 1)) * go;
    int screenY1 = centerY - y1 * go;
    int screenX2 = (x2 + (centerX/go + 1)) * go;
    int screenY2 = centerY - y2 * go;

    // Calculate deltas
    int dx = abs(screenX2 - screenX1);
    int dy = abs(screenY2 - screenY1);

    // Determine step direction
    int sx = (screenX1 < screenX2) ? go : -go;
    int sy = (screenY1 < screenY2) ? go : -go;

    // Initialize error term
    int err = dx - dy;

    // Current position
    int x = screenX1;
    int y = screenY1;

    // Draw the line
    while (true) {
        // Draw current pixel/cell
        painter.drawRect(x - go, y, go, 0);

        // Check if we've reached the end point
        if (x == screenX2 && y == screenY2) {
            break;
        }

        // Calculate error for next step
        int e2 = 2 * err;

        // Step in x direction
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }

        // Step in y direction
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void MainWindow::on_draw_clicked()
{
    int sides = vertices.size();
    int gridOffset = go;bool f = false;

    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(40, 100, 25), go);      // Define your pen
    painter.setPen(pen);
    int n = vertices.size();

    // Draw edges
    for (int i = 0; i < n && !f; ++i) {
        if(sides ==2) f = true;
        int x1 = vertices[i].first, y1 = vertices[i].second;
        int x2 = vertices[(i + 1) % n].first, y2 = vertices[(i + 1) % n].second; // Wrap around to the first vertex
        bresenhamline(x1, y1, x2, y2, painter);
    }
    painter.end();  // End the painter
    ui->graph->setPixmap(QPixmap::fromImage(image));  // Set the modified image back to the pixmap

    cvertices = vertices;
    vertices.clear();
}

int computeCode(int x, int y, int x_min, int y_min, int x_max, int y_max) {
    int code = 0;
    if (y > y_max) code |= TOP;
    if (y < y_min) code |= BOTTOM;
    if (x > x_max) code |= RIGHT;
    if (x < x_min) code |= LEFT;
    return code;
}
void MainWindow::on_cohensutherland_clicked(){
    int x_min=-1*ww,x_max=ww,y_min=-1*wh,y_max=wh;
    int x1=cvertices[0].first,x2=cvertices[1].first,y1=cvertices[0].second,y2=cvertices[1].second;
    int code1 = computeCode(x1, y1, x_min, y_min, x_max, y_max);
    int code2 = computeCode(x2, y2, x_min, y_min, x_max, y_max);

    bool accept = true;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            break;
        } else if (code1 & code2) {
            accept = false;
            break;
        } else {
            int code_out;
            int x, y;

            if (code1 != 0) {
                code_out = code1;
            } else {
                code_out = code2;
            }

            if (code_out & TOP) {
                y = y_max;
                x = x1 + static_cast<int>(round((x2 - x1) * (double)(y_max - y1) / (y2 - y1)));
            } else if (code_out & BOTTOM) {
                y = y_min;
                x = x1 + static_cast<int>(round((x2 - x1) * (double)(y_min - y1) / (y2 - y1)));
            } else if (code_out & RIGHT) {
                x = x_max;
                y = y1 + static_cast<int>(round((y2 - y1) * (double)(x_max - x1) / (x2 - x1)));
            } else if (code_out & LEFT) {
                x = x_min;
                y = y1 + static_cast<int>(round((y2 - y1) * (double)(x_min - x1) / (x2 - x1)));
            }

            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1, x_min, y_min, x_max, y_max);
            } else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2, x_min, y_min, x_max, y_max);
            }
        }
    }
    if (accept)
    {
        ui->graph->setPixmap(QPixmap::fromImage(emptyimage));
        QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
        QImage image = canvas.toImage();        // Convert it to QImage for pixel access
        QPainter painter(&image);                // Use QPainter with QImage
        QPen pen(QColor(255,0,255), go);      // Define your pen
        painter.setPen(pen);
        bresenhamline(x1,y1,x2,y2,painter);
        ui->graph->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        ui->graph->setPixmap(QPixmap::fromImage(emptyimage));
    }
}

int x_intersect(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4)
{
    int num = (x1*y2 - y1*x2) * (x3-x4) -
              (x1-x2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}

// Returns y-value of point of intersection of two lines
int y_intersect(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4)
{
    int num = (x1*y2 - y1*x2) * (y3-y4) -
              (y1-y2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}
void clip(int poly_points[][2], int &poly_size,
          int x1, int y1, int x2, int y2)
{
    int new_points[20][2], new_poly_size = 0;

    // (ix,iy),(kx,ky) are the co-ordinate values of the points
    for (int i = 0; i < poly_size; i++)
    {
        // i and k form a line in polygon
        int k = (i+1) % poly_size;
        int ix = poly_points[i][0], iy = poly_points[i][1];
        int kx = poly_points[k][0], ky = poly_points[k][1];

        // Calculating position of first point w.r.t. clipper line
        int i_pos = (x2-x1) * (iy-y1) - (y2-y1) * (ix-x1);

        // Calculating position of second point w.r.t. clipper line
        int k_pos = (x2-x1) * (ky-y1) - (y2-y1) * (kx-x1);

        // Case 1 : When both points are inside
        if (i_pos < 0 && k_pos < 0)
        {
            // Only second point is added
            new_points[new_poly_size][0] = kx;
            new_points[new_poly_size][1] = ky;
            new_poly_size++;
        }
        // Case 2: When only first point is outside
        else if (i_pos >= 0 && k_pos < 0)
        {
            // Point of intersection with edge and the second point is added
            new_points[new_poly_size][0] = x_intersect(x1, y1, x2, y2, ix, iy, kx, ky);
            new_points[new_poly_size][1] = y_intersect(x1, y1, x2, y2, ix, iy, kx, ky);
            new_poly_size++;

            new_points[new_poly_size][0] = kx;
            new_points[new_poly_size][1] = ky;
            new_poly_size++;
        }
        // Case 3: When only second point is outside
        else if (i_pos < 0 && k_pos >= 0)
        {
            // Only point of intersection with edge is added
            new_points[new_poly_size][0] = x_intersect(x1, y1, x2, y2, ix, iy, kx, ky);
            new_points[new_poly_size][1] = y_intersect(x1, y1, x2, y2, ix, iy, kx, ky);
            new_poly_size++;
        }
        // Case 4: When both points are outside, no points are added
    }

    // Copying new points into original array and changing the no. of vertices
    poly_size = new_poly_size;
    for (int i = 0; i < poly_size; i++)
    {
        poly_points[i][0] = new_points[i][0];
        poly_points[i][1] = new_points[i][1];
    }
}
void MainWindow::on_sutherlandhodgman_clicked(){
    int clipper_size = 4;
    int clipper_points[][2] = {{-ww, -wh}, {-ww,wh}, {ww, wh}, {ww, -wh}};
    int poly_size = cvertices.size();
    int poly_points[20][2];
    int i = 0;
    for(auto x : cvertices){
        poly_points[i][0] = x.first;
        poly_points[i][1] = x.second;
        i++;
    }
    for (int i = 0; i < 4; i++)
    {
        int k = (i + 1) % 4;

        // We pass the current array of vertices, its size and the end points of the selected clipper line
        clip(poly_points, poly_size, clipper_points[i][0],
             clipper_points[i][1], clipper_points[k][0],
             clipper_points[k][1]);
    }

    // Printing vertices of clipped polygon
    for (int i = 0; i < poly_size; i++)
        qDebug() << "(" << poly_points[i][0] << ", " << poly_points[i][1] << ") ";

    ui->graph->setPixmap(QPixmap::fromImage(emptyimage));
    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(255,0,255), go);      // Define your pen
    painter.setPen(pen);
    int n = poly_size;
    for (int i = 0; i < n ; ++i) {
        int x1 = poly_points[i][0], y1 = poly_points[i][1];
        int x2 = poly_points[(i + 1) % n][0], y2 = poly_points[(i + 1) % n][1]; // Wrap around to the first vertex
        bresenhamline(x1, y1, x2, y2, painter);
    }
    ui->graph->setPixmap(QPixmap::fromImage(image));
}
void MainWindow::on_window_clicked()
{
    wh = ui->h->value();
    ww = ui->w->value();
    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(255,255,0), go);      // Define your pen
    painter.setPen(pen);
    bresenhamline(ww,wh,-1*ww,wh,painter);
    bresenhamline(-1*ww,wh,-1*ww,-1*wh,painter);
    bresenhamline(-1*ww,-1*wh,ww,-1*wh,painter);
    bresenhamline(ww,-1*wh,ww,wh,painter);
    emptyimage = image;
    ui->graph->setPixmap(QPixmap::fromImage(image));  // Set the modified image back to the pixmap
}

