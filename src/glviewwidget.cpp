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
// glviewwidget.cpp
// OpenGL view widget to show 3d models
//
#include <cmath>
#include <numbers>

#include <QTimer>
#include <QDir>
#include <QDebug>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>

#include "contour.h"
#include "contoursplitter.h" // NOLINT
#include "geo.h"
#include "glmesh3d.h"
#include "meshclip.h" // NOLINT
#include "mesh3dsaver.h" // NOLINT
#include "mesh3dbuilder.h" // NOLINT
#include "colorgenerator.h"

#include "glviewwidget.h"

GlViewWidget::GlViewWidget(QWidget* parent) // NOLINT
    :QOpenGLWidget(parent)
{
    clipPlane_.point_ = { 0.1F, 0.0F, 0.0F }; // NOLINT
    clipPlane_.normal_ = { 0.7071F, 0.664462F, 0.241844F }; // NOLINT
    clipPlane_.normal_.normalize();

    setFocusPolicy(Qt::StrongFocus);
    makeCurrent();
}

GlViewWidget::~GlViewWidget()
{
    makeCurrent();
    // destroy ptr object
    program_.reset();
    doneCurrent();
}

namespace
{
} // namespace

void GlViewWidget::setContour(const geo::Contour& contourObject) // NOLINT
{
    // build from 2d contour by mesh builder
    renderMeshes_.clear();
    sourceMeshes_.clear();

    const geo::ArrayContours2f contoursArray = contourObject.getContours();
    const auto numSrcContours = static_cast<int>(contoursArray.size());
    app::ColorGenerator colorGenerator;
    for (int i = 0; i < numSrcContours; i++)
    {
        geo::ArrayContour2f pointsCopy = contoursArray[i];
        geo::ContourSplitter::makePointsCounterClockwise(pointsCopy);

        geo::GlMesh3d glMesh;
        const float yMin{ -0.5F };
        const float yMax{ 0.5F };
        geo::Mesh3dBuilder::build3dMeshFromConcavePoints(pointsCopy, glMesh.getMesh(), yMin, yMax);
        glMesh.getMesh().color_ = colorGenerator.nextColor();
        renderMeshes_.push_back(glMesh);

        // save source mesh
        const geo::Mesh3d meshCopy{ glMesh.getMesh() };
        sourceMeshes_.push_back(meshCopy);
    } // for i, all contours

    if (glInitialized_)
    {
        for (geo::GlMesh3d& mesh : renderMeshes_)
        {
            mesh.initGL();
            mesh.setupGeometryData();
        }
    }
    
    constexpr bool kNeedCylinderClipTest{ false };
    if(kNeedCylinderClipTest)
    {
        geo::Mesh3d meshCylinder;
        const int numSides{ 11 };
        geo::Mesh3dBuilder::createCylinder(1.0F, 2.0F, numSides, meshCylinder); // NOLINT

        renderMeshes_.clear();
        renderMeshes_.emplace_back(meshCylinder);

        if (glInitialized_)
        {
            renderMeshes_[0].initGL();
            renderMeshes_[0].setupGeometryData();
        }
    }

    constexpr bool kNeedBoxClipTest{ false };
    if (kNeedBoxClipTest)
    {
        geo::Mesh3d meshBox;
        geo::Mesh3dBuilder::createCube(2.0F, meshBox); // NOLINT

        renderMeshes_.clear();
        renderMeshes_.emplace_back(meshBox);

        if (glInitialized_)
        {
            renderMeshes_[0].initGL();
            renderMeshes_[0].setupGeometryData();
        }
    }

    // repaint
    update();
}

void GlViewWidget::showSourceMesh() // NOLINT
{
    renderMeshes_.clear();
    for (geo::Mesh3d& mesh : sourceMeshes_)
    {
        renderMeshes_.emplace_back(mesh);
    }
    if (glInitialized_)
    {
        for (geo::GlMesh3d& glMesh : renderMeshes_)
        {
            glMesh.initGL();
            glMesh.setupGeometryData();
        }
    }

    // repaint
    update();
}

namespace
{

const std::string kVertexShader = R"(
    #version 140
    attribute vec3 position;
    attribute vec3 normal;
    varying vec3 vNormal;
        
    uniform mat4 projection; // == projection * view * model
    uniform mat4 view;
    uniform mat4 model;
        
    void main() 
    {
        gl_Position = projection * view * model * vec4(position, 1.0);
        vNormal = mat3(transpose(inverse(model))) * normal;
        vNormal = normal;
    }
)";

const std::string kFragmentShader = R"(
    #version 140
    varying vec3 vNormal;
    uniform vec3 color;

    void main() 
    {
        vec3 norm = normalize(vNormal);
        vec3 lightDir = normalize(vec3(0.2, 0.1, 0.9));
        float diff = clamp( dot(norm, lightDir), 0.4, 1.0); // set range [0.4..1.0]
        //vec3 objectColor = vec3(0.2, 0.2, 0.9);
        vec3 objectColor = color;
        gl_FragColor = vec4(objectColor * diff, 1.0);
    }
)";

} // namespace

void GlViewWidget::initializeGL()
{
    //makeCurrent();
    initializeOpenGLFunctions();
    // NOLINTBEGIN
    glClearColor(0.999f, 0.999f, 0.999f, 1.0f);
    // NOLINTEND
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling: glEnable(GL_CULL_FACE);


    // create program
    {
        program_ = std::make_unique<QOpenGLShaderProgram>();

        const bool ok1{ program_ && program_->addShaderFromSourceCode(QOpenGLShader::Vertex, kVertexShader.data()) }; // NOLINT
        if (!ok1)
        {
            const QString err = program_->log();
            qDebug() << "OpenGL error: " << err << "\n";
        }

        assert(ok1);
        // Simple Fragment Shader doing basic directional lighting
        const bool ok2{ program_ && program_->addShaderFromSourceCode(QOpenGLShader::Fragment, kFragmentShader.data()) }; // NOLINT
        assert(ok2);
        const bool okLink{ program_ && program_->link() }; // NOLINT
        assert(okLink);
        program_->bind();
    }


    // init GL-related for meshes
    constexpr float kEnd{ 1.0F };
    geo::Mesh3dBuilder::createAxis(geo::Point3f(kEnd, 0.0F, 0.0F), meshAxisX_.getMesh());
    geo::Mesh3dBuilder::createAxis(geo::Point3f(0.0F, kEnd, 0.0F), meshAxisY_.getMesh());
    geo::Mesh3dBuilder::createAxis(geo::Point3f(0.0F, 0.0F, kEnd), meshAxisZ_.getMesh());

    // NOLINTBEGIN
    geo::Mesh3dBuilder::createCube(0.1F, meshBoxX_.getMesh());
    geo::Mesh3dBuilder::createCube(0.1F, meshBoxY_.getMesh());
    geo::Mesh3dBuilder::createCube(0.1F, meshBoxZ_.getMesh());
    geo::Mesh3dBuilder::createCube(0.2F, meshBoxZero_.getMesh());

    meshBoxZero_.getMesh().color_ = { 0.7F, 0.7F, 0.7F };
    meshBoxX_.getMesh().color_ = { 0.99F, 0.1F, 0.1F };
    meshBoxY_.getMesh().color_ = { 0.1F, 0.99F, 0.1F };
    meshBoxZ_.getMesh().color_ = { 0.1F, 0.1F, 0.99F };

    meshAxisX_.getMesh().color_ = { 0.99F, 0.1F, 0.1F };
    meshAxisY_.getMesh().color_ = { 0.1F, 0.99F, 0.1F };
    meshAxisZ_.getMesh().color_ = { 0.1F, 0.1F, 0.99F };

    // NOLINTEND

    geo::Mesh3dBuilder::translateMesh({ 1.0F, 0.0F, 0.0F}, meshBoxX_.getMesh());
    geo::Mesh3dBuilder::translateMesh({ 0.0F, 1.0F, 0.0F }, meshBoxY_.getMesh());
    geo::Mesh3dBuilder::translateMesh({ 0.0F, 0.0F, 1.0F }, meshBoxZ_.getMesh());

    meshBoxX_.initGL();
    meshBoxY_.initGL();
    meshBoxZ_.initGL();
    meshBoxZero_.initGL();

    meshAxisX_.initGL();
    meshAxisY_.initGL();
    meshAxisZ_.initGL();

    meshBoxX_.setupGeometryData();
    meshBoxY_.setupGeometryData();
    meshBoxZ_.setupGeometryData();
    meshBoxZero_.setupGeometryData();

    meshAxisX_.setupGeometryData();
    meshAxisY_.setupGeometryData();
    meshAxisZ_.setupGeometryData();

    for (geo::GlMesh3d& mesh : renderMeshes_)
    {
        mesh.initGL();
        mesh.setupGeometryData();
    }
    glInitialized_ = true;
}

void GlViewWidget::resizeGL(int w, int h) // NOLINT
{
    viewportWidth_ = w;
    viewportHeight_ = h;
}

void GlViewWidget::paintGL() // NOLINT
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float rotationRadY{ rotationAroundYGrad_ * std::numbers::pi_v<float> / 180.0F }; // NOLINT

    const float yEye = rotationRadius_ * sinf(rotationAroundRight_);
    const float xzProj = rotationRadius_ * cosf(rotationAroundRight_);

    const float xEye = xzProj * cosf(rotationRadY);
    const float zEye = xzProj * sinf(rotationRadY);

    QVector3D eye(xEye, yEye, zEye);
    QVector3D target(0.0F, 0.0F, 0.0F);
    QVector3D up(0.0F, 1.0F, 0.0F);

    // final matrix will be: projectionMatrix * viewMatrix * modelMatrix
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(eye, target, up);

    QMatrix4x4 projectionMatrix;
    constexpr float kViewAngle{ 45.0F };
    constexpr float kClipMin{0.1F };
    constexpr float kClipMax{ 100.F };

    projectionMatrix.perspective(
            kViewAngle,
            static_cast<float>(viewportWidth_) / static_cast<float>(viewportHeight_),
            kClipMin,
            kClipMax);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    // set matrices to shader
    program_->bind();
    const int locP{ program_->uniformLocation("projection") }; // NOLINT
    const int locV{ program_->uniformLocation("view") }; // NOLINT
    const int locM{ program_->uniformLocation("model") }; // NOLINT
    program_->setUniformValue(locP, projectionMatrix);
    program_->setUniformValue(locV, viewMatrix);
    program_->setUniformValue(locM, modelMatrix);


    // draw boxes on axis poi
    const bool needDrawBoxes{false};
    if (needDrawBoxes)
    {
        meshBoxX_.paintGL(*program_);
        meshBoxY_.paintGL(*program_);
        meshBoxZ_.paintGL(*program_);
        meshBoxZero_.paintGL(*program_);
    }

    // draw pyramids as axis
    if (axisVisible_)
    {
        meshAxisX_.paintGL(*program_);
        meshAxisY_.paintGL(*program_);
        meshAxisZ_.paintGL(*program_);
    }

    for (geo::GlMesh3d& mesh : renderMeshes_)
    {
        mesh.paintGL(*program_);
    }
}


void GlViewWidget::clipByPlane() // NOLINT
{
    bool clipFound{ false }; // NOLINT
    for (geo::Mesh3d& mesh3d: sourceMeshes_)
    {
        const bool hasClip = geo::Mesh3dClip::canBeClipped(mesh3d, clipPlane_);
        clipFound |= hasClip;
    }
    if (!clipFound)
    {
        return;
    }

    // clip meshes by plane
    std::vector<geo::GlMesh3d> meshesNew;
    for (geo::Mesh3d& mesh3d : sourceMeshes_)
    {
        const bool hasClip = geo::Mesh3dClip::canBeClipped(mesh3d, clipPlane_);
        if (!hasClip)
        {
            constexpr float kHalf{0.5F};
            mesh3d.color_ = {kHalf, kHalf, kHalf};
            meshesNew.emplace_back(mesh3d);
            continue;
        }
        auto pairMeshes = geo::Mesh3dClip::clipMesh(mesh3d, clipPlane_);
        geo::Mesh3d& meshPos = pairMeshes.first;
        geo::Mesh3d& meshNeg = pairMeshes.second;

        constexpr float k03{0.3F};
        meshPos.color_ = { 0.0F, geo::kFloatAlmostOne, k03 };
        meshNeg.color_ = { geo::kFloatAlmostOne, 0.0F, k03 };

        const float height2 = clipHeight_ / 2.0F;

        geo::Mesh3dBuilder::translateMesh({ height2, 0.0F, 0.0F}, meshPos);
        geo::Mesh3dBuilder::translateMesh({ -height2, 0.0F, 0.0F }, meshNeg);

        meshesNew.emplace_back(meshPos);
        meshesNew.emplace_back(meshNeg);
    }
    renderMeshes_.clear();
    for (const auto& m : meshesNew)
    {
        renderMeshes_.push_back(m);
    }
    
    // create GL data for meshes
    if (glInitialized_)
    {
        for (geo::GlMesh3d& graphMesh : renderMeshes_)
        {
            graphMesh.initGL();
            graphMesh.setupGeometryData();
        }
    }
    // repaint gl widget
    update();
}


void GlViewWidget::saveMeshesToObj() // NOLINT
{
    const QString filter = "Obj files (*.obj);;All Files (*)";
    const QString curFolder = QDir::currentPath() + "/../data/out"; // Default path and file name
    qDebug() << curFolder << "\n";

    const QString outFileName = QFileDialog::getSaveFileName(
        this, // Parent widget
        tr("Save to OBJ file"),
        curFolder,
        filter // File type filters
    );
    if (outFileName == "")
    {
        return;
    }

    std::vector<geo::Mesh3d> meshes;

    for (geo::GlMesh3d& glMesh : renderMeshes_)
    {
        meshes.push_back(glMesh.getMesh());
    }

    QString sOut = geo::Mesh3dSaver::saveToObjString(meshes);

    QFile file(outFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) 
    {
        QByteArray ba = sOut.toLocal8Bit();
        file.write(ba);
        file.close();
        const QString msg = "File written successfully: " + outFileName;
        QMessageBox::information(this, "OBJ saver", msg);
    }
    else {
        const QString msg = "Could not open file for writing:: " + outFileName;
        QMessageBox::warning(this, "OBJ saver", msg);
    }
}

void GlViewWidget::onClipped(int state)
{
    if (state == Qt::Checked)
    {
        clipByPlane();
    }
    else
    {
        showSourceMesh();
    }
}

void GlViewWidget::onAxis(int state) // NOLINT
{
    axisVisible_ = (state == Qt::Checked);
    update();
}

void GlViewWidget::onAngleAroundY(int sliderValue)
{
    // get clipping angle from slider, slider value in [0..90]
    const auto angleFloat01 = static_cast<float>(sliderValue);
    const float angleRad{ angleFloat01 * std::numbers::pi_v<float> / 180.0F }; // NOLINT
    clipPlane_.normal_.x_ = 1.0F;
    clipPlane_.normal_.y_ = cosf(angleRad);
    clipPlane_.normal_.z_ = sinf(angleRad);
    clipPlane_.normal_.normalize();

    clipByPlane();
}

void GlViewWidget::onSliderHeight(int sliderValue) // NOLINT    
{
    // assume we have source slider values in [0..24] with "central" value == 12
    // need to transform this range into [0..24]
    // 
    // actual input range in [0..100]
    // NOLINTNEXTLINE
    constexpr float kScaleFromSlider{ 1.0F / 100.0F };
    clipHeight_ = static_cast<float>(sliderValue) * kScaleFromSlider;

    clipByPlane();
}

void GlViewWidget::mousePressEvent(QMouseEvent* event) // NOLINT
{
    if (event->button() == Qt::LeftButton)
    {
        mousePressed_ = true;
        mousePrevPos_.x_ = event->pos().x();
        mousePrevPos_.y_ = event->pos().y();
    }

    QOpenGLWidget::mousePressEvent(event);
}

void GlViewWidget::mouseReleaseEvent(QMouseEvent* event) // NOLINT
{
    if (event->button() == Qt::LeftButton)
    {
        mousePressed_ = false;
    }
    QOpenGLWidget::mouseReleaseEvent(event);
}

void GlViewWidget::mouseMoveEvent(QMouseEvent* event) // NOLINT
{
    if (mousePressed_)
    {
        const int dx = event->pos().x() - mousePrevPos_.x_;
        const int dy = event->pos().y() - mousePrevPos_.y_;
        if ((dx != 0) || (dy != 0))
        {
            rotationAroundYGrad_ += static_cast<float>(dx);
            // NOLINTBEGIN
            if (rotationAroundYGrad_ < 0.0F)
            {
                rotationAroundYGrad_ = 360.0F + rotationAroundYGrad_;
            }
            if (rotationAroundYGrad_ > 359.0F)
            {
                rotationAroundYGrad_ = rotationAroundYGrad_ - 359.0F;
            }

            float rotRight = rotationAroundRight_ * 180.0F / std::numbers::pi_v<float>;
            rotRight += dy;
            rotRight = std::max(rotRight, -89.0F);
            rotRight = std::min(rotRight, 89.0F);
            rotationAroundRight_ = rotRight * std::numbers::pi_v<float> / 180.0F;
            // NOLINTEND

            // repaint
            update();
        }

        mousePrevPos_.x_ = event->pos().x();
        mousePrevPos_.y_ = event->pos().y();
    }

    QOpenGLWidget::mouseMoveEvent(event);
}

void GlViewWidget::wheelEvent(QWheelEvent* event) // NOLINT
{
    const int delta = event->angleDelta().y();
    if (delta != 0)
    {
        // NOLINTBEGIN
        rotationRadius_ += static_cast<float>(delta) / (120.0F / 0.2F);
        rotationRadius_ = std::max(rotationRadius_, 1.0F);
        rotationRadius_ = std::min(rotationRadius_, 10.0F);
        // NOLINTEND
        update();
    }

    QOpenGLWidget::wheelEvent(event);
}

void GlViewWidget::keyPressEvent(QKeyEvent* event) // NOLINT
{
    constexpr int kCodeA{ 65 };
    if (event->key() == kCodeA) 
    {
        axisVisible_ = !axisVisible_;
        update();
    }

    constexpr int kCodeC{ 67 };
    if (event->key() == kCodeC)
    {
        clipByPlane();
    }

    constexpr int kCodeS{ 83 };
    if (event->key() == kCodeS)
    {
        saveMeshesToObj();
        update();
    }

    // pass to base class
    QWidget::keyPressEvent(event);
}

void GlViewWidget::onActionSaveObj()
{
    saveMeshesToObj();
    update();
}