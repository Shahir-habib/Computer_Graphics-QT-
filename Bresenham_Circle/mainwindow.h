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

    void on_polarcircle_clicked();

    void on_BresenhamCircle_clicked();

private:
    Ui::MainWindow *ui;
    void colorPoint(int x, int y, int r, int g, int b, int gridOffset) ;

    bool eventFilter(QObject *watched, QEvent *event) override;
};
#endif // MAINWINDOW_H
