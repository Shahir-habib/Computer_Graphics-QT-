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
vector<pair<int,int>> vertices;
vector<pair<int,int>> cvertices;
vector<pair<int,int>> nvertices;
vector<vector<int>> vmat;
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
int centerX,centerY,go;
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
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graph && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *cursor = static_cast<QMouseEvent*>(event);
        int x = cursor->pos().x();
        int y = cursor->pos().y();

        int gridOffset = ui->gridoffset->value();

        int gridX = (x / gridOffset) * gridOffset;
        int gridY = (y / gridOffset) * gridOffset;
        gridY+=gridOffset/2;
        centerX = (ui->graph->width() / (2 * gridOffset)) * gridOffset;
        centerY = (ui->graph->height() /(2* gridOffset)) * gridOffset;
        centerY += gridOffset/2;
        ui->x->setText(QString::number((gridX/gridOffset +1) - (centerX/gridOffset +1)));
        ui->y->setText(QString::number((centerY/gridOffset) - (gridY/gridOffset )));
        //vertices.push_back({gridX/gridOffset +1,gridY/gridOffset});
        vertices.push_back({(gridX/gridOffset +1) - (centerX/gridOffset +1),(centerY/gridOffset) - (gridY/gridOffset )});
        QPixmap canvas = ui->graph->pixmap();
        QImage image = canvas.toImage();        // Convert it to QImage for pixel access
        QPainter painter(&image);
        QPen pen;QColor c;
        if(true){
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
void initializematrix(){
    vector<int> xc;
    vmat.clear();
    for(auto v : cvertices){
        xc.push_back(v.first);
    }
    vmat.push_back(xc);
    xc.clear();
    for(auto v  :cvertices){
        xc.push_back(v.second);
    }
    vmat.push_back(xc);
    xc.clear();
    for(auto v : cvertices){
        xc.push_back(1);
        //qDebug()<< v.first << v.second ;
    }
    vmat.push_back(xc);
    xc.clear();
}
void MainWindow::bresenhamLine(int x1, int y1, int x2, int y2, const QColor &color1, const QColor &color2, QPainter &painter) {
    int go = ui->gridoffset->value();
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int steps = std::max(dx, dy);

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
        if(true)
            painter.setPen(pen);
        // Draw the pixel
        painter.drawRect((x1 + (centerX/go +1) - 1) * go,centerY - (y1) * go, go, 0);

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
    int go = ui->gridoffset->value(),gridOffset = go;
    if (sides > 3){
        int height = ui->graph->height();
        std::vector<std::pair<int,int>> vertices;
        int xc=ui->x->toPlainText().toInt(),yc=ui->y->toPlainText().toInt();
        // double radius = s/(2*(sin(M_PI/sides)));
        double angleStep = 2 * M_PI / sides;

        double radius =  height/ (5.0 * go);
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
            // QColor c1 = image.pixel((x1)*go -1, (y1)*go + 1);
            // QColor c2 = image.pixel((x2)*go -1 , (y2)*go + 1);
            QColor c1 = image.pixel((x1 + (centerX/gridOffset +1))*go -1 ,   (y1 - (centerY/gridOffset))*go*-1 + 1);
            QColor c2 = image.pixel((x2 + (centerX/gridOffset +1))*go -1 ,   (y2 - (centerY/gridOffset))*go*-1 + 1);
            if(false);// antialiasedLine(x1,y1,x2,y2,painter,image);
            else bresenhamLine(x1, y1, x2, y2, c1, c2, painter);
        }
        painter.end();  // End the painter
        ui->graph->setPixmap(QPixmap::fromImage(image));  // Set the modified image back to the pixmap
    }
    cvertices = vertices;
    initializematrix();
    vertices.clear();
}

vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();
    if (colsA != rowsB) {
        throw invalid_argument("Number of columns in A must be equal to number of rows in B.");
    }
    vector<vector<int>> result(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

void MainWindow::on_translate_clicked()
{
    vector<vector<int>> transmat;
    transmat.push_back({1,0,ui->tx->value()});
    transmat.push_back({0,1, ui->ty->value()});
    transmat.push_back({0,0,1});
    initializematrix();
    vector<vector<int>> r = multiplyMatrices(transmat,vmat);
    for(int i=0;i < r[0].size();i++){
        nvertices.push_back({r[0][i],r[1][i]});
    }
    int go =ui->gridoffset->value();
    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(40, 100, 25), go);      // Define your pen
    painter.setPen(pen);
    int n = nvertices.size();

    // Draw edges
    for (int i = 0; i < n; ++i) {
        int x1 = nvertices[i].first, y1 = nvertices[i].second;
        int x2 = nvertices[(i + 1) % n].first, y2 = nvertices[(i + 1) % n].second; // Wrap around to the first vertex

        // Obtain colors at the endpoints from the image
        int xp1 = vertices[i].first, yp1 = vertices[i].second;
        int xp2 = vertices[(i + 1) % n].first, yp2 = vertices[(i + 1) % n].second;
        QColor c1 = image.pixel((xp1 + (centerX/go +1))*go -1 ,   (yp1 - (centerY/go))*go*-1 + 1);
        QColor c2 = image.pixel((xp2 + (centerX/go +1))*go -1 ,   (yp2 - (centerY/go))*go*-1 + 1);
        bresenhamLine(x1, y1, x2, y2, c1, c2, painter);
    }
    painter.end();  // End the painter
    ui->graph->setPixmap(QPixmap::fromImage(image));
    cvertices = nvertices;
    nvertices.clear();
}


void MainWindow::on_rotate_clicked()
{
    double theta = 1.0 * ui->angle->value() ;

    initializematrix();
    for(int i=0;i < vmat[0].size();i++){
        int x = vmat[0][i],y =vmat[1][i];
        float xn,yn;
        xn = (x * cos(theta) - y * sin(theta));
        yn = (x * sin(theta) + y * cos(theta));
        nvertices.push_back({int(xn),int(yn)});
        qDebug() << xn << yn ;
    }
    int go =ui->gridoffset->value();
    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(40, 100, 25), go);      // Define your pen
    painter.setPen(pen);
    int n = nvertices.size();

    // Draw edges
    for (int i = 0; i < n; ++i) {
        int x1 = nvertices[i].first, y1 = nvertices[i].second;
        int x2 = nvertices[(i + 1) % n].first, y2 = nvertices[(i + 1) % n].second; // Wrap around to the first vertex

        // Obtain colors at the endpoints from the image
        int xp1 = vertices[i].first, yp1 = vertices[i].second;
        int xp2 = vertices[(i + 1) % n].first, yp2 = vertices[(i + 1) % n].second;
        QColor c1 = image.pixel((xp1 + (centerX/go +1))*go -1 ,   (yp1 - (centerY/go))*go*-1 + 1);
        QColor c2 = image.pixel((xp2 + (centerX/go +1))*go -1 ,   (yp2 - (centerY/go))*go*-1 + 1);
        if(false);// antialiasedLine(x1,y1,x2,y2,painter,image);
        else bresenhamLine(x1, y1, x2, y2, c1, c2, painter);
    }
    painter.end();  // End the painter
    ui->graph->setPixmap(QPixmap::fromImage(image));
    cvertices = nvertices;
    nvertices.clear();
}


void MainWindow::on_shear_clicked()
{
    vector<vector<int>> shearmat;
    shearmat.push_back({1,ui->hx->value(),0});
    shearmat.push_back({ui->hy->value(),1,0});
    shearmat.push_back({0,0,1});
    initializematrix();
    vector<vector<int>> r = multiplyMatrices(shearmat,vmat);
    for(int i=0;i < r[0].size();i++){
        nvertices.push_back({r[0][i],r[1][i]});
    }
    int go =ui->gridoffset->value();
    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(40, 100, 25), go);      // Define your pen
    painter.setPen(pen);
    int n = nvertices.size();

    // Draw edges
    for (int i = 0; i < n; ++i) {
        int x1 = nvertices[i].first, y1 = nvertices[i].second;
        int x2 = nvertices[(i + 1) % n].first, y2 = nvertices[(i + 1) % n].second; // Wrap around to the first vertex

        // Obtain colors at the endpoints from the image
        int xp1 = vertices[i].first, yp1 = vertices[i].second;
        int xp2 = vertices[(i + 1) % n].first, yp2 = vertices[(i + 1) % n].second;
        QColor c1 = image.pixel((xp1 + (centerX/go +1))*go -1 ,   (yp1 - (centerY/go))*go*-1 + 1);
        QColor c2 = image.pixel((xp2 + (centerX/go +1))*go -1 ,   (yp2 - (centerY/go))*go*-1 + 1);
        bresenhamLine(x1, y1, x2, y2, c1, c2, painter);
    }
    painter.end();  // End the painter
    ui->graph->setPixmap(QPixmap::fromImage(image));
    cvertices = nvertices;
    nvertices.clear();
}


void MainWindow::on_scale_clicked()
{
    vector<vector<int>> scalemat;
    scalemat.push_back({ui->sx->value(),0,0});
    scalemat.push_back({0,ui->sy->value(),0});
    scalemat.push_back({0,0,1});
    initializematrix();
    vector<vector<int>> r = multiplyMatrices(scalemat,vmat);
    for(int i=0;i < r[0].size();i++){
        nvertices.push_back({r[0][i],r[1][i]});
    }
    int go =ui->gridoffset->value();
    QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
    QImage image = canvas.toImage();        // Convert it to QImage for pixel access
    QPainter painter(&image);                // Use QPainter with QImage
    QPen pen(QColor(40, 100, 25), go);      // Define your pen
    painter.setPen(pen);
    int n = nvertices.size();

    // Draw edges
    for (int i = 0; i < n; ++i) {
        int x1 = nvertices[i].first, y1 = nvertices[i].second;
        int x2 = nvertices[(i + 1) % n].first, y2 = nvertices[(i + 1) % n].second; // Wrap around to the first vertex

        // Obtain colors at the endpoints from the image
        int xp1 = vertices[i].first, yp1 = vertices[i].second;
        int xp2 = vertices[(i + 1) % n].first, yp2 = vertices[(i + 1) % n].second;
        QColor c1 = image.pixel((xp1 + (centerX/go +1))*go -1 ,   (yp1 - (centerY/go))*go*-1 + 1);
        QColor c2 = image.pixel((xp2 + (centerX/go +1))*go -1 ,   (yp2 - (centerY/go))*go*-1 + 1);
        bresenhamLine(x1, y1, x2, y2, c1, c2, painter);
    }
    painter.end();  // End the painter
    ui->graph->setPixmap(QPixmap::fromImage(image));
    cvertices = nvertices;
    nvertices.clear();
}


void MainWindow::on_reflet_clicked()
{
    vector<vector<int>> refx,refy;
    refx.push_back({1,0,0});
    refx.push_back({0,-1,0});
    refx.push_back({0,0,1});
    refy.push_back({-1,0,0});
    refy.push_back({0,1,0});
    refy.push_back({0,0,1});
    if(ui->rx->isChecked()){
        initializematrix();
        vector<vector<int>> r = multiplyMatrices(refx,vmat);
        for(int i=0;i < r[0].size();i++){
            nvertices.push_back({r[0][i],r[1][i]});
        }
        int go =ui->gridoffset->value();
        QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
        QImage image = canvas.toImage();        // Convert it to QImage for pixel access
        QPainter painter(&image);                // Use QPainter with QImage
        QPen pen(QColor(40, 100, 25), go);      // Define your pen
        painter.setPen(pen);
        int n = nvertices.size();

        // Draw edges
        for (int i = 0; i < n; ++i) {
            int x1 = nvertices[i].first, y1 = nvertices[i].second;
            int x2 = nvertices[(i + 1) % n].first, y2 = nvertices[(i + 1) % n].second; // Wrap around to the first vertex

            // Obtain colors at the endpoints from the image
            int xp1 = vertices[i].first, yp1 = vertices[i].second;
            int xp2 = vertices[(i + 1) % n].first, yp2 = vertices[(i + 1) % n].second;
            QColor c1 = image.pixel((xp1 + (centerX/go +1))*go -1 ,   (yp1 - (centerY/go))*go*-1 + 1);
            QColor c2 = image.pixel((xp2 + (centerX/go +1))*go -1 ,   (yp2 - (centerY/go))*go*-1 + 1);
            bresenhamLine(x1, y1, x2, y2, c1, c2, painter);
        }
        painter.end();  // End the painter
        ui->graph->setPixmap(QPixmap::fromImage(image));
        cvertices = nvertices;
        nvertices.clear();
    }
    if(ui->ry->isChecked()){
        initializematrix();
        vector<vector<int>> r = multiplyMatrices(refy,vmat);
        for(int i=0;i < r[0].size();i++){
            nvertices.push_back({r[0][i],r[1][i]});
        }
        int go =ui->gridoffset->value();
        QPixmap canvas = ui->graph->pixmap();  // Get the current pixmap
        QImage image = canvas.toImage();        // Convert it to QImage for pixel access
        QPainter painter(&image);                // Use QPainter with QImage
        QPen pen(QColor(40, 100, 25), go);      // Define your pen
        painter.setPen(pen);
        int n = nvertices.size();

        // Draw edges
        for (int i = 0; i < n; ++i) {
            int x1 = nvertices[i].first, y1 = nvertices[i].second;
            int x2 = nvertices[(i + 1) % n].first, y2 = nvertices[(i + 1) % n].second; // Wrap around to the first vertex

            // Obtain colors at the endpoints from the image
            int xp1 = vertices[i].first, yp1 = vertices[i].second;
            int xp2 = vertices[(i + 1) % n].first, yp2 = vertices[(i + 1) % n].second;
            QColor c1 = image.pixel((xp1 + (centerX/go +1))*go -1 ,   (yp1 - (centerY/go))*go*-1 + 1);
            QColor c2 = image.pixel((xp2 + (centerX/go +1))*go -1 ,   (yp2 - (centerY/go))*go*-1 + 1);
            if(false);// antialiasedLine(x1,y1,x2,y2,painter,image);
            else bresenhamLine(x1, y1, x2, y2, c1, c2, painter);
        }
        painter.end();  // End the painter
        ui->graph->setPixmap(QPixmap::fromImage(image));
        cvertices = nvertices;
        nvertices.clear();
    }
}

