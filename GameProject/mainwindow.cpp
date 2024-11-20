#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include<bits/stdc++.h>

#include "spaceship.h"
#include "bullet.h"
#include "enemies.h"

using namespace std;

void MainWindow::drawTextOnPixmap(QPixmap &canvas, const QString &text, int x, int y, QColor color) {
    QPainter painter(&canvas);
    painter.setPen(color);
    painter.setFont(QFont("Arial", 12, QFont::Bold)); // Customize font as needed
    painter.drawText(x, y, text); // Draw text at specified coordinates
    ui->canvas->setPixmap(canvas); // Update the canvas to display the text
}
QTimer * timer;
int go,gridOffset;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus); // Enable keyboard focus
    go=gridOffset=gap;
    int height = ui->canvas->height()/gap;
    int width = ui->canvas->width()/gap;
    w.set_params(height,width);

    //Pixmap initialization on canvas
    QPixmap canvas = ui->canvas->pixmap(Qt::ReturnByValue);
    if (canvas.isNull()) {
        canvas = QPixmap(ui->canvas->size());
        canvas.fill(Qt::black);
        ui->canvas->setPixmap(canvas);
    }

    drawTextOnPixmap(canvas,"Hello",100,200);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(50);
}

void MainWindow::putPoint(int x, int y, int gap, QPixmap* canvas, QColor color = Qt::green){
    QPainter painter(canvas);
    QPen pen = QPen(color);
    painter.setPen(pen);
    QRect rect(x, y, gap, gap );
    painter.drawRect(rect);
}

void MainWindow::update(QPixmap& canvas){
    ui->canvas->setPixmap(canvas);
    ui->canvas->update();

}


void MainWindow::clear(){
    auto canvas = QPixmap(ui->canvas->size());
    canvas.fill(Qt::black);
    ui->canvas->setPixmap(canvas);
    w.clear();
    // drawTextOnPixmap(canvas,s,10,20);
}




MainWindow::~MainWindow()
{
    delete ui;
}





// global variabes
spaceShip s(20,20);
vector<Bullet> bullets;
// Enemy e(10,10);
Enemies es(5);


int spaceship_dx = 0;
int spaceship_dy = 0;
int enemyMoveCounter = 0;
//


void MainWindow::keyPressEvent(QKeyEvent *event){
    int d = 3  ; // Define the movement distance
    int x = s.x+ 2;
    int y = s.y;
    switch (event->key()) {
    case Qt::Key_Up:
        spaceship_dx = 0;
        spaceship_dy = -d;
        break;
    case Qt::Key_Down:
        spaceship_dx = 0;
        spaceship_dy = d;
        break;
    case Qt::Key_Left:
        spaceship_dx = -d;
        spaceship_dy = 0;
        break;
    case Qt::Key_Right:
        spaceship_dx = d;
        spaceship_dy = 0;
        break;
    case Qt::Key_Space:
        bullets.push_back(Bullet(x, y,0, -1));
        bullets.push_back(Bullet(x, y,1, -1));
        bullets.push_back(Bullet(x, y,-1, -1));
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}int cc=0;

void MainWindow::updateGame(){

    if (f) {

        // If the game is over, display GAME OVER and stop the timer
        //  QPainter painter(&canvasp); // You can use QPainter to draw text on the canvas
        //  canvasp.fill(Qt::white);
        //  painter.setPen(QPen(Qt::red)); // Set the color for the text
        //  painter.setFont(QFont("Arial", 30)); // Set the font size and style
        //  painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
        // // painter.drawText(rect(), Qt::AlignCenter,  (to_string(es.totalpoint)));
        //  qDebug() <<es.totalpoint;
        //  ui->canvas->setPixmap(canvasp);
        // Stop the timer to stop the game loop

        vector<vector<pair<int,int>>> letters = {
            // A
            {{1, 0}, {2, 0}, {0, 1}, {3, 1}, {0, 2}, {1, 2}, {2, 2}, {3, 2}, {0, 3}, {3, 3}, {0, 4}, {3, 4}},
            // B
            {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {3, 1}, {0, 2}, {1, 2}, {2, 2}, {0, 3}, {3, 3}, {0, 4}, {1, 4}, {2, 4}},
            // C
            {{1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 4}, {2, 4}, {3, 4}},
            // D
            {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {3, 1}, {0, 2}, {3, 2}, {0, 3}, {3, 3}, {0, 4}, {1, 4}, {2, 4}},
            // E
            {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {0, 3}, {0, 4}, {1, 4}, {2, 4}, {3, 4}},
            // F
            {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {0, 3}, {0, 4}},
            // G
            {{1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3}, {3, 2}, {3, 4}, {1, 4}, {2, 4}},
            // H
            {{0, 0}, {3, 0}, {0, 1}, {3, 1}, {0, 2}, {1, 2}, {2, 2}, {3, 2}, {0, 3}, {3, 3}, {0, 4}, {3, 4}},
            // I
            {{0, 0}, {1, 0}, {2, 0}, {1, 1}, {1, 2}, {1, 3}, {0, 4}, {1, 4}, {2, 4}},
            // J
            {{2, 0}, {3, 0}, {2, 1}, {2, 2}, {2, 3}, {0, 4}, {1, 4}, {2, 4}},
            // K
            {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 1}, {3, 0}, {2, 3}, {3, 4}, {0, 4}},
            // L
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 4}, {2, 4}, {3, 4}},
            // M
            {{0, 0}, {4, 0}, {0, 1}, {1, 1}, {3, 1}, {4, 1},{0, 2}, {2, 2}, {4, 2}, {0, 3}, {4, 3}, {0, 4}, {4, 4}},
            // N
            {{0, 0}, {3, 0}, {0, 1}, {1, 1}, {3, 1}, {0, 2},{2, 2}, {3, 2}, {0, 3}, {3, 3}, {0, 4}, {3, 4}},
            // O
            {{1, 0}, {2, 0},  {0, 1}, {3, 1}, {0, 2}, {3, 2}, {0, 3}, {3, 3}, {1, 4}, {2, 4}},
            // P
            {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {3, 1}, {0, 2}, {1, 2}, {2, 2}, {0, 3}, {0, 4}},
            // Q
            {{1, 0}, {2, 0}, {3, 0}, {0, 1}, {3, 1}, {0, 2}, {3, 2}, {0, 3}, {3, 3}, {1, 4}, {2, 4}, {3, 4}, {4, 4}},
            // R
            {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {3, 1}, {0, 2}, {1, 2}, {2, 2}, {0, 3}, {2, 3}, {3, 4}, {0, 4}},
            // S
            {{1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {3, 3},{ 0, 4}, {1, 4}, {2, 4}, {3, 4}},
            // T
            {{0, 0}, {1, 0}, {2, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}},
            // U
            {{0, 0}, {3, 0}, {0, 1}, {3, 1}, {0, 2}, {3, 2}, {0, 3}, {3, 3}, {1, 4}, {2, 4}},
            // V
            {{0, 0}, {4, 0}, {0, 1}, {4, 1}, {1, 2}, {3, 2}, {2, 3}},
            // W
            { {0, 0}, {4, 0}, {0, 1}, {4, 1}, {0, 2}, {2, 2},{4, 2}, {0, 3}, {2, 3}, {4, 3}, {1, 4}, {3, 4}},
            // X
            {{0, 0}, {3, 0}, {1, 1}, {2, 1}, {2, 2}, {1, 2}, {1, 3}, {2, 3}},
            // Y
            {{0, 0},{1,1},{3,1}, {4, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}},
            // Z
            {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {2, 1}, {1, 2}, {1, 3}, {0, 4}, {1, 4}, {2, 4}, {3, 4}}
        };
        std::vector<std::vector<std::pair<int, int>>> numbers = {
            // 0
            {{9, 1}, {10, 1}, {8, 2}, {11, 2}, {8, 3}, {11, 3}, {8, 4}, {11, 4}, {9, 5}, {10, 5}},

            // 1
            {{9,2},{10, 1}, {10, 2}, {10, 3}, {10, 4}, {10, 5}},

            // 2
            {{9, 1}, {10, 1}, {8, 2}, {11, 2}, {10, 3}, {9, 4},{8, 5}, {9, 5}, {10, 5}, {11, 5}},

            // 3
            {{8, 1}, {9, 1}, {10, 1}, {11, 2}, {10, 3}, {11, 4}, {8, 5}, {9, 5}, {10, 5}, {11, 5}},

            // 4
            {{11, 1}, {10, 2}, {9, 3}, {8, 4}, {9, 4}, {10, 4}, {11, 4}, {11, 5}},

            // 5
            {{8, 1}, {9, 1}, {10, 1}, {8, 2}, {8, 3}, {9, 3}, {10, 3}, {11, 4}, {8, 5}, {9, 5}, {10, 5}},

            // 6
            {{10, 1}, {9, 2}, {8, 3}, {9, 3}, {10, 3}, {11, 4}, {9, 5}, {10, 5},{8,4},{8,5}},

            // 7
            {{8, 1}, {9, 1}, {10, 1}, {11, 1}, {10, 2}, {10, 3}, {10, 4}, {10, 5}},

            // 8
            {{9, 1}, {10, 1}, {8, 2}, {11, 2}, {9, 3}, {10, 3}, {8, 4}, {11, 4}, {9, 5}, {10, 5}},

            // 9
            {{9, 1}, {10, 1}, {8, 2}, {11, 2}, {10, 3}, {11, 4}, {8, 5}, {9, 5}, {10, 5}, {11, 5}}
        };
        QPixmap canvasp = ui->canvas->pixmap(); // Retrieve the current canvas pixmap
        QPainter painter2(&canvasp);
        canvasp.fill(Qt::black); // Clear the canvas with a white background

        QPen pen(Qt::red, gridOffset); // Set up the pen
        painter2.setPen(pen);

        std::string names = "GAME OVER";
        std::string score = "YOUR SCORE "+std::to_string(es.totalpoint);
        int wxName = 0, wyName = 20; // Starting coordinates for drawing text
        int gap = 10;                // Gap between characters

        for (char ch : names) {
            if (ch == ' ') {
                wxName += 3; // Space between words
                continue;
            }

            if (ch < 'A' || ch > 'Z') {
                // Handle invalid characters (e.g., '=' or others)
                wxName += 3; // Skip or add a specific offset
                continue;
            }


            int index = ch - 'A'; // Map character to letter index
            for (auto x : letters[index]) {
                int xd = (x.first + wxName) * gap;
                int yd = (x.second + wyName) * gap;

                // Draw the corresponding pixel (slightly adjust positions)
                painter2.drawRect(xd, yd, gap, gap);
            }

            wxName += 5; // Move to the next letter's position
            if (index == 'M' - 'A' || index == 'W' - 'A') wxName++;
        }
        wyName+=15;wxName=0;
        qDebug()<<score;
        for (char ch : score) {
            if (ch == ' ') {
                wxName += 3; // Space between words
                continue;
            }
            if(ch>='0'&& ch<='9'){
                int index = ch -'0';
                for (auto x : numbers[index]) {
                    int xd = (x.first + wxName) * gap;
                    int yd = (x.second + wyName) * gap;

                    // Draw the corresponding pixel (slightly adjust positions)
                    painter2.drawRect(xd, yd, gap, gap);
                }
                wxName += 5;
                continue;
            }

            int index = ch - 'A'; // Map character to letter index
            for (auto x : letters[index]) {
                int xd = (x.first + wxName) * gap;
                int yd = (x.second + wyName) * gap;

                // Draw the corresponding pixel (slightly adjust positions)
                painter2.drawRect(xd, yd, gap, gap);
            }

            wxName += 5; // Move to the next letter's position
            if (index == 'M' - 'A' || index == 'W' - 'A') wxName++;
        }
        // Set the updated pixmap to the canvas
        ui->canvas->setPixmap(canvasp);

        // Stop the timer after rendering
        timer->stop();
        return;
    }

    clear();

    // Draw and Move spaceship
    s.set_move_direction(spaceship_dx, spaceship_dy); 
    s.move();
    s.draw(*this);
    spaceship_dx = 0;
    spaceship_dy = 0;

    // Draw enemies and move them
    int width = ui->canvas->width()/gap;
    int height = ui->canvas->height()/gap;
    es.draw(*this);
    if (enemyMoveCounter >= 30) {
        if (es.is_moving_right) {
            if (es.x_max >= width - 20) {
                es.is_moving_right = false;  // Switch to moving left
                es.set_move_direction(0,10);
                es.move();
            }
        } else {
            if (es.x_min <= 20) {
                es.is_moving_right = true;   // Switch to moving right
                es.set_move_direction(0,10);
                es.move();
            }
        }

        // Set the movement direction based on the updated state
        int direction = es.is_moving_right ? 1 : -1;
        es.set_move_direction(direction*10, 0);

        // qDebug()<<es.x_min<<" "<<width<<"\n";
        es.move();
        enemyMoveCounter = 0;
    } else {
        if (enemyMoveCounter == 20) {
            es.fire_bullets(bullets);
        }
        ++enemyMoveCounter;
    }
    // qDebug()<<bullets.size()<<"\n";
    // Move bullets and remove out-of-bounds bullets
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->move();
        it->draw(*this);
        //  qDebug()<<w.get_px(it->x,it->y)<<"\n";
        int code = w.get_px(it->x,it->y);
        if(code != 0){
            es.remove_dead_enemies(code);
            bullets.erase(it);
            continue;
        }

        // Check collision with spaceship
        if (it->x >= s.get_x() && it->x <= s.get_x() + s.get_width() &&
            it->y >= s.get_y() && it->y <= s.get_y() + s.get_height()) {
            // Bullet hit spaceship, handle the collision
            // For example, reduce spaceship's health or end the game
            qDebug() << "Collision detected between bullet and spaceship!"<<cc;cc++;
            // You can call a method here to reduce health or end the game.
            s.reduce_health(*this);  // Assuming there's a method to handle health reduction
            bullets.erase(it);  // Remove the bullet that hit the spaceship
            continue;
        }



        // Remove bullet if it goes out of bounds
        if (it->y < 0 || it->y > height) {
            it = bullets.erase(it);
        } else {
            // Check for collision with other bullets
            bool collision_detected = false;
            for (auto inner_it = std::next(it); inner_it != bullets.end(); ) {
                if (it->x <= inner_it->x + 1 && it->x >= inner_it->x - 1 && it->y == inner_it->y) {
                    // Collision detected, remove both bullets
                    inner_it = bullets.erase(inner_it);
                    it = bullets.erase(it);
                    collision_detected = true;
                    break;
                } else {
                    ++inner_it;
                }
            }

            // Move to the next bullet if no collision was detected for the current one
            if (!collision_detected) {
                ++it;
            }
        }
    }


}

