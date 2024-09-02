/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *graph;
    QSpinBox *gridoffset;
    QLabel *label;
    QPushButton *gridlines;
    QPushButton *axis;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *firstpoint;
    QPushButton *secondpoint;
    QTextBrowser *px1;
    QTextBrowser *x;
    QTextBrowser *y;
    QTextBrowser *py1;
    QTextBrowser *px2;
    QTextBrowser *py2;
    QPushButton *dda;
    QPushButton *bresenham;
    QLCDNumber *ddatime;
    QLCDNumber *bresentime;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1313, 723);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        graph = new QLabel(centralwidget);
        graph->setObjectName("graph");
        graph->setGeometry(QRect(62, -20, 1141, 531));
        graph->setMouseTracking(true);
        graph->setTabletTracking(true);
        graph->setScaledContents(true);
        gridoffset = new QSpinBox(centralwidget);
        gridoffset->setObjectName("gridoffset");
        gridoffset->setGeometry(QRect(60, 600, 101, 31));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 550, 101, 31));
        gridlines = new QPushButton(centralwidget);
        gridlines->setObjectName("gridlines");
        gridlines->setGeometry(QRect(200, 550, 111, 31));
        axis = new QPushButton(centralwidget);
        axis->setObjectName("axis");
        axis->setGeometry(QRect(200, 600, 111, 31));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(360, 540, 91, 21));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(560, 540, 91, 21));
        firstpoint = new QPushButton(centralwidget);
        firstpoint->setObjectName("firstpoint");
        firstpoint->setGeometry(QRect(352, 590, 91, 31));
        secondpoint = new QPushButton(centralwidget);
        secondpoint->setObjectName("secondpoint");
        secondpoint->setGeometry(QRect(352, 630, 91, 31));
        px1 = new QTextBrowser(centralwidget);
        px1->setObjectName("px1");
        px1->setGeometry(QRect(470, 590, 71, 31));
        x = new QTextBrowser(centralwidget);
        x->setObjectName("x");
        x->setGeometry(QRect(470, 540, 71, 31));
        y = new QTextBrowser(centralwidget);
        y->setObjectName("y");
        y->setGeometry(QRect(670, 540, 71, 31));
        py1 = new QTextBrowser(centralwidget);
        py1->setObjectName("py1");
        py1->setGeometry(QRect(580, 590, 71, 31));
        px2 = new QTextBrowser(centralwidget);
        px2->setObjectName("px2");
        px2->setGeometry(QRect(470, 630, 71, 31));
        py2 = new QTextBrowser(centralwidget);
        py2->setObjectName("py2");
        py2->setGeometry(QRect(580, 630, 71, 31));
        dda = new QPushButton(centralwidget);
        dda->setObjectName("dda");
        dda->setGeometry(QRect(830, 540, 121, 31));
        bresenham = new QPushButton(centralwidget);
        bresenham->setObjectName("bresenham");
        bresenham->setGeometry(QRect(1040, 540, 121, 31));
        ddatime = new QLCDNumber(centralwidget);
        ddatime->setObjectName("ddatime");
        ddatime->setGeometry(QRect(800, 580, 151, 61));
        bresentime = new QLCDNumber(centralwidget);
        bresentime->setObjectName("bresentime");
        bresentime->setGeometry(QRect(1010, 580, 151, 61));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1313, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        graph->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Grid Offset", nullptr));
        gridlines->setText(QCoreApplication::translate("MainWindow", "Show Gridlines", nullptr));
        axis->setText(QCoreApplication::translate("MainWindow", "Show Axis", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "x-coordinate", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "y-coordinate", nullptr));
        firstpoint->setText(QCoreApplication::translate("MainWindow", "First Point", nullptr));
        secondpoint->setText(QCoreApplication::translate("MainWindow", "Second Point", nullptr));
        dda->setText(QCoreApplication::translate("MainWindow", "DDA Line", nullptr));
        bresenham->setText(QCoreApplication::translate("MainWindow", "Bresenham Line", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
