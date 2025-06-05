/**
 * @file main.cpp
 * @brief Entry point for the Shadow Segmentation application
 * 
 * @author Created for course project
 * @date June 2025
 */

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Shadow Segmentation");
    window.resize(1200, 800);
    window.show();

    return app.exec();
}
