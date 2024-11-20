#ifndef BULLET_H
#define BULLET_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>
using namespace std;

class Bullet
{
public:
    int dx = 0, dy = 0;
    int x = 0, y = 0;
    vector<pair<int,int>> points;
    Bullet(int x,int y,int dx = 0, int dy = 0);
    void draw(MainWindow& w);
    void move();
    void set_move_direction(int dx,int dy);
};

#endif // BULLET_H
