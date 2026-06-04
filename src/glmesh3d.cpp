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
// glmesh3d.cpp
// 3d mesh with OpenGL render functionality
//

#include <cstdlib>

#include "geo.h"
#include "mesh3d.h"

#include "glmesh3d.h"


geo::GlMesh3d::~GlMesh3d() // NOLINT(modernize-use-equals-default)
{
    if (glInitialized_)
    {
        vboVertices_.destroy();
        eboIndices_.destroy();
    }
}

void geo::GlMesh3d::assign(const geo::Mesh3d& mesh)
{
    mesh_ = mesh;
}

geo::GlMesh3d& geo::GlMesh3d::operator=(const geo::Mesh3d& mesh)
{
    assign(mesh);
    return *this;
}
geo::GlMesh3d::GlMesh3d(const geo::Mesh3d& mesh)
{
    assign(mesh);
}

void geo::GlMesh3d::initGL() // NOLINT
{
    if (mesh_.vertices_.empty() || mesh_.indices_.empty())
    {
        return;
    }
    initializeOpenGLFunctions();
}

void geo::GlMesh3d::setupGeometryData()
{
    if (mesh_.vertices_.empty() || mesh_.indices_.empty())
    {
        // strange: should not enter here.
        // geometry vertices and indices should be non empty
        // assert(!mesh_.vertices_.empty());
        return;
    }

    constexpr float k3{ 3 };
    constexpr float kBytesInFloat{ 4 };
    const std::size_t szPoint3f = sizeof(geo::Point3f);
    assert(szPoint3f == k3 * sizeof(float));

    // allocate temp buffer for vertex + normal
    std::vector<float> vertNormals;
    const auto numVertices = static_cast<int>(mesh_.vertices_.size());
    const int numFloatsInVeftNormals = numVertices * (k3 + k3);
    vertNormals.resize(numFloatsInVeftNormals);
    const int numBytesInVertNormals = numFloatsInVeftNormals * kBytesInFloat;

    // write interleaving vertex + normal
    assert(numVertices == mesh_.normals_.size());
    std::size_t j{ 0 };
    for (int i = 0; i < numVertices; i++)
    {
        vertNormals[j++] = mesh_.vertices_[i].x_;
        vertNormals[j++] = mesh_.vertices_[i].y_;
        vertNormals[j++] = mesh_.vertices_[i].z_;

        vertNormals[j++] = mesh_.normals_[i].x_;
        vertNormals[j++] = mesh_.normals_[i].y_;
        vertNormals[j++] = mesh_.normals_[i].z_;
    }

    // Allocate & Populate Vertex Positions
    vboVertices_.create();
    vboVertices_.bind();
    vboVertices_.allocate(vertNormals.data(), numBytesInVertNormals);

    // Allocate & Populate Indices (EBO) --
    eboIndices_.create();
    eboIndices_.bind();
    eboIndices_.allocate(mesh_.indices_.data(),
        static_cast<int>(mesh_.indices_.size() * sizeof(int)));

    glInitialized_ = true;
}

void geo::GlMesh3d::paintGL(QOpenGLShaderProgram& program) // NOLINT
{
    if (mesh_.indices_.empty())
    {
        return;
    }

    vboVertices_.bind();
    eboIndices_.bind();

    const int locColor = program.uniformLocation("color");
    program.setUniformValue(locColor, mesh_.color_.x_, mesh_.color_.y_, mesh_.color_.z_);

    const int locPos = program.attributeLocation("position");
    program.enableAttributeArray(locPos);
    int offset{ 0 };
    constexpr int k2{ 2 };
    constexpr int k3{ 3 };
    const int tupleSize{ k3 };
    const int stride{ sizeof(float) * k3 * k2 };
    program.setAttributeBuffer(locPos, GL_FLOAT, offset, tupleSize, stride);

    const int locNormal = program.attributeLocation("normal");
    program.enableAttributeArray(locNormal);
    offset += sizeof(float) * k3;
    program.setAttributeBuffer(locNormal, GL_FLOAT, offset, tupleSize, stride);

    // Bind layout state and draw triangles based on indexed EBO data
    const GLsizei numIndices = static_cast<GLsizei>(mesh_.indices_.size());
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    program.disableAttributeArray(locNormal);
    program.disableAttributeArray(locPos);
    vboVertices_.release();
    eboIndices_.release();
}
