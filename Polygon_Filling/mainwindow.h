#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_gridlines_clicked();

    void on_axis_clicked();

    void on_scanlinefill_clicked();

    void on_floodfill_clicked();

    void on_boundaryfill_clicked();

    void on_draw_clicked();

private:
    Ui::MainWindow *ui;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void delay(int ms);
    void putPoint(QImage &image, int x,int y,QPainter &painter);
    void floodFill(QImage &image, int x, int y, const QColor &fillColor, const QColor &oldColor,QPainter &painter);
    void boundaryFill(QImage &image, int x, int y, const QColor &fillColor, const QColor &oldColor,QPainter &painter);
    void bresenhamLine(int x1, int y1, int x2, int y2, const QColor &color1, const QColor &color2, QPainter &painter);
};
#endif // MAINWINDOW_H
