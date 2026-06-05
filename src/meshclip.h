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
// meshclip.h
// Clipping of the Mesh3d
//
#ifndef MESHCLIP_H
#define MESHCLIP_H

#include <utility>
#include "mesh3d.h"
#include "plane3d.h"

namespace geo
{

/**
* @enum ClipResult
* @brief Get result of clipping 3D triangle by plane
* @note All vertices can be from plane positive side, negative side, or clipped
*/
enum class ClipResult : uint8_t
{
	//! Clipped object on positive side of plane
	kOnPositiveSide = 0,
	//! Clipped object on negative side of plane
	kOnNegativeSide = 1,
	//! Clipped object is clipped by plane
	kClipped = 2
};

/**
* @class Mesh3dClip
* @brief Clip 3d mesh
* @note Clipping result is pair of meshes
*/
class Mesh3dClip
{
public:
	/**
	* @brief Check can mesh be clipped by plane
	* @param mesh checked mesh
	* @param plane clip plane 
	* @return true if can be clipped
	*/
	static bool canBeClipped(const geo::Mesh3d& mesh, const geo::Plane3d& plane);

	/**
	* @brief Clip mesh by plane
	* @param mesh Mesh to be clipped
	* @param plane clipping plane
	* @return pair of meshes, as clipping result
	*/
	static std::pair<geo::Mesh3d, geo::Mesh3d> clipMesh(
		const geo::Mesh3d& mesh, 
		const geo::Plane3d& plane);

	/**
	* @brief Check is triangle clipped by plane
	* @param pointsTri 3 points of triangle
	* @param plane clipping plane
	* @return Clipping status
	*/
	static geo::ClipResult triangleClipped(const ArrayPoint3f& pointsTri, const geo::Plane3d& plane);

	/**
	* @brief Clip triangle by plane
	* @param pointsTri 3 points of triangle
	* @param plane clipping plane
	* @return pair of triangle meshes
	*/
	static std::pair<geo::ArrayPoint3f, geo::ArrayPoint3f> clipTriangle(
		const ArrayPoint3f& pointsTri,
		const geo::Plane3d& plane);

	/**
	* @brief Check is line can be clipped by plane
	* @param va first vertex of line
	* @param vb second vertex of line
	* @param plane clipping plane
	* @param vOutClip clip result vertex
	* @param aPos true if vertex a is on positive side of plane
	* @param bPos true if vertex b is on positive side of plane
	* @return true if line clipped by plane
	*/
	static bool lineClipped(
		const geo::Point3f& va, 
		const geo::Point3f& vb, 
		const geo::Plane3d& plane, 
		geo::Point3f& vOutClip,
		bool& aPos,
		bool& bPos);

	/**
	* @brief Check mesh for unreferenced vertices
	* @param mesh Mesh to check
	* @return true if no unreferenced vertices found
	*/
	static bool checkTopology(const geo::Mesh3d& mesh);
};

} // namespace geo
#endif // MESHCLIP_H

