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
// mesh3dbuilder.cpp
// Build Mesh3d by adding triangle one by one, and build indices
//
#include <cmath>
#include <cstdint>
#include <numbers> // NOLINT(misc-include-cleaner)

#include "geo.h"
#include "mesh3d.h"
#include "mesh3dbuilder.h"

namespace
{
	constexpr int kThree{ 3 };
} // namespace


void geo::Mesh3dBuilder::build3dMeshFromConcavePoints( // NOLINT(readability-convert-member-functions-to-static)
	const ArrayContour2f& points,
	Mesh3d& outMesh,
	float yMin, // NOLINT(bugprone-easily-swappable-parameters)
	float yMax)
{
	const auto numVertices = static_cast<int>(points.size());
	outMesh.vertices_.resize(static_cast<std::size_t>(numVertices) * 2);
	outMesh.normals_.resize(static_cast<std::size_t>(numVertices) * 2);
	for (int i = 0, j = 0; i < numVertices; i++)
	{
		outMesh.vertices_[j].x_ = yMin;
		outMesh.vertices_[j].y_ = points[i].y_;
		outMesh.vertices_[j].z_ = -points[i].x_;
		j++;

		outMesh.vertices_[j].x_ = yMax;
		outMesh.vertices_[j].y_ = points[i].y_;
		outMesh.vertices_[j].z_ = -points[i].x_;
		j++;
	}
	for (int i = 0; i < numVertices; i++)
	{
		const int iNext = (i + 1 < numVertices) ? (i + 1) : 0;
		const int iLo0 = i * 2;
		const int iHi0 = iLo0 + 1;
		const int iLo1 = iNext * 2;
		const int iHi1 = iLo1 + 1;

		outMesh.indices_.push_back(iHi0);
		outMesh.indices_.push_back(iLo0);
		outMesh.indices_.push_back(iLo1);

		outMesh.indices_.push_back(iLo1);
		outMesh.indices_.push_back(iHi1);
		outMesh.indices_.push_back(iHi0);
	}
	calcVertexNormalsByTriNormals(outMesh);
}

void geo::Mesh3dBuilder::build3dMeshFromPoints( // NOLINT(readability-convert-member-functions-to-static)
	const ArrayContour2f& points,
	geo::Mesh3d& outMesh)
{
	const auto numVertices = static_cast<int>(points.size());
	outMesh.vertices_.resize(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		// XY -> X0Y
		outMesh.vertices_[i].x_ = points[i].x_;
		outMesh.vertices_[i].y_ = 0.0F;
		outMesh.vertices_[i].z_ = points[i].y_;
	}

	// calculate normals
	// to do : reverse normals (if needed) to keep counterclockwise direction of points
	outMesh.normals_.resize(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		const int iPrev = (i > 0) ? (i - 1) : (numVertices - 1);
		const int iNext = (i + 1 < numVertices) ? (i + 1) : 0;
		geo::Point2f dir = points[iNext] - points[iPrev];
		dir.normalize();
		// here can be inverted direction
		outMesh.normals_[i] = geo::Point3f(-dir.y_, 0.0F, dir.x_);
	}
	// triangle indices
	const int numTriangles{ numVertices - 2 };
	outMesh.indices_.resize(static_cast<std::size_t>(numTriangles * kThree));
	int dst{ 0 };
	for (int t = 0; t < numTriangles; t++)
	{
		const int tNext{ t + 1 };
		const int tNextNext = (t + 2 < numVertices) ? (t + 2) : 0;
		outMesh.indices_[dst++] = 0;
		outMesh.indices_[dst++] = tNext;
		outMesh.indices_[dst++] = tNextNext;
	}
}

void geo::Mesh3dBuilder::translateMesh(const geo::Point3f& vAdd, geo::Mesh3d& outMesh)
{
	for (geo::Point3f& v : outMesh.vertices_)
	{
		v = v + vAdd;
	}
}

void geo::Mesh3dBuilder::createCube(float side, geo::Mesh3d& outMesh)
{
	// NOLINTBEGIN
	outMesh.vertices_.resize(8);
	outMesh.normals_.resize(8);
	outMesh.indices_ = {
		4, 5, 6, 4, 6, 7,
		0, 2, 1, 0, 3, 2,
		5, 1, 2, 5, 2, 6,
		6, 2, 3, 6, 3, 7,
		4, 7, 0, 7, 3, 0,
		5, 4, 1, 4, 0, 1
	};
	// NOLINTEND
	std::size_t i{ 0 };
	const float kFrontScale{ 1.0F };
	const float kXMin{ -side };
	//const float kXMin{ 0.1F };
	outMesh.vertices_[i++] = { kXMin, -side, -side };
	outMesh.vertices_[i++] = { kXMin, -side, side };
	outMesh.vertices_[i++] = { side, -side * kFrontScale, side * kFrontScale };
	outMesh.vertices_[i++] = { side, -side * kFrontScale, -side * kFrontScale };
	outMesh.vertices_[i++] = { kXMin, side, -side };
	outMesh.vertices_[i++] = { kXMin, side, side };
	outMesh.vertices_[i++] = { side, side * kFrontScale, side * kFrontScale };
	outMesh.vertices_[i++] = { side, side * kFrontScale, -side * kFrontScale };

	calcVertexNormalsByTriNormals(outMesh);
}

void geo::Mesh3dBuilder::createAxis(const geo::Point3f& vLookTo, geo::Mesh3d& outMesh)
{
	// NOLINTBEGIN
	outMesh.vertices_.resize(4);

	const float xMax = std::max(std::max(vLookTo.x_, vLookTo.y_), vLookTo.z_);
	const float xMin{ xMax * 0.01F };
	constexpr float kSmall{ 0.04F };
	outMesh.vertices_[0] = { xMax, 0.0F, 0.0F };
	outMesh.vertices_[1] = { xMin, 0.0F, -kSmall };
	outMesh.vertices_[2] = { xMin, kSmall, kSmall };
	outMesh.vertices_[3] = { xMin, -kSmall, kSmall };
	// NOLINTEND

	constexpr float kCloseToOne{ 0.9F };
	// rotate vertices for Y and Z axis
	if (vLookTo.y_ >= kCloseToOne)
	{
		const auto numVertices = static_cast<int>(outMesh.vertices_.size());
		for (int i = 0; i < numVertices; i++)
		{
			const geo::Point3f v{ outMesh.vertices_[i] };
			outMesh.vertices_[i] = { -v.y_ , v.x_, v.z_ };
		}
	}
	if (vLookTo.z_ >= kCloseToOne)
	{
		const auto numVertices = static_cast<int>(outMesh.vertices_.size());
		for (int i = 0; i < numVertices; i++)
		{
			const geo::Point3f v{ outMesh.vertices_[i] };
			outMesh.vertices_[i] = { -v.z_ , v.y_, v.x_ };
		}
	}


	// NOLINTBEGIN
	outMesh.indices_ = {
		2, 3, 0,
		1, 2, 0,
		3, 1, 0,
		1, 3, 2
	};

	// build mesh normals from vertices and triangle indices
	outMesh.normals_.resize(4);
	// NOLINTEND
	calcVertexNormalsByTriNormals(outMesh);

}

void geo::Mesh3dBuilder::createCylinder(float radius, float height, int numSides, geo::Mesh3d& outMesh) // NOLINT(bugprone-easily-swappable-parameters)
{
	outMesh.vertices_.resize(static_cast<std::size_t>(numSides) * 2);
	outMesh.normals_.resize(static_cast<std::size_t>(numSides) * 2);

	const float h2 = height * 0.5F;
	constexpr float kGradToRad{ std::numbers::pi_v<float> / 180.0F }; // NOLINT(cppcoreguidelines-init-variables)

	int j{ 0 };
	for (int i = 0; i < numSides; i++)
	{
		const int angleGrad = i * 360 / numSides;
		const auto angleFloat = static_cast<float>(angleGrad);
		const float angleRad{ angleFloat * kGradToRad };
		const float z = radius * cosf(angleRad);
		const float y = radius * sinf(angleRad);
		outMesh.vertices_[j++] = { -h2, y, z };
		outMesh.vertices_[j++] = { +h2, y, z };
	}
	for (int i = 0; i < numSides; i++)
	{
		const int iNext = (i + 1 < numSides) ? (i + 1) : 0;
		const int iLo0 = i * 2;
		const int iHi0 = iLo0 + 1;
		const int iLo1 = iNext * 2;
		const int iHi1 = iLo1 + 1;
		outMesh.indices_.push_back(iLo0);
		outMesh.indices_.push_back(iHi0);
		outMesh.indices_.push_back(iHi1);

		outMesh.indices_.push_back(iHi1);
		outMesh.indices_.push_back(iLo1);
		outMesh.indices_.push_back(iLo0);
	}
	calcVertexNormalsByTriNormals(outMesh);
}

void geo::Mesh3dBuilder::calcVertexNormalsByTriNormals(geo::Mesh3d& mesh)
{
	const auto numTriangles = static_cast<int>(mesh.indices_.size()) / kThree;
	std::vector<geo::Point3f> triNormals(numTriangles);

	for (int t = 0, j = 0; t < numTriangles; t++, j += kThree)
	{
		const uint32_t i0 = mesh.indices_[j + 0];
		const uint32_t i1 = mesh.indices_[j + 1];
		const uint32_t i2 = mesh.indices_[j + 2];
		const geo::Point3f v0 = mesh.vertices_[i0];
		const geo::Point3f v1 = mesh.vertices_[i1];
		const geo::Point3f v2 = mesh.vertices_[i2];
		geo::Point3f d01 = v1 - v0;
		geo::Point3f d12 = v2 - v1;
		d01.normalize();
		d12.normalize();
		geo::Point3f normTri = crossProduct(d01, d12);
		normTri.normalize();
		triNormals[t] = normTri;
	} // for t

	// assign ave tri normals to each vertex
	const auto numVertices = static_cast<int>(mesh.vertices_.size());

	mesh.normals_.resize(numVertices);

	for (int i = 0; i < numVertices; i++)
	{
		geo::Point3f normal{ 0.0F, 0.0F, 0.0F };
		int numSharedTri{ 0 };
		for (int t = 0, j = 0; t < numTriangles; t++, j += kThree)
		{
			const int i0 = static_cast<int>(mesh.indices_[j + 0]);
			const int i1 = static_cast<int>(mesh.indices_[j + 1]);
			const int i2 = static_cast<int>(mesh.indices_[j + 2]);
			if ((i0 == i) || (i1 == i) || (i2 == i))
			{
				normal = normal + triNormals[t];
				numSharedTri++;
			}
		}
		// It can be isolated vertex with no one triangle, with reference to it.
		// After some clipping, new triangle can be invalid (have two vertices in
		// the same position)
		// 
		//assert(numSharedTri > 0);
		if (numSharedTri > 0)
		{
			normal.scale(1.0F / static_cast<float>(numSharedTri));
		}
		mesh.normals_[i] = normal;
	} // for i
}
