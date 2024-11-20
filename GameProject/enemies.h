#ifndef ENEMIES_H
#define ENEMIES_H
#include "enemy.h"
#include <unordered_map>
#include <vector>
using namespace std;

class Enemies
{
public:
    unordered_map<int, Enemy *> enemies;
    int cnt;
    int alive_cnt;
    int x_max = -999;
    int x_min = 99999;
    int totalpoint;
    int dx = 0, dy = 0;
    bool is_moving_right = true;

    Enemies(int cnt);

    void draw(MainWindow& w);
    void move();
    void set_move_direction(int dx, int dy);
    void fire_bullets(vector<Bullet>& bullets);
    void remove_dead_enemies(int code);
    void updateBounds();
    void re_populate();
};

#endif // ENEMIES_H

