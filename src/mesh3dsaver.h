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
// mesh3dsaver.h
// Save 3d mesh to OBJ text format
//
#ifndef MESH3DSAVER_H
#define MESH3DSAVER_H

#include <vector>
#include <QString>

#include "mesh3d.h"

namespace geo
{

/**
* @class Mesh3dSaver
* @brief Saver functionality for 3D mesh
*/
class Mesh3dSaver
{
public:
	/**
	* @brief Save single mesh object to OBJ format string
	* @param mesh input mesh
	* @return string representation of OBJ file
	*/
    static [[nodiscard]] QString saveToObjString(const geo::Mesh3d& mesh);

	/**
	* @brief Save mesh set to OBJ format string
	* @param meshes input meshes
	* @return string representation of OBJ file
	*/
	static [[nodiscard]] QString saveToObjString(const std::vector<geo::Mesh3d>& meshes);
}; // class Mesh3DSaver

} // namespace geo

#endif // MESH3DSAVER_H
 