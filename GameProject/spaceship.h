#ifndef SPACESHIP_H
#define SPACESHIP_H
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <bits/stdc++.h>
using namespace std;
class spaceShip
{
public:
    int dx = 0, dy = 0;
    int x = 0, y = 0;    // Position of the spaceship
    int width =4, height =4; // Size of the spaceship
    int health = 2;  // Example health value
    vector<pair<int,int>> points;
    spaceShip(int x,int y,int dx = 0, int dy = 0);
    void draw(MainWindow& w);
    void move();
    void set_move_direction(int dx,int dy);
    int get_x();// const { return x; }
    int get_y();// const { return y; }
    int get_width();// const { return width; }
    int get_height();// const { return height; }
    void reduce_health(MainWindow& w);
};

#endif // SPACESHIP_H
