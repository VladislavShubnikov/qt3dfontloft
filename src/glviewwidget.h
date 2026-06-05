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

/**
 * @class GlViewWidget
 * @brief Widget with OpenGL render ouput
 * 
 * This widget uses source 3D meshes to create openGL render meshes
 * 
 * @note Wireframe render is not implemehted here
 * 
 */
class GlViewWidget: public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	//! constructor
	GlViewWidget(QWidget* parent = nullptr);
	//! destructor
	~GlViewWidget();

	// NOLINTBEGIN
	//! copy constructor
	GlViewWidget(const GlViewWidget&) = delete;
	//! copy assignment
	GlViewWidget& operator=(const GlViewWidget&) = delete;
	//! constructor with move semantic
	GlViewWidget(GlViewWidget&&) = delete;
	//! move assignment
	GlViewWidget& operator=(GlViewWidget&&) = delete;
	// NOLINTEND

	/**
	* @brief Assign extern 2D font letter contour to the 3d mesh oject.
	* 3D loft creates here using @ref geo::Mesh3dBuilder
	* @param contourObject set of input 2d contours
	*/
	void setContour(const geo::Contour& contourObject);

public slots: // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)


	/**
	* @brief Reaction on user interface change in 1st slider: clipping angle vaue set
	* @param sliderValue Value in [0..90]
	*/
	void onAngleAroundY(int sliderValue);

	/**
	* @brief Reaction on user interface change in 2st slider: artificial distance between clipped meshed
	* @param sliderValue Value in [0..100]
	*/
	void onSliderHeight(int sliderValue);

	/**
	* @brief Switch on/off clipping during 3D mesh render
	* @param state state of check box, see Qt::Checked
	*/
	void onClipped(int state);

	/**
	* @brief Switch on/off show artificial 3D axises
	* @param state State of the checkbox, see Qt::Checked
	*/
	void onAxis(int state);

	/**
	* @brief Open dialog with OBJ file name selection and perform save to OBJ format
	*/
	void onActionSaveObj();
	
protected:

	/**
	* @brief Create 3d meshes for axis, boxes and if 3D mesh is ready,
	* create gl data for rendering. GL shader compilation is also performs here
	*/
	void initializeGL() override;

	/**
	* @brief Calculate current view matrix based on camera position
	*/
	void paintGL() override;

	//! reaction on mouse button press event
	void mousePressEvent(QMouseEvent* event) override;
	//! reaction on mouse button release event
	void mouseReleaseEvent(QMouseEvent* event) override;
	//! reaction on mouse move event
	void mouseMoveEvent(QMouseEvent* event) override;
	//! reaction on mouse wheel event
	void wheelEvent(QWheelEvent* event) override;
	//! reaction on keybord key press event
	void keyPressEvent(QKeyEvent* event) override;

private:
	/**
	* @brief Show source 3D mesh set without clipping
	*/
	void showSourceMesh();

	/**
	* @brief Perform source 3D mesh clipping by current clip plane
	*/
	void clipByPlane();

	/**
	* @brief Save current clipped 3D mesh to *.OBJ text file to see
	* 3D object in the external obj file viewer
	*/
	void saveMeshesToObj();

	//! rotation around vertical axis in graduses
	float rotationAroundYGrad_{ 0.0F };

	//! rotation around right vector
	float rotationAroundRight_{ 0.0F };

	//! default camera distance to zero point
	const float defaultRadius_{ 2.0F };

	//! eye rotation radius
	float rotationRadius_{ defaultRadius_ };

	//! mouse pressed
	bool mousePressed_{ false };

	//! mouse prev coord
	geo::Point2d mousePrevPos_{};

	//! special flag to avoid init GL again for new contour
	bool glInitialized_{ false };

	//! compiler openGL shader
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
