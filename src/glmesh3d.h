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
// glmesh3d.h
// 3d mesh with OpenGL render functionality
//
#ifndef GLMESH3D_H
#define GLMESH3D_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "mesh3d.h"

namespace geo
{

/**
* @class GlMesh3d
* @brief 3D mesh with openGL render functionality
*
* @note Mesh data stores here in order to clip source mesh if need
*/
class GlMesh3d: public QOpenGLFunctions
{
public:
    //! constructor
    GlMesh3d() = default;
    //! destructor
    ~GlMesh3d();

    //! copy constructor
    GlMesh3d(const GlMesh3d&) = default;
    //! move constructor
    GlMesh3d(GlMesh3d&&) = default;
    //! copy assignment
    GlMesh3d& operator=(const GlMesh3d&) = default;
    //! move assignment
    GlMesh3d& operator=(GlMesh3d&&) = default;

    //! initialize openGL
    void initGL();
    //! setup internal buffers with 3d geometry data
    void setupGeometryData();

    //! Paint GL function
    void paintGL(QOpenGLShaderProgram& program);

    //! Assign new mesh
    void assign(const geo::Mesh3d& mesh);
    //! Assignment operator
    GlMesh3d& operator=(const geo::Mesh3d& mesh);
    //! construct with 3d mesh
    explicit GlMesh3d(const geo::Mesh3d& mesh);

    /**
     * @brief Get internal mesh
     * @return mesh data
     */
    Mesh3d& getMesh()
    { 
        return mesh_;
    }

private:

    //! Geometry itself (without openGL-related data)
    Mesh3d mesh_{};

    bool glInitialized_{ false };
    /**
    * Here both 3d vertex + 3d normal is stored in the interleaving layout:
    * x0, y0, z0, nx0, ny0, nz0, x1, y1, z1, nx1, ny1, nz1, ...
    */
    QOpenGLBuffer vboVertices_{ QOpenGLBuffer::VertexBuffer };

    //! Triangle indices
    QOpenGLBuffer eboIndices_{ QOpenGLBuffer::IndexBuffer };
};

} // namespace geo

#endif // GLMESH3D_H

