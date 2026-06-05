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
// mainwindow.h
// Main window for QT based 3d loft viewer application
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <algorithm>

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>

#include "glviewwidget.h"
#include "ui_mainwindow.h"

namespace app::Ui
{
   class MainWindow;
} // namespace app::Ui

/**
* @class MainWindow
* @brief Main ap window with menu and toolbar with icons (3D mesh selection)
*/
class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    //! constructor with parent widget
    explicit MainWindow(QWidget *parent = nullptr);
    //! destructor
    ~MainWindow() override = default;

private slots:
    void onClipped(int state);
    void onAbout();

    void onActionA() { onLoadCharacter("a"); }
    void onActionB() { onLoadCharacter("b"); }
    void onActionI() { onLoadCharacter("i"); }
    void onActionQ() { onLoadCharacter("Q"); }
    void onActionR() { onLoadCharacter("R"); }
    void onActionW() { onLoadCharacter("W"); }
    void onActionZ() { onLoadCharacter("Z"); }

private:

    //! reaction on UI event: press on character load icon
    void onLoadCharacter(const QString& character);

    //! interface from UI
    std::unique_ptr<app::Ui::MainWindow> ui_{};

    //! GL render widget
    std::unique_ptr<GlViewWidget> glViewWidget_{};
};

#endif // MAINWINDOW_H
