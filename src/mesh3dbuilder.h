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
// mesh3dbuilder.h
// Build Mesh3d by adding triangle one by one, and build indices
//
#ifndef MESH3DBUILDER_H
#define MESH3DBUILDER_H

#include "geo.h"
#include "mesh3d.h"

namespace geo
{

class Mesh3dBuilder
{
public:
    //! vLookTo can be (1,0,0) or (0,1,0) or (0,0,1)
    static void createAxis(const geo::Point3f& vLookTo, geo::Mesh3d& outMesh);
    static void createCube(float side, geo::Mesh3d& outMesh);
    static void createCylinder(float radius, float height, int numSides, geo::Mesh3d& outMesh);

    static void translateMesh(const geo::Point3f& vAdd, geo::Mesh3d& outMesh);
    static void calcVertexNormalsByTriNormals(geo::Mesh3d& mesh);

    //! points are convex polygone
    static void build3dMeshFromPoints(const ArrayContour2f& points, Mesh3d& outMesh);

    static void build3dMeshFromConcavePoints(const ArrayContour2f& points, Mesh3d& outMesh, float yMin, float yMax);
};

} // namespace geo

#endif // MESH3DBUILDER_H
