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

    void on_cohensutherland_clicked();

    void on_sutherlandhodgman_clicked();

    void on_window_clicked();

private:
    Ui::MainWindow *ui;
     bool eventFilter(QObject *watched, QEvent *event) override;
    void bresenhamline(int x1,int y1,int x2,int y2,QPainter &painter);
};
#endif // MAINWINDOW_H
