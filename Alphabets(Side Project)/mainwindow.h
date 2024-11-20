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
   void  gridlines();
 void putPoint(QImage &image, int x,int y,QPainter &painter);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
