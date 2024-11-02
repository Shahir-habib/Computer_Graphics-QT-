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
#include <algorithm>
#define DELAY delay(10)
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

using namespace std;
vector< pair<int,int>> vertices;
vector< pair<int,int>> svertices;
map<pair<int,int>,bool> m;
int ymin,ymax;
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

void MainWindow::on_gridlines_clicked()
{
    int gridOffset = ui->gridoffset->value();
    int width = ui->graph->width();
    int height = ui->graph->height();

    if (gridOffset <= 0) {
        return;
    }

    QImage gridImage(width, height, QImage::Format_ARGB32);
    gridImage.fill(QColor(20, 20, 20));

    QPainter painter(&gridImage);
    painter.setPen(QPen(Qt::white, 1));

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
    QPen pen= QPen(QColor(200,200,200),gridOffset);
    painter.setPen(pen);

    for(int i=0;i<height;i+=gridOffset){
        painter.drawRect(gridX, i, gridOffset,0);
    }
    for(int i=0;i<width;i+=gridOffset){
        painter.drawRect(i, gridY, gridOffset,0);
    }

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
        vertices.push_back({gridX/gridOffset +1,gridY/gridOffset});
        QPixmap canvas = ui->graph->pixmap();
        QImage image = canvas.toImage();        // Convert it to QImage for pixel access
        QPainter painter(&image);
        QPen pen;QColor c;
        if(ui->interpolation->isChecked()){
            int r = rand() %10;
            pen= QPen(colorVector[r],gridOffset);
        }
        else{
            pen = QPen(Qt::blue,gridOffset);
        }
        painter.setPen(pen);
        painter.drawRect(gridX, gridY,gridOffset,0);
        c = image.pixel((gridX/gridOffset +1 )*gridOffset -1, (gridY/gridOffset)*gridOffset + 1);
        QPen pen2(c, ui->gridoffset->value());      // Define your pen
        painter.setPen(pen2);
        painter.drawRect(0,gridOffset/2,ui->gridoffset->value(),0);
        ui->graph->setPixmap(QPixmap::fromImage(image));
    }
    return QMainWindow::eventFilter(watched, event);
}

bool isInsidePolygon(int x, int y) {
    // int n = vertices.size();
    // bool inside = false;

    // for (int i = 0, j = n - 1; i < n; j = i++) {
    //     if (((vertices[i].second > y) != (vertices[j].second > y)) &&
    //         (x < (vertices[j].first - vertices[i].first) * (y - vertices[i].second) / (vertices[j].second - vertices[i].second) + vertices[i].first)) {
    //         inside = !inside;
    //     }
    // }
    // return inside;
    return true;
}
void MainWindow::putPoint(QImage &image, int x,int y,QPainter &painter){
    int go =ui->gridoffset->value();
    painter.drawRect((x-1)*go,(y+0.5)*go,go,0);
    ui->graph->setPixmap(QPixmap::fromImage(image));
    ui->graph->update();
    QEventLoop loop;
    QTimer::singleShot(20,&loop,&QEventLoop::quit);
    loop.exec();
}
void MainWindow::floodFill(QImage &image, int x, int y, const QColor &fillColor, const QColor &oldColor,QPainter &painter) {
    if (x < 0 || x >= image.width() || y < 0 || y >= image.height()) return; // Out of bounds
    int go = ui->gridoffset->value();
    if (image.pixel((x)*go -1, (y)*go + 1) != oldColor.rgb() && image.pixel((x)*go -1, (y)*go + 1) != QColor(200,200,200).rgb())return;

    putPoint(image,x,y,painter);
    floodFill(image, x + 1, y, fillColor, oldColor,painter ); // Right
    floodFill(image, x - 1, y, fillColor, oldColor, painter);
    floodFill(image, x, y + 1, fillColor, oldColor, painter);
    floodFill(image, x, y - 1, fillColor, oldColor, painter);
    if(ui->n8->isChecked()){
        floodFill(image, x + 1, y + 1, fillColor, oldColor,painter );
        floodFill(image, x - 1, y  + 1, fillColor,oldColor,painter );
        floodFill(image, x + 1, y - 1, fillColor, oldColor,painter );
        floodFill(image, x - 1, y - 1, fillColor, oldColor,painter );
    }
}

void MainWindow::on_floodfill_clicked()
{
    vertices.clear();
    QPixmap canvas = ui->graph->pixmap();
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);
    int go = ui->gridoffset->value();
    QColor fillColor = Qt::white;
    QColor oldColor = QColor(20, 20, 20);
    QPen pen(fillColor,go);

    int startX = ui->x->toPlainText().toInt();
    int startY = ui->y->toPlainText().toInt();
    QPen dummy(oldColor,go);
    painter.setPen(dummy);
    painter.drawRect((startX-1)*go,(startY+0.5)*go,go,0);
    painter.setPen(pen);
    if (isInsidePolygon(startX, startY)) {
        // Perform the flood fill operation
        floodFill(image, startX, startY, fillColor, oldColor,painter);
    }
    // Update the pixmap with the modified image
    ui->graph->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::boundaryFill(QImage &image, int x, int y, const QColor &fillColor, const QColor &bColor,QPainter &painter){
    if (x < 0 || x >= image.width() || y < 0 || y >= image.height()) return; // Out of bounds
    int go = ui->gridoffset->value();
    if (image.pixel((x)*go -1, (y)*go + 1) == bColor.rgb() || image.pixel((x)*go -1, (y)*go + 1) == fillColor.rgb() )return;
    putPoint(image,x,y,painter);

    boundaryFill(image, x + 1, y, fillColor, bColor,painter ); // Right
    boundaryFill(image, x - 1, y, fillColor, bColor,painter ); // Left
    boundaryFill(image, x, y + 1, fillColor, bColor,painter ); // Down
    boundaryFill(image, x, y - 1, fillColor, bColor,painter ); // Up
    if(ui->n8->isChecked()){
        boundaryFill(image, x + 1, y + 1, fillColor, bColor,painter );
        boundaryFill(image, x - 1, y  + 1, fillColor, bColor,painter );
        boundaryFill(image, x + 1, y - 1, fillColor, bColor,painter );
        boundaryFill(image, x - 1, y - 1, fillColor, bColor,painter );
    }
}

void MainWindow::on_boundaryfill_clicked()
{
    vertices.clear();
    QPixmap canvas = ui->graph->pixmap();
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);
    int go = ui->gridoffset->value();
    QColor fillColor = Qt::white;
    QColor bColor = QColor(40, 100, 25);
    QPen pen(fillColor,go);

    int startX = ui->x->toPlainText().toInt();
    int startY = ui->y->toPlainText().toInt();
    QColor oldColor = QColor(20,20,20);
    QPen dummy(oldColor,go);
    painter.setPen(dummy);
    painter.drawRect((startX-1)*go,(startY+0.5)*go,go,0);
    painter.setPen(pen);
    if (isInsidePolygon(startX, startY)) {
        // Perform the flood fill operation
        boundaryFill(image, startX, startY, fillColor, bColor,painter);
    }
    // Update the pixmap with the modified image
    ui->graph->setPixmap(QPixmap::fromImage(image));
}
/*
void initialize(){
    ymin = INT_MAX;ymax = INT_MIN;
    for(auto v : svertices){
        if(ymin > v.second) ymin = v.second;
        if(ymax < v.second) ymax = v.second;
    }
    int n  =svertices.size();
    for(int i =0;i<n;i++){
        int y = svertices[i].second;
        int yp = svertices[(i-1 + n)%n].second;
        int yn = svertices[(i+1)%n].second;
        if((y>= yp && y>=yn) || (y<=yp && y<=yn)) {
            m[svertices[i]] = true;
        }
        else m[svertices[i]]= false;
    }
}
vector<vector<int>> yfill;
void MainWindow::on_scanlinefill_clicked()
{
    initialize();
    qDebug()<<ymin<<"     "<< ymax;
    int w = ui->graph->width();
    int go = ui->gridoffset->value();
    for(int i=ymin;i<=ymax;i++){
        vector<int> t;
        for(int x=1;x*go<w;x++){
            pair<int,int> p ={x,i};
            if(m.find(p)!=m.end()){
                if(!m[p]) t.push_back(x);
                else {
                    t.push_back(x);
                    t.push_back(x);
                }
            }
            else{
                QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
                QImage image = canvas.toImage();
                QColor b= QColor(20, 20, 20);
                QColor c = image.pixel((x)*go -1, (i)*go + 1);
                if(c!=b && c != QColor(200,200,200)){
                    t.push_back(x);
                }
            }
        }
        yfill.push_back(t);
    }
    for(auto x:yfill){
        for(auto e : x){
            qDebug()<<e;
        }
        qDebug()<<"____________";
    }
    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(40, 100, 25), go);      // Define your pen
    painter.setPen(pen);
    for(int i=0;i<yfill.size();i++){
        int y = ymin + i;
        if(yfill[i].size()==1 )continue;
        for(int j=0;j< yfill[i].size();){
            // Obtain colors at the endpoints from the image
            if(j==yfill[i].size()-1) break;
            int x1= yfill[i][j],x2=yfill[i][j+1];
            if(x2-x1==1){j++;continue;}
            else{
                j+=2;
            }
            QColor c1 = image.pixel((x1)*go -1, (y)*go + 1);
            QColor c2 = image.pixel((x2)*go -1 , (y)*go + 1);
            bresenhamLine(x1, y, x2, y, c1, c2, painter);
            ui->graph->setPixmap(QPixmap::fromImage(image));
            ui->graph->update();
            QEventLoop loop;
            QTimer::singleShot(90,&loop,&QEventLoop::quit);
            loop.exec();
        }
    }
    ui->graph->setPixmap(QPixmap::fromImage(image));
    svertices.clear();
}
//This Code fail for some concave polygons but perfect for convex .
*/

struct Edge {
    int ymax;   // Maximum y-coordinate of the edge
    float xmin;  // Minimum x-coordinate where scanline intersects edge
    float slopeinverse;  // 1/slope of the edge

    Edge(int ymax, float xmin, float slopeinverse)
        : ymax(ymax), xmin(xmin), slopeinverse(slopeinverse) {}
};
void MainWindow::on_scanlinefill_clicked(){

    vector< pair<int,int>> vertices = svertices;
    int go = ui->gridoffset->value();
    int ymin = vertices[0].second;
    int ymax = vertices[0].second;
    for (const auto& vertex : vertices) {
        ymin = min(ymin, vertex.second);
        ymax = max(ymax, vertex.second);
    }
    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(40, 100, 25), go);      // Define your pen
    painter.setPen(pen);
    QColor cl = QColor(40,100,25),bl = QColor(20,20,20);
    // Initialize edge table
    vector<list<Edge>> edgeTable(ymax + 1);

    // Build edge table
    for (size_t i = 0; i < vertices.size(); i++) {
        // Get current vertex and next vertex
        int x1 = vertices[i].first;
        int y1 = vertices[i].second;
        int x2 = vertices[(i + 1) % vertices.size()].first;
        int y2 = vertices[(i + 1) % vertices.size()].second;

        // Skip horizontal edges
        if (y1 == y2) continue;

        // Ensure y1 < y2
        if (y1 > y2) {
            swap(x1, x2);
            swap(y1, y2);
        }

        // Calculate inverse slope
        float slopeinverse = float(x2 - x1) / (y2 - y1);

        // Create and add edge to edge table
        Edge edge(y2, x1, slopeinverse);
        edgeTable[y1].push_back(edge);
    }

    // Active Edge Table (AET) - initially empty
    list<Edge> activeEdges;

    // Process each scanline
    for (int y = ymin; y <= ymax; y++) {
        // Remove edges from AET where ymax = y
        activeEdges.remove_if([y](const Edge& edge) {
            return edge.ymax == y;
        });

        // Move edges from edge table to AET
        activeEdges.splice(activeEdges.end(), edgeTable[y]);

        // Sort AET by xmin
        activeEdges.sort([](const Edge& e1, const Edge& e2) {
            return e1.xmin < e2.xmin;
        });

        // Fill pixels between pairs of intersections
        auto it = activeEdges.begin();
        while (it != activeEdges.end()) {
            // Get first x-coordinate
            float x1 = it->xmin;
            ++it;
            if (it == activeEdges.end()) break;

            // Get second x-coordinate
            float x2 = it->xmin;
            ++it;
            QColor c1 = image.pixel((int(x1))*go -1, (y)*go + 1);
            QColor c2 = image.pixel((int(x2))*go -1 , (y)*go + 1);
            if(c1==bl){
                if(bl!=image.pixel((int(x1) + 1)*go -1, (y)*go + 1)) c1 = image.pixel((int(x1) + 1)*go -1, (y)*go + 1);
                else c1 = image.pixel((int(x1) - 1)*go -1, (y)*go + 1);
            }

            if(c2==bl){
                if(bl!=image.pixel((int(x2) + 1)*go -1, (y)*go + 1)) c2 = image.pixel((int(x2) + 1)*go -1, (y)*go + 1);
                else c2 = image.pixel((int(x2) - 1)*go -1, (y)*go + 1);
            }
            bresenhamLine(int(x1),y,int(x2) + 1,y,c1,c2,painter);
            qDebug() << " from "<< int(x1) << "to" << int(x2) << "on" << y;
        }

        // Update xmin values for active edges
        for (auto& edge : activeEdges) {
            edge.xmin += edge.slopeinverse;
        }
    }
    ui->graph->setPixmap(QPixmap::fromImage(image));
    svertices.clear();
}

void MainWindow::bresenhamLine(int x1, int y1, int x2, int y2, const QColor &color1, const QColor &color2, QPainter &painter) {
    int go = ui->gridoffset->value();
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int steps =  max(dx, dy);

    // Color interpolation
    float rStep = static_cast<float>(color2.red() - color1.red()) / steps;
    float gStep = static_cast<float>(color2.green() - color1.green()) / steps;
    float bStep = static_cast<float>(color2.blue() - color1.blue()) / steps;

    // Initialize the current color
    float r = color1.red();
    float g = color1.green();
    float b = color1.blue();

    // Calculate direction
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    for (int i = 0; i < steps; ++i) {
        // Set the pen color
        QColor currentColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
        QPen pen(currentColor, go); // Define your pen
        if(ui->interpolation->isChecked())
            painter.setPen(pen);

        // Draw the pixel
        painter.drawRect((x1 - 1) * go, (y1 + 0.5) * go, go, 0);

        // Update the current color for the next step
        r += rStep;
        g += gStep;
        b += bStep;

        // Bresenham's line algorithm
        if (2 * err > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (2 * err < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void MainWindow::on_draw_clicked()
{
    int sides = ui->n->value();
    int go = ui->gridoffset->value();
    if (sides > 3){
        int height = ui->graph->height();
        vector< pair<int,int>> vertices;
        int xc=ui->x->toPlainText().toInt(),yc=ui->y->toPlainText().toInt();
        // double radius = s/(2*(sin(M_PI/sides)));
        double angleStep = 2 * M_PI / sides;

        double radius =  height/ (5.0 * go);


        for (int i = 0; i < sides; ++i) {
            double angle = i * angleStep;
            int x = static_cast<int>(xc + radius *  cos(angle));
            int y = static_cast<int>(yc + radius * sin(angle));
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
            bresenhamLine(x1,y1,x2,y2, Qt::blue,Qt::blue,painter);
        }
        ui->graph->setPixmap(canvas);
    }
    else{   //Draw Manually
        QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
        QImage image = canvas.toImage();        // Convert it to QImage for pixel access
        QPainter painter(&image);                // Use QPainter with QImage
        QPen pen(QColor(40, 100, 25), go);      // Define your pen
        painter.setPen(pen);
        int n = vertices.size();

        // Draw edges
        for (int i = 0; i < n; ++i) {
            int x1 = vertices[i].first, y1 = vertices[i].second;
            int x2 = vertices[(i + 1) % n].first, y2 = vertices[(i + 1) % n].second; // Wrap around to the first vertex

            // Obtain colors at the endpoints from the image
            QColor c1 = image.pixel((x1)*go -1, (y1)*go + 1);
            QColor c2 = image.pixel((x2)*go -1 , (y2)*go + 1);
            bresenhamLine(x1, y1, x2, y2, c1, c2, painter);
        }
        painter.end();  // End the painter
        ui->graph->setPixmap(QPixmap::fromImage(image));  // Set the modified image back to the pixmap
    }
    svertices= vertices;
    vertices.clear();
}

