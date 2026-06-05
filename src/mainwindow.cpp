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
// mainwindow.cpp
// Main window for QT based 3d loft viewer application
//

#include <cstdint>

#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>
#include <QTextEdit>
#include <QTextStream>

#include "ui_mainwindow.h"
#include "mainwindow.h"

#include "geo.h"
#include "contour.h"


// NOLINTNEXTLINE
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<app::Ui::MainWindow>())
    , glViewWidget_(std::make_unique<GlViewWidget>(parent) )
{
    ui_->setupUi(this);

    // replace default QOpenGLWidget to the custom GlViewWidget
    ui_->verticalLayout->replaceWidget(ui_->openGLWidget, glViewWidget_.get());
    delete ui_->openGLWidget;
    ui_->openGLWidget = nullptr;

    setFocusPolicy(Qt::StrongFocus);

    // load default json
    onLoadCharacter("Q");

    // actions for main window
    connect(ui_->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui_->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui_->actionSaveObj, &QAction::triggered, glViewWidget_.get(), &GlViewWidget::onActionSaveObj);

    // 3d camera movement
    connect(ui_->sliderAngleY, &QSlider::valueChanged, glViewWidget_.get(), &GlViewWidget::onAngleAroundY);
    connect(ui_->sliderHeight, &QSlider::valueChanged, glViewWidget_.get(), &GlViewWidget::onSliderHeight);

    connect(ui_->checkClippped, &QCheckBox::stateChanged, glViewWidget_.get(), &GlViewWidget::onClipped);
    connect(ui_->checkClippped, &QCheckBox::stateChanged, this, &MainWindow::onClipped);
    connect(ui_->checkAxis, &QCheckBox::stateChanged, glViewWidget_.get(), &GlViewWidget::onAxis);

    // load characters
    connect(ui_->actionA, &QAction::triggered, this, &MainWindow::onActionA);
    connect(ui_->actionB, &QAction::triggered, this, &MainWindow::onActionB);
    connect(ui_->actionI, &QAction::triggered, this, &MainWindow::onActionI);
    connect(ui_->actionQ, &QAction::triggered, this, &MainWindow::onActionQ);
    connect(ui_->actionR, &QAction::triggered, this, &MainWindow::onActionR);
    connect(ui_->actionW, &QAction::triggered, this, &MainWindow::onActionW);
    connect(ui_->actionZ, &QAction::triggered, this, &MainWindow::onActionZ);
}

void MainWindow::onClipped(int state)
{
    if (state == Qt::Checked)
    {
        ui_->sliderAngleY->setEnabled(true);
        ui_->sliderHeight->setEnabled(true);
    }
    if (state == Qt::Unchecked)
    {
        ui_->sliderAngleY->setEnabled(false);
        ui_->sliderHeight->setEnabled(false);
    }
}

void MainWindow::onLoadCharacter(const QString& character)
{
    // load json
    geo::Contour contourObject;
    const bool okRead = contourObject.readCharacterFromJson(character);
    if (okRead)
    {
        contourObject.removeDuplicates();
        constexpr float kSize{1.5F};
        contourObject.scaleTo(kSize);
        glViewWidget_->setContour(contourObject);

        // UI: clipping disable
        ui_->checkClippped->setChecked(false);
        ui_->sliderAngleY->setEnabled(false);
        ui_->sliderHeight->setEnabled(false);
    }
    else
    {
        const QString strErr = "Error loaded letter " + character;
        QMessageBox::critical(this, "Error loading 3D mesh", strErr);
    }
}

void MainWindow::onAbout()
{
    QMessageBox::about(
        this,

        "About Loft Viewer",

        "This is a simple <b>Loft viewer</b> application <br>"
        "Mouse control: <br>"
        "<b>Left button</b>: Rotate around vertical axis <br>"
        "<b>Right button</b>: Rotate around right camera direction <br>"
        "<b>Wheel</b>: Zoom in/out <br>");
}

