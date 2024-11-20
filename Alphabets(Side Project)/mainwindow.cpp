#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QMouseEvent>
#include <QDebug>
#include <QColor>
#include <QVector>
#include <algorithm>
#include <vector>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graph->setMouseTracking(true);
    ui->graph->installEventFilter(this);

    QPixmap canvas = ui->graph->pixmap(Qt::ReturnByValue);
    if (canvas.isNull()) {
        canvas = QPixmap(ui->graph->size());
        canvas.fill(Qt::white);
        ui->graph->setPixmap(canvas);
    }
    gridlines();
}
int gridOffset=8,go =8;


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

int calculateStringWidth(const std::vector<std::vector<std::pair<int, int>>>& letters, const std::string& text, int spacing) {
    return (text.size() * 5) + (text.size() - 1) * spacing;
}
void MainWindow::gridlines()
{

    int width = ui->graph->width();
    int height = ui->graph->height();


    QImage gridImage(width, height, QImage::Format_ARGB32);
    gridImage.fill(Qt::white);

    QPainter painter(&gridImage);
    painter.setPen(QPen(Qt::gray, 1));

    for (int x = 0; x < width; x += gridOffset) {
        painter.drawLine(x, 0, x, height);
    }

    for (int y = 0; y < height; y += gridOffset) {
        painter.drawLine(0, y, width, y);
    }

    ui->graph->setPixmap(QPixmap::fromImage(gridImage));
    // int wx = 0;  // Horizontal offset
    // int wy = 0;  // Vertical offset
    // int n = 26;  // Total letters to draw
    // int spacing = 2; // Spacing between letters

    // for (int i = 0; i < letters.size(); i++) {
    //     if (i % 5 == 0 && i != 0) {
    //         // Move to the next row after every 5 letters
    //         wx = 0;
    //         wy += 7 + spacing; // Adjust vertical position
    //     }

    //     for (auto x : letters[i]) {
    //         putPoint(gridImage, (x.first + wx) * go, (x.second + wy) * go, painter);
    //     }

    //     wx += 5 + spacing; // Move to the next letter position
    // }
    std::string heading = "COMPUTER GRAPHICS";
    int spacing = 1;       // Space between letters
    int go = 8;           // Grid scale factor
    int gridWidth = ui->graph->width()/go;   // Grid width in "units" (adjust as needed)
    int gridHeight = ui->graph->height();  // Grid height in "units" (adjust as needed)

    // Calculate the starting position for centering
    int headingWidth = calculateStringWidth(letters, heading, spacing);
    int wx = (gridWidth * go - headingWidth * go) / 2 / go; // Centered starting x-position
    int wy = 2;  // Fixed position on the second row (adjust as needed)

    // Draw the heading
    for (char ch : heading) {
        if (ch == ' ') { // Handle spaces
            wx += 3 + spacing; // Adjust spacing for spaces
            continue;
        }

        int index = ch - 'A'; // Map character to letter index
        for (auto x : letters[index]) {
            putPoint(gridImage, (x.first + wx) * go, (x.second + wy) * go, painter);
        }
        wx += 5 + spacing; // Move to the next letter position
        if(index=='M'-'A'||index == 'W'-'A')wx++;
    }
    // Centering ASSIGNMENT REPORT
    std::string subheading = "ASSIGNMENT REPORT";
    int subheadingSpacing = 1;  // Space between letters for the subheading

    // Calculate width of subheading
    int subheadingWidth = calculateStringWidth(letters, subheading, subheadingSpacing);
    int wxSub = (gridWidth * go - subheadingWidth * go) / 2 / go; // Centered x-position for subheading
    int wySub = wy + 6; // Position below "COMPUTER GRAPHICS" (adjust as needed)

    // Draw the subheading
    wx = wxSub; // Reset wx for subheading
    for (char ch : subheading) {
        if (ch == ' ') { // Handle spaces
            wx += 3 + subheadingSpacing; // Adjust spacing for spaces
            continue;
        }

        int index = ch - 'A'; // Map character to letter index
        for (auto x : letters[index]) {
            putPoint(gridImage, (x.first + wx) * go, (x.second + wySub) * go, painter);
        }
        wx += 5 + subheadingSpacing; // Move to the next letter position
    }
    // Member details
    std::vector<std::string> names = {"SWAPNAMOY GARAI", "ABHINANDAN BARUA", "ABRAR AHMED ANSARI", "SHAHIR HABIB"};
    vector<std::string> rollNumber = {"002210501001","002210501002","002210501005","002210501006"};

    int nameStartX = 5;  // Starting position (left side of the grid)
    int nameStartY = wySub + 15;  // Position below "ASSIGNMENT REPORT"
    int lineSpacing = 20;  // Spacing between the start of each name-roll pair

    // Draw names and roll numbers
    for (int i = 0; i < names.size(); ++i) {
        int wxName = nameStartX;  // Reset x-position for each name
        int wyName = nameStartY + i * lineSpacing;  // Line-specific y-position

        // Draw name
        for (char ch : names[i]){
            if (ch == ' ') {
                wxName += 3; // Space between words
                continue;
            }

            int index = ch - 'A'; // Map character to letter index
            for (auto x : letters[index]) {
                putPoint(gridImage, (x.first + wxName) * go, (x.second + wyName) * go, painter);
            }
            wxName += 5; // Move to next letter position
            if(index=='M'-'A' || index == 'W' -'A')wxName++;
        }

        // Draw roll number below name
        int wxRoll = nameStartX;  // Align roll number to the name
        int wyRoll = wyName + 8;  // Adjust y-position to below the name

        for (char ch : rollNumber[i]) {
            int index = ch - '0'; // Map character to digit index (create digit mapping if needed)
            for (auto x : numbers[index]) { // Assuming digit coordinates are added to 'letters'
                putPoint(gridImage, (x.first+10 + wxRoll) * go, (x.second + wyRoll) * go, painter);
            }
            wxRoll += 5; // Move to next digit position
        }
    }
    QPixmap canvas = ui->graph->pixmap();
    canvas.save("./Frontpage.png");

}
void MainWindow::putPoint(QImage &image, int x,int y,QPainter &painter){

    QPixmap canvas = ui->graph->pixmap();
    QPainter painter2(&canvas);
    QPen pen= QPen(QColor(30,30,20),gridOffset);
    painter2.setPen(pen);
    painter2.drawRect(x+go, y-go/2, gridOffset,0);
    ui->graph->setPixmap(canvas);

}
MainWindow::~MainWindow()
{
    delete ui;
}
