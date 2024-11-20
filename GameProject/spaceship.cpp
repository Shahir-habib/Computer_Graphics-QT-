#include "spaceship.h"

spaceShip::spaceShip(int x, int y, int dx,int dy) {
    this->x = x;
    this->y = y;
    this->dx = dx;
    this->dy = dy;
    //this->points = {{2, 0},{1, 1},{3, 1},{1, 2},{2, 2},{3, 2},{1, 3},{2, 3},{3, 3},{1, 4},{2, 4},{3, 4},{0, 5},{1, 5},{2, 5},{3, 5},{4, 5}};
    this->points = { {0, 0}, {1, 0}, {2, 0}, {3, 0},
                    {0, 1}, {1, 1}, {2, 1}, {3, 1},
                    {0, 2}, {1, 2}, {2, 2}, {3, 2},
                    {0, 3}, {1, 3}, {2, 3}, {3, 3}}    ;
}

void spaceShip::draw(MainWindow& w) {
    QPixmap canvas = w.ui->canvas->pixmap();
    for(auto p: points){
        int x = this->x + p.first;
        int y = this->y + p.second;
        w.putPoint(x*w.gap, y*w.gap, w.gap, &canvas, Qt::green);
    }
    w.update(canvas);
}

void spaceShip::move() {
    x += dx;
    y += dy;
}

void spaceShip::set_move_direction(int dx,int dy) {
    this->dx = dx;
    this->dy = dy;
}
int spaceShip::get_x()  { return this->x; }
int spaceShip::get_y()  { return this->y; }
int spaceShip::get_width()  { return width; }
int spaceShip::get_height()  { return height; }

void spaceShip::reduce_health(MainWindow& w) {
    this->health -= 1;  // For example, reduce health by 10
    if (this->health <= 0) {
        // Handle spaceship destruction
        w.f =true;
        qDebug() << "Spaceship destroyed!";
        qDebug() << "===============================================================================================";
        // You can trigger game over or other logic here
    }
}
