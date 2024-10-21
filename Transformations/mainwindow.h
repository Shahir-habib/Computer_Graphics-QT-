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

    void on_draw_clicked();

    void on_translate_clicked();

    void on_rotate_clicked();

    void on_shear_clicked();

    void on_scale_clicked();

    void on_reflet_clicked();

private:
    Ui::MainWindow *ui;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void bresenhamLine(int x1, int y1, int x2, int y2, const QColor &color1, const QColor &color2, QPainter &painter);
};
#endif // MAINWINDOW_H
