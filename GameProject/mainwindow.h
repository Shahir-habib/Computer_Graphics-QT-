#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bits/stdc++.h>
using namespace std;
class MyWindow {
public:
    int height;
    int width;
    vector<vector<int>> points;

    MyWindow() = default;

    void set_params(int h, int w) {
        height = h;
        width = w;
        points.resize(h, vector<int>(w, 0));
    }

    void set_px(int code, int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            points[y][x] = code;
        } else {
            // qWarning() << "Invalid point";
        }
    }

    int get_px(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return points[y][x];
        } else {
            // qWarning() << "Invalid point";
        }

        return 0;
    }

    void clear() {
        for (auto& row : points) {
            fill(row.begin(), row.end(), 0);
        }
    }
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int gap = 5;
    bool f= false;
    MyWindow w;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void drawTextOnPixmap(QPixmap &canvas, const QString &text, int x, int y, QColor color = Qt::white);
    void putPoint(int x, int y, int gap, QPixmap* canvas, QColor color);
    void update(QPixmap& canvas);
    void clear();
    void updateGame();

protected:
    void keyPressEvent(QKeyEvent *event) override; 

public:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
