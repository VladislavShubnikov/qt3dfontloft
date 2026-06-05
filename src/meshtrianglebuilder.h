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
// meshtrianglebuilder.h
// Build mesh by adding triangles (after clipping)
//
#ifndef MESHTRIANGLEBUILDER_H
#define MESHTRIANGLEBUILDER_H

#include <vector>
#include "mesh3d.h"

namespace geo
{

/**
* @class MeshTriangleBuilder
* @brief Create mesh from triangles, one by one
*
* @note Shared vertices are taken into account. Produced mesh has no duplicated vertices.
*
*/
class MeshTriangleBuilder
{
public:
	explicit MeshTriangleBuilder(geo::Mesh3d& mesh);
	~MeshTriangleBuilder() = default;
	MeshTriangleBuilder(const MeshTriangleBuilder&) = default;
	MeshTriangleBuilder(MeshTriangleBuilder&&) = default;
	MeshTriangleBuilder& operator=(const MeshTriangleBuilder&) = default;
	MeshTriangleBuilder& operator=(MeshTriangleBuilder&&) = default;

	/**
	* @brief Add triangle to mesh
	* @param triVerts added triangle vertices
	*/
	void addTriangle(const std::vector<geo::Point3f>& triVerts);

private:
	[[nodiscard]] int findVertex(const geo::Point3f& p) const;
	[[nodiscard]] int addVertex(const geo::Point3f& p);

	std::reference_wrapper<geo::Mesh3d> mesh_;

}; // class MeshTriangleBuilder

}  // namespace geo

#endif // MESHTRIANGLEBUILDER_H
