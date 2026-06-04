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
// meshclip.cpp
// Clipping of the Mesh3d
//
#include <cstdlib>
#include <cstdint>
#include <utility>
#include "geo.h"
#include "mesh3d.h"
#include "mesh3dbuilder.h"
#include "meshtools.h"
#include "plane3d.h"
#include "meshtrianglebuilder.h"

#include "meshclip.h"


bool geo::Mesh3dClip::checkTopology(const geo::Mesh3d& mesh)
{
	const auto numVertices = static_cast<uint32_t>(mesh.vertices_.size());
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
			return false;
		}
		verticesRefs[i0] = 1;
		verticesRefs[i1] = 1;
		verticesRefs[i2] = 1;
		// check all vertices are not same
		const geo::Point3f p0{ mesh.vertices_[i0] };
		const geo::Point3f p1{ mesh.vertices_[i1] };
		const geo::Point3f p2{ mesh.vertices_[i2] };
		const float d01 = (p0 - p1).squaredNorm();
		const float d12 = (p1 - p2).squaredNorm();
		const float d02 = (p0 - p2).squaredNorm();
		constexpr float kSmall{ 1.0e-12F };
		if ((d01 < kSmall) || (d12 < kSmall) || (d02 < kSmall))
		{
			return false;
		}
	}
	const auto numRefs = 
		static_cast<uint32_t>(std::count(verticesRefs.begin(), verticesRefs.end(), 1));
	return (numRefs == numVertices);
}

bool geo::Mesh3dClip::canBeClipped(const geo::Mesh3d& mesh, const geo::Plane3d& plane)
{
	const auto numVertices = static_cast<int>(mesh.vertices_.size());
	int numPos{ 0 };
	int numNeg{ 0 };
	for (int i = 0; i < numVertices; i++)
	{
		const geo::Point3f& v{ mesh.vertices_[i] };
		const geo::Point3f pToV{ v - plane.point_ };
		const float dot{ pToV.dotProduct(plane.normal_) };
		constexpr float kSmall{ 1.0e-12F };
		numPos += (dot > kSmall) ? 1 : 0;
		numNeg += (dot < -kSmall) ? 1 : 0;
	}
	return ((numPos > 0) && (numNeg > 0));
}

geo::ClipResult 
geo::Mesh3dClip::triangleClipped( // NOLINT(readability-convert-member-functions-to-static)
	const std::vector<geo::Point3f>& pointsTri, 
	const geo::Plane3d& plane)
{
	int numPos{ 0 };
	int numNeg{ 0 };
	constexpr int k3{ 3 };
	for (int i = 0; i < k3; i++)
	{
		const geo::Point3f& v{ pointsTri[i] };
		const geo::Point3f pToV{ v - plane.point_ };
		const float dot{ pToV.dotProduct(plane.normal_) };
		constexpr float kSmall{ 1.0e-12F };
		numPos += (dot > kSmall) ? 1 : 0;
		numNeg += (dot < -kSmall) ? 1 : 0;
	}
	if ((numPos > 0) && (numNeg == 0))
	{
		return ClipResult::kOnPositiveSide;
	}
	if ((numNeg > 0) && (numPos == 0))
	{
		return ClipResult::kOnNegativeSide;
	}
	return ClipResult::kClipped;
}

std::pair<geo::Mesh3d, geo::Mesh3d> 
geo::Mesh3dClip::clipMesh(
	const geo::Mesh3d& mesh,
	const geo::Plane3d& plane)
{
	constexpr int k2{ 2 };
	constexpr int k3{ 3 };
	constexpr int k4{ 4 };

	const auto numTriangles = static_cast<int>(mesh.indices_.size()) / k3;

	geo::Mesh3d meshPos;
	geo::Mesh3d meshNeg;
	geo::MeshTriangleBuilder builderPos(meshPos); // NOLINT(misc-const-correctness)
	geo::MeshTriangleBuilder builderNeg(meshNeg); // NOLINT(misc-const-correctness)

	for (int t = 0, j = 0; t < numTriangles; t++, j += k3)
	{
		const auto i0 = mesh.indices_[j + 0];
		const auto i1 = mesh.indices_[j + 1];
		const auto i2 = mesh.indices_[j + k2];
		const geo::ArrayPoint3f triPoints
		{
			mesh.vertices_[i0],
			mesh.vertices_[i1],
			mesh.vertices_[i2]
		};
		const geo::ClipResult clipResult = triangleClipped(triPoints, plane);
		if (clipResult == ClipResult::kOnPositiveSide)
		{
			builderPos.addTriangle(triPoints);
		}
		else if (clipResult == ClipResult::kOnNegativeSide)
		{
			builderNeg.addTriangle(triPoints);
		}
		else
		{
			// clip tringle by plane
			std::pair<geo::ArrayPoint3f, geo::ArrayPoint3f> pairRes = clipTriangle(triPoints, plane);
			geo::ArrayPoint3f& pointsPos = pairRes.first;
			geo::ArrayPoint3f& pointsNeg = pairRes.second;

			if (pointsPos.size() == k3)
			{
				builderPos.addTriangle(pointsPos);
			}
			else
			{
				assert(pointsPos.size() == k4);
				builderPos.addTriangle(pointsPos);

				std::vector<geo::Point3f> pts{
					pointsPos[0], pointsPos[k2], pointsPos[k3]
				};
				builderPos.addTriangle(pts);
			}

			if (pointsNeg.size() == k3)
			{
				builderNeg.addTriangle(pointsNeg);
			}
			else
			{
				assert(pointsNeg.size() == k4);
				builderNeg.addTriangle(pointsNeg);

				std::vector<geo::Point3f> pts{
					pointsNeg[0], pointsNeg[k2], pointsNeg[k3]
				};
				builderNeg.addTriangle(pts);
			}
		}

	} // for t, all triangles

	// get vertices and indices from builders and create 2 meshes for output
	// ...

	geo::Mesh3dBuilder::calcVertexNormalsByTriNormals(meshPos);
	geo::Mesh3dBuilder::calcVertexNormalsByTriNormals(meshNeg);

	// debug
	bool ok1 = checkTopology(meshPos);
	bool ok2 = checkTopology(meshNeg);

	if (!ok1)
	{
		geo::MeshTools::fixUnreferencedVertices(meshPos);
		ok1 = checkTopology(meshPos);
	}
	if (!ok2)
	{
		geo::MeshTools::fixUnreferencedVertices(meshNeg);
		ok2 = checkTopology(meshNeg);
	}
	assert(ok1);
	assert(ok2);

	return {std::move(meshPos), std::move(meshNeg)};
}

std::pair<geo::ArrayPoint3f, geo::ArrayPoint3f> 
geo::Mesh3dClip::clipTriangle(
	const ArrayPoint3f& pointsTri,
	const geo::Plane3d& plane)
{
	std::pair<geo::ArrayPoint3f, geo::ArrayPoint3f> out;
	
	constexpr int k3{ 3 };
	constexpr float kSmall{ 1.0e-12F };
	int iPos{ 0 };
	for (; iPos < k3; iPos++)
	{
		const geo::Point3f pToV{ pointsTri[iPos] - plane.point_ };
		const float dot{ pToV.dotProduct(plane.normal_) };
		if (dot > kSmall)
		{
			break;
		}
	}
	// not found vertex from plane positive side: should not happen for clipper triangle
	assert(iPos < k3);

	for (int it = 0; it < k3; it++)
	{
		const int i = (iPos + it) % k3;
		const int iNext = (iPos + it + 1) % k3;
		const geo::Point3f& v = pointsTri[i];
		const geo::Point3f& vNext = pointsTri[iNext];
		geo::Point3f vClip;
		bool curPos{ false };
		bool nextPos{ false };
		const bool hasClip = lineClipped(v, vNext, plane, vClip, curPos, nextPos);
		if (!hasClip)
		{
			if (curPos)
			{
				out.first.push_back(v);
			}
			continue;
		}
		if (curPos)
		{
			out.first.push_back(v);
		}
		out.first.push_back(vClip);
	}


	iPos = 0;
	for (; iPos < k3; iPos++)
	{
		const geo::Point3f pToV{ pointsTri[iPos] - plane.point_ };
		const float dot{ pToV.dotProduct(plane.normal_) };
		if (dot < -kSmall)
		{
			break;
		}
	}
	// not found vertex from plane negative side: should not happen for clipped triangle
	assert(iPos < k3);

	for (int it = 0; it < k3; it++)
	{
		const int i = (iPos + it) % k3;
		const int iNext = (iPos + it + 1) % k3;
		const geo::Point3f& v = pointsTri[i];
		const geo::Point3f& vNext = pointsTri[iNext];
		geo::Point3f vClip;
		bool curPos{ false };
		bool nextPos{ false };

		const bool hasClip = lineClipped(v, vNext, plane, vClip, curPos, nextPos);
		if (!hasClip)
		{
			if (!curPos)
			{
				out.second.push_back(v);
			}
			continue;
		}
		if (!curPos)
		{
			out.second.push_back(v);
		}
		out.second.push_back(vClip);
	}

	return out;
}

bool geo::Mesh3dClip::lineClipped(
	const geo::Point3f& va, 
	const geo::Point3f& vb, 
	const geo::Plane3d& plane, 
	geo::Point3f& vOutClip,
	bool& aPos,
	bool& bPos)
{
	const geo::Point3f aToP{ va - plane.point_ };
	const geo::Point3f bToP{ vb - plane.point_ };
	const float aDot = aToP.dotProduct(plane.normal_);
	const float bDot = bToP.dotProduct(plane.normal_);
	constexpr float kSmall{ 1.0e-12F };

	aPos = aDot > kSmall;
	bPos = bDot > kSmall;

	const bool hasNeg = (aDot < -kSmall) || (bDot < -kSmall);
	const bool hasPos = (aDot > kSmall) || (bDot > kSmall);

	if (!hasNeg || !hasPos)
	{
		return false;
	}
	// clipping detected here:
	const geo::Point3f pToA{ plane.point_ - va};
	const geo::Point3f bToA{ vb - va };
	const float up = pToA.dotProduct(plane.normal_);
	const float lo = bToA.dotProduct(plane.normal_);
	assert(std::abs(lo) > kSmall);
	const float t{ up / lo };
	vOutClip.x_ = va.x_ + ((vb.x_ - va.x_) * t);
	vOutClip.y_ = va.y_ + ((vb.y_ - va.y_) * t);
	vOutClip.z_ = va.z_ + ((vb.z_ - va.z_) * t);

	return true;
}
