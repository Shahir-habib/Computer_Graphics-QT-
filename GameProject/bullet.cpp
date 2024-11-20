#include "bullet.h"

Bullet::Bullet(int x, int y,int dx,int dy) {
    this->x = x;
    this->y = y;
    this->dx = dx;
    this->dy = dy;


}


void Bullet::draw(MainWindow& w) {
    QPixmap canvas = w.ui->canvas->pixmap();
    w.putPoint(x*w.gap, y*w.gap, w.gap, &canvas, Qt::darkYellow); // dark yellow bullet
    // w.putPoint((x + 1)*w.gap, y*w.gap, w.gap, &canvas, Qt::darkYellow); // dark yellow bullet
    // w.putPoint((x - 1)*w.gap, y*w.gap, w.gap, &canvas, Qt::darkYellow); // dark yellow bullet
    w.update(canvas);
}

void Bullet::move() {
    x += dx;
    y += dy;
}

void Bullet::set_move_direction(int dx,int dy) {
    this->dx = dx;
    this->dy = dy;
}
