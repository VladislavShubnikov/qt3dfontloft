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
// glviewwidget.h
// OpenGL view widget to show 3d models
//
#ifndef GLVIEWWIDGET_H
#define GLVIEWWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <vector>

#include "geo.h"
#include "contour.h"
#include "plane3d.h"
#include "glmesh3d.h"

constexpr float kDefaultHeight{ 0.12F };

class GlViewWidget: public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	GlViewWidget(QWidget* parent = nullptr);
	~GlViewWidget();

	// NOLINTBEGIN
	GlViewWidget(const GlViewWidget&) = delete;
	GlViewWidget& operator=(const GlViewWidget&) = delete;
	GlViewWidget(GlViewWidget&&) = delete;
	GlViewWidget& operator=(GlViewWidget&&) = delete;
	// NOLINTEND

	void setContour(const geo::Contour& contourObject);

public slots: // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
	void onAngleAroundY(int sliderValue);
	void onSliderHeight(int sliderValue);
	void onClipped(int state);
	void onAxis(int state);
	void onActionSaveObj();
	
protected:
	void initializeGL() override;
	void paintGL() override;

	// receive all mouse events
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

private:

	void showSourceMesh();
	void clipByPlane();
	void saveMeshesToObj();

	//! rotation around vertical axis in graduses
	float rotationAroundYGrad_{ 0.0F };

	//! rotation around right vector
	float rotationAroundRight_{ 0.0F };

	const float defaultRadius_{ 2.0F };

	//! eye rotation radius
	float rotationRadius_{ defaultRadius_ };

	//! mouse pressed
	bool mousePressed_{ false };

	//! mouse prev coord
	geo::Point2d mousePrevPos_{};

	//! special flag to avoid init GL again for new contour
	bool glInitialized_{ false };

	std::unique_ptr<QOpenGLShaderProgram> program_{};

	//! meshes, received from 2d contours for render
	std::vector<geo::GlMesh3d> renderMeshes_{};

	//! source meshes, required for further clipping
	std::vector<geo::Mesh3d> sourceMeshes_{};

	//! clipping plane. Animated by UI.
	geo::Plane3d clipPlane_;

	//! clipping height shift. Animated by UI.
	float clipHeight_{ kDefaultHeight };

	//! Render axis
	geo::GlMesh3d meshAxisX_{};
	geo::GlMesh3d meshAxisY_{};
	geo::GlMesh3d meshAxisZ_{};
	//! Flag to switch on/off axis render
	bool axisVisible_{ true };


	//! Render box
	geo::GlMesh3d meshBoxZero_{};
	geo::GlMesh3d meshBoxX_{};
	geo::GlMesh3d meshBoxY_{};
	geo::GlMesh3d meshBoxZ_{};
};

#endif // GLVIEWWIDGET_H
