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
    QApplication a(argc, argv);
    // NOLINTNEXTLINE
    MainWindow w;
    w.show();

    return a.exec();
}
