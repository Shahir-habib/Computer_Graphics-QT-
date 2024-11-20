#ifndef ENEMY_H
#define ENEMY_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>
#include "bullet.h"
using namespace std;
class Enemy
{
public:
    int dx = 0, dy = 0;
    int x = 0, y = 0;
    int code;
    vector<pair<int,int>> points;
    Enemy(int x,int y,int code = 0, int dx = 0, int dy = 0);
    void draw(MainWindow& w);
    void move();
    void set_move_direction(int dx,int dy);
    void fire_bullet(vector<Bullet>& bullets);
};

#endif // ENEMY_H
