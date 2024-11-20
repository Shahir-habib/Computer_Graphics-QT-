#include "enemy.h"

Enemy::Enemy(int x,int y,int code, int dx, int dy) {
    this->x = x;
    this->y = y;
    this->dx = dx;
    this->dy = dy;
    this->code = code;
    this->points = {
        {3, 0},                  // Tip
        {1, 1}, {5, 1},          // Outer wings
        {0, 2}, {6, 2},          // Extreme wing tips
        {1, 2}, {5, 2},          // Inner wing sections
        {2, 3}, {4, 3},          // Mid-section
        {3, 3},                  // Core center
        {0, 4}, {1, 4}, {5, 4}, {6, 4},  // Reinforced outer frame
        {2, 5}, {4, 5},          // Engine section
        {3, 6}                   // Tail
    };
}


void Enemy::draw(MainWindow& w) {
    QPixmap canvas = w.ui->canvas->pixmap();
    for(auto p: points){
        int x = this->x + p.first;
        int y = this->y + p.second;
        w.putPoint(x*w.gap, y*w.gap, w.gap, &canvas, Qt::red);
        w.w.set_px(code,x,y);
    }
    w.update(canvas);
}
void Enemy::move() {
    x += dx;
    y += dy;
}

void Enemy::set_move_direction(int dx,int dy) {
    this->dx = dx;
    this->dy = dy;
}

void Enemy::fire_bullet(vector<Bullet>& bullets){
    Bullet b(x + 3,y + 6,0,1);
    bullets.push_back(b);
}
