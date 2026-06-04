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
// meshtools.cpp
// Mesh fix unreferenced vertices
//
#include <cstdint>

#include "mesh3d.h"

#include "meshtools.h"

void geo::MeshTools::fixUnreferencedVertices(geo::Mesh3d& mesh)
{
	const auto numVertices = static_cast<uint32_t>(mesh.vertices_.size());
	constexpr int k2{ 2 };
	constexpr int k3{ 3 };
	const auto numTriangles = static_cast<int>(mesh.indices_.size()) / k3;

	// all triangle indices in range and 
	std::vector<int> verticesRefs(numVertices, 0);
	int j{ 0 };
	for (int t = 0; t < numTriangles; t++)
	{
		const uint32_t i0 = mesh.indices_[j++];
		const uint32_t i1 = mesh.indices_[j++];
		const uint32_t i2 = mesh.indices_[j++];
		if ((i0 >= numVertices) || (i1 >= numVertices) || (i2 >= numVertices))
		{
			return;
		}
		verticesRefs[i0] = 1;
		verticesRefs[i1] = 1;
		verticesRefs[i2] = 1;
	} // for t, all triangles

	const auto numRefs =
		static_cast<uint32_t>(std::count(verticesRefs.begin(), verticesRefs.end(), 1));

	if (numRefs == numVertices)
	{
		// each vertex has at least one reference from triangles
		return;
	}
	assert(numRefs < numVertices);
	std::vector<int> reindex(numVertices);
	std::vector<Point3f> verticesNew;
	std::vector<Point3f> normalsNew;

	int dst{ 0 };
	for (uint32_t src = 0; src < numVertices; src++)
	{
		if (verticesRefs[src] == 0)
		{
			reindex[src] = -1;
			continue;
		}
		reindex[src] = dst;
		dst++;
		verticesNew.push_back(mesh.vertices_[src]);
		if (mesh.normals_.size() == numVertices)
		{
			normalsNew.push_back(mesh.normals_[src]);
		}
	} // for src

	// replace old vertices with new one (only used by triangles)
	mesh.vertices_ = verticesNew;
	if (mesh.normals_.size() == numVertices)
	{
		mesh.normals_ = normalsNew;
	}

	j = 0;
	for (int t = 0; t < numTriangles; t++, j += k3)
	{
		const uint32_t i0 = mesh.indices_[j + 0];
		const uint32_t i1 = mesh.indices_[j + 1];
		const uint32_t i2 = mesh.indices_[j + k2];
		mesh.indices_[j + 0] = reindex[i0];
		mesh.indices_[j + 1] = reindex[i1];
		mesh.indices_[j + k2] = reindex[i2];
	} // for t, all triangles
}