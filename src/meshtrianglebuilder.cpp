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
// meshtrianglebuilder.cpp
// Build mesh by adding triangles (after clipping)
//

#include <array>

#include "geo.h"
#include "mesh3d.h"

#include "meshtrianglebuilder.h"

geo::MeshTriangleBuilder::MeshTriangleBuilder(geo::Mesh3d& mesh)
	:mesh_(mesh)
{
}

int geo::MeshTriangleBuilder::findVertex(const geo::Point3f& p) const // NOLINT(readability-convert-member-functions-to-static)
{
	const auto numVertices = static_cast<int>(mesh_.get().vertices_.size());
	constexpr float kSmallValue{ 1.0e-8F };
	for (int i = 0; i < numVertices; i++)
	{
		const auto vecDif{ mesh_.get().vertices_[i] - p };
		const float d2{ vecDif.squaredNorm() };
		if (d2 < kSmallValue)
		{
			return i;
		}
	}
	return -1;
}

int geo::MeshTriangleBuilder::addVertex(const geo::Point3f& p)
{
	mesh_.get().vertices_.push_back(p);
	return static_cast<int>(mesh_.get().vertices_.size()) - 1;
}

void geo::MeshTriangleBuilder::addTriangle(const std::vector<geo::Point3f>& triVertices)
{
	constexpr int k2{ 2 };
	constexpr int k3{ 3 };
	std::array<int, k3> indices{};

	for (int i = 0; i < k3; i++)
	{
		int ind = findVertex(triVertices[i]);
		if (ind < 0)
		{
			ind = addVertex(triVertices[i]);
		}
		indices[i] = ind;
	}
	if ((indices[0] == indices[1]) || (indices[0] == indices[k2]) || (indices[1] == indices[k2]))
	{
		// degenerated triangle: this is hust line
		return;
	}
	mesh_.get().indices_.push_back(indices[0]);
	mesh_.get().indices_.push_back(indices[1]);
	mesh_.get().indices_.push_back(indices[k2]);
}

