// The MIT License (MIT)
//
// Copyright (c) 2026 Vladislav Shubnikov
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// main.cpp
// Main entry point for Viewer application
//
#include <QApplication>
#include <QtGlobal>
#include <QLibraryInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

 /**
 * @brief main entry to app
 * @note
 *   In some cases, on Ubuntu, we need to perform
 * 
 *   qputenv("QT_QPA_PLATFORM", "xcb");
 * 
 *   before app declaration
 * @param argc number of arguments
 * @param argv strings in arguments
 * @return 0 if successful
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app_icon_256.png"));

    // NOLINTNEXTLINE
    MainWindow w;
    w.show();

    return app.exec();
}
