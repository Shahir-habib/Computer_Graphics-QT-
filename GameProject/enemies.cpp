#include "enemies.h"
#include "enemy.cpp"

Enemies::Enemies(int cnt) : cnt(cnt), alive_cnt(cnt), x_max(-999), x_min(99999) {
    for (int i = 0; i < cnt; ++i) {
        enemies[i + 1] = new Enemy((i + 1) * 15, 10, i + 1);
    }
    updateBounds();
}

void Enemies::re_populate(){
    for (int i = 0; i < cnt; ++i) {
        enemies[i + 1] = new Enemy((i + 1) * 15, 10, i + 1);
    }
    updateBounds();
}

void Enemies::draw(MainWindow& w) {
    for (const auto& [key, enemy] : enemies) {
        if (enemy) {
            enemy->draw(w);
        }
    }
}

void Enemies::move() {
    for (const auto& [key, enemy] : enemies) {
        if (enemy) {
            enemy->move();
        }
    }
    x_max += dx;
    x_min += dx;
}

void Enemies::set_move_direction(int dx, int dy) {
    this->dx = dx;
    this->dy = dy;
    for (const auto& [key, enemy] : enemies) {
        if (enemy) {
            enemy->set_move_direction(dx, dy);
        }
    }
}

void Enemies::fire_bullets(vector<Bullet>& bullets) {
    for (const auto& [key, enemy] : enemies) {
        if (enemy) {
            enemy->fire_bullet(bullets);
        }
    }
}

void Enemies::remove_dead_enemies(int code) {
    if (enemies.count(code) && enemies[code]) {
        delete enemies[code];
        enemies[code] = nullptr;
        --alive_cnt;
        updateBounds();
        totalpoint++;
    }
    qDebug()<<alive_cnt;
     qDebug() << "===============================================================================================";
    if(alive_cnt == 0){
        re_populate();
        alive_cnt = cnt;
    }
}

void Enemies::updateBounds() {
    x_max = -999;
    x_min = 99999;
    for (const auto& [key, enemy] : enemies) {
        if (enemy) {
            x_max = max(x_max, enemy->x);
            x_min = min(x_min, enemy->x);
        }
    }
}

