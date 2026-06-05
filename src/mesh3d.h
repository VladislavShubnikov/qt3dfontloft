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
// mesh3d.h
// Basic 3d mesh data (without rendering functionality)
//

#ifndef MESH3D_H
#define MESH3D_H

#include <vector>
#include "geo.h"
#include "contour.h"

namespace geo
{

/**
* @class Mesh3d
* @brief Geometry of 3D mesh (set of meshes)
*
* @note 3D mesh is produced from 2D contours. For many font letters
* this is a set of contours
* Clipping operation is performed under this data structure
*
*/
class Mesh3d
{
public:
    //! constructor
    Mesh3d() = default;
    //! copy constructor
    Mesh3d(const Mesh3d&) = default;
    //! move assignment
    Mesh3d& operator=(const Mesh3d&) = default;
    //! in future maybe should repeat the same move semantic as in move constructor
    Mesh3d& operator=(Mesh3d&&) = default; 

    //! destructor
    ~Mesh3d();
    //! constructor with move semantic
    Mesh3d(Mesh3d&& other) noexcept;

    /**
    * @brief Get source vertices, used for GL render
    * @return 3D vertices
    */
    std::vector<Point3f> vertices_{};

    /**
    * @brief Get source vertex normals, used for GL render
    * @return 3D vertex normals
    */
    std::vector<Point3f> normals_{};

    /**
    * @brief Get source vertex normals, used for GL render
    * @return 3D vertex normals
    */
    std::vector<uint32_t> indices_{}; // 3 * number of triangles

    //! Material color for this mesh
    Point3f color_{0.0F, kFloatAlmostOne, 0.0F};
};

} // namespace geo

#endif // MESH3D_H
