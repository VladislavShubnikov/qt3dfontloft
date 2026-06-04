/**
 * @file main.cpp
 * @brief Main entry point for Viewer application
 * @author Vladislav Shubnikov
 */
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QtGlobal>
#include <QLibraryInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app_icon_256.png"));

    // NOLINTNEXTLINE
    MainWindow w;
    w.show();

    return app.exec();
}
