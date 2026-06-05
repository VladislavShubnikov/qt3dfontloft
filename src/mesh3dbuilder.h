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

/**
* @class Mesh3dBuilder
* @brief Builder for 3d mesh
* @note Simple loft operation is performed here to create 3D mesh from 2D contour
*/
class Mesh3dBuilder
{
public:
    
    /**
    * @brief Create axis mesh. source vector vLookTo should be one from (0,0,1), (0,1,0) or (0,0,1)
    * @param vLookTo axis look vector
    * @param outMesh output 3D mesh
    */
    static void createAxis(const geo::Point3f& vLookTo, geo::Mesh3d& outMesh);

    /**
    * @brief Create 3d cube mesh.
    * @param side side of cube
    * @param outMesh output 3D mesh
    */
    static void createCube(float side, geo::Mesh3d& outMesh);

    /**
    * @brief Create 3d cylinder mesh.
    * @param radius cylinder radius
    * @param height cylinder height
    * @param numSides number of sides
    * @param outMesh output 3D mesh
    */
    static void createCylinder(float radius, float height, int numSides, geo::Mesh3d& outMesh);

    /**
    * @brief Perform += v operation on mesh
    * @param vAdd vector to add to all vertices
    * @param outMesh output 3D mesh
    */
    static void translateMesh(const geo::Point3f& vAdd, geo::Mesh3d& outMesh);

    /**
    * @brief Calculate vertices normals, based on triangle normals
    * @param mesh in and out 3D mesh
    */
    static void calcVertexNormalsByTriNormals(geo::Mesh3d& mesh);

    /**
    * @brief Build 3D mesh from 2D contour(s) points
    * @param points of contours
    * @param outMesh out 3D mesh
    */
    static void build3dMeshFromPoints(const ArrayContour2f& points, Mesh3d& outMesh);

    /**
    * @brief Build 3D mesh (loft) from 2D contour(s) points with given min and max value on 3rd dimension
    * @param points of contours
    * @param outMesh out 3D mesh
    * @param yMin minimum on y for loft
    * @param yMax maximum on y for loft
    */
    static void build3dMeshFromConcavePoints(const ArrayContour2f& points, Mesh3d& outMesh, float yMin, float yMax);
};

} // namespace geo

#endif // MESH3DBUILDER_H
