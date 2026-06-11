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
// contoursplitter.cpp
// Geometry operations with 2d contour(s)
//

#include <stack> // NOLINT
#include <limits>
#include <cstdlib>

#include "geo.h"

#include "contoursplitter.h"

namespace
{
bool linesIntersected(
	const geo::Point2f& a0,
	const geo::Point2f& a1,
	const geo::Point2f& b0,
	const geo::Point2f& b1)
{
	const geo::Point2f vA{ a1 - a0 };
	const geo::Point2f normA(-vA.y_, vA.x_);
	const float dotA0 = (b0 - a0).dotProduct(normA);
	const float dotA1 = (b1 - a0).dotProduct(normA);
	const bool differentByA = (dotA0 * dotA1 < 0.0F);

	const geo::Point2f vB{ b1 - b0 };
	const geo::Point2f normB(-vB.y_, vB.x_);
	const float dotB0 = (a0 - b0).dotProduct(normB);
	const float dotB1 = (a1 - b0).dotProduct(normB);
	const bool differentByB = (dotB0 * dotB1 < 0.0F);
	return (differentByA && differentByB);
}

} // namespace

geo::ContourSplitter::ContourSplitter(const geo::ArrayContour2f& points, const SplitMethod split)
	:pointsSrc_(points)
	,splitMethod_(split)
{
}

geo::ArrayContours2f geo::splitToConvexContours(const geo::ArrayContour2f& points, const SplitMethod split) // NOLINT
{
	geo::ArrayContour2f pointsTemp{ points };
	geo::ContourSplitter::makePointsCounterClockwise(pointsTemp);

	if (!geo::ContourSplitter::pointsConvex(pointsTemp))
	{
		geo::ContourSplitter splitter(pointsTemp, split);
		geo::ArrayContours2f contours = splitter.splitConcaveContour();
		return contours;
	}
	// this contour is convex
	geo::ArrayContours2f out;
	out.push_back(pointsTemp);
	return out;
}

void geo::ContourSplitter::makePointsCounterClockwise(std::vector<geo::Point2f>& points)  // NOLINT
{
	const auto numPoints = static_cast<int>(points.size());
	constexpr int kThree{ 3 };
	if (numPoints < kThree)
	{
		return;
	}

	if (numPoints == kThree)
	{
		return;
	}

	// search for first edge with all other points of poly
	// are from one side
	int is{ 0 };
	bool found{ false };
	for (is = 0; is < numPoints - 1; is++)
	{
		const int isNext{ is + 1 };
		if (allPointsFromOneSide(points, is, isNext))
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		// this is degenerate polygone
		assert(found);
		return;
	}

	// search for the first non-parallel line segments
	found = false;
	float dotSignStart{ 0.0F };
	for (; (is < numPoints - 1) && !found; is++)
	{
		const int iNext = is + 1;
		const int iNextNext = (is + 2 < numPoints) ? (is + 2) : 0;
		const geo::Point2f dir0 = points[iNext] - points[is];
		const geo::Point2f dir1 = points[iNextNext] - points[iNext];
		const geo::Point2f normal0{ -dir0.y_ , dir0.x_ };
		const float dot = normal0.dotProduct(dir1);
		constexpr float kSmall{ 1.0e-8F };
		if (std::abs(dot) < kSmall)
		{
			// i -> (i + 1) -> (i + 2) is bad line sequence: this is a single line
			continue;
		}
		dotSignStart = dot;
		found = true;
	} // for is
	if (!found)
	{
		return;
	}

	if (dotSignStart > 0.0F)
	{
		// already in counterclockwise
		return;
	}

	// invert points to give them CCW direction
	std::reverse(points.begin(), points.end());
}

bool geo::ContourSplitter::pointsConvex(const std::vector<geo::Point2f>& points)  // NOLINT
{
	const auto numPoints = static_cast<int>(points.size());
	constexpr int kThree{ 3 };
	if (numPoints < kThree)
	{
		return false;
	}

	if (numPoints == kThree)
	{
		return true;
	}

	// search for first edge with all other points of poly
	// are from one side
	int is{ 0 };
	bool found{ false };
	for (is = 0; is < numPoints - 1; is++)
	{
		const int isNext{ is + 1 };
		if (allPointsFromOneSide(points, is, isNext))
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		// this is degenerate polygone
		assert(found);
		return false;
	}

	// search for the first non-parallel line segments
	found = false;
	float dotSignStart{ 0.0F };

	for (; (is < numPoints - 1) && !found; is++)
	{
		const int iNext = is + 1;
		const int iNextNext = (is + 2 < numPoints) ? (is + 2) : 0;
		const geo::Point2f dir0 = points[iNext] - points[is];
		const geo::Point2f dir1 = points[iNextNext] - points[iNext];
		const geo::Point2f normal0{ -dir0.y_ , dir0.x_ };
		const float dot = normal0.dotProduct(dir1);
		constexpr float kSmall{ 1.0e-8F };
		if (std::abs(dot) < kSmall)
		{
			// i -> (i + 1) -> (i + 2) is bad line sequence: this is a single line
			continue;
		}
		dotSignStart = dot;
		found = true;
	} // for is

	if (!found)
	{
		// this is degenerate polygone
		assert(found);
		return false;
	}

	for (int i = 0; i < numPoints - 1; i++)
	{
		const int i0 = (is + i + 0) % numPoints;
		const int i1 = (is + i + 1) % numPoints;
		const int i2 = (is + i + 2) % numPoints;
		const geo::Point2f dir0 = points[i1] - points[i0];
		const geo::Point2f dir1 = points[i2] - points[i1];
		const geo::Point2f normal0{ -dir0.y_ , dir0.x_ };
		const float dotSign = normal0.dotProduct(dir1);
		if (dotSignStart * dotSign < 0.0F)
		{
			return false;
		}
	} // for i, all points

	return true;
}

bool geo::ContourSplitter::allPointsFromOneSide(  // NOLINT
	const geo::ArrayContour2f& points,  // NOLINT
	const int iCur,
	const int iNext)
{
	const auto numPoints = static_cast<int>(points.size());
	const geo::Point2f dir0 = points[iNext] - points[iCur];
	// normal is dir0 , turned by 90 degrees counterclockwise
	const geo::Point2f normal0{ -dir0.y_ , dir0.x_ };

	bool posSignFound{ false };
	bool negSignFound{ false };

	for (int i = 0; i < numPoints; i++)
	{
		// do not compare with this edge ends
		if ((i == iCur) && (i == iNext))
		{
			continue;
		}
		const geo::Point2f dir = points[i] - points[iCur];
		const float dot = normal0.dotProduct(dir);
		constexpr float kSmall{ 1.0e-8F };
		if (dot > kSmall)
		{
			posSignFound = true;
		}
		if (dot < -kSmall)
		{
			negSignFound = true;
		}
	} // for i, all points

	if (!posSignFound && !negSignFound)
	{
		// strange degenerated into line polygone
		return false;
	}
	if (posSignFound && negSignFound)
	{
		return false;
	}
	// one of them is true
	return true;
}

bool geo::ContourSplitter::vertexConvex(  // NOLINT
	const std::vector<geo::Point2f>& points,  // NOLINT
	int iCur)
{
	const auto n = static_cast<int>(points.size());
	const int iPrev = (iCur > 0) ? (iCur - 1) : (n - 1);
	const int iNext = (iCur + 1 < n) ? (iCur + 1) : 0;

	const geo::Point2f dir0 = points[iCur] - points[iPrev];
	const geo::Point2f dir1 = points[iNext] - points[iCur];
	const geo::Point2f normal0{ -dir0.y_ , dir0.x_ };
	const float dot = normal0.dotProduct(dir1);
	// if dot product is positive then vertex convex
	constexpr float kSmall{ 1.0e-8F };
	return dot > kSmall;
}

bool geo::ContourSplitter::edgeIntersectsPoly(const geo::ArrayContour2f& poly, int i0, int i1) // NOLINT
{
	const auto numVertices = static_cast<int>(poly.size());
	const geo::Point2f e0(poly[i0]);
	const geo::Point2f e1(poly[i1]);
	for (int i = 0; i < numVertices; i++)
	{
	  const int iNext = (i + 1 < numVertices) ? (i + 1) : 0;
	  if ((i == i0) || (i == i1) || (iNext == i0) || (iNext == i1))
	  {
		continue;
	  }
	  const geo::Point2f p0(poly[i]);
	  const geo::Point2f p1(poly[iNext]);
	  if (linesIntersected(e0, e1, p0, p1))
	  {
		return true;
	  }

	} // for i
	return false;
}

//! input: polygon is concave, and in CCW vertex order
std::pair<geo::ArrayContour2f, geo::ArrayContour2f>
geo::ContourSplitter::splitPolyInTwo(const geo::ArrayContour2f& poly) // NOLINT
{
	const auto n = static_cast<int>(poly.size());

	std::pair<geo::ArrayContour2f, geo::ArrayContour2f> out;
	float minBridgeLen{ std::numeric_limits<float>::max() };
	int indexCut0{ -1 };
	int indexCut1{ -1 };

	for (int i = 0; i < n; i++)
	{
		// new possible edge should be started from convex vertex
		// this check is performed in vertexConvex(poly, i)
		if (!vertexConvex(poly, i))
		{
			continue;
		}
		// 
		// find all possible edges between i-th vertex and any others,
		// with except for neighbour vertices
		const int iPrev = (i > 0) ? (i - 1) : (n - 1);
		const int iNext = (i + 1 < n) ? (i + 1) : 0;
		for (int j = 0; j < n; j++)
		{
			if ((j == i) || (j == iPrev) || (j == iNext))
			{
				continue;
			}
			if (edgeInside(poly, iPrev, i, iNext, j) && !edgeIntersectsPoly(poly, i, j))
			{
				const float d = (poly[i] - poly[j]).squaredNorm();
				if (d < minBridgeLen)
				{
					minBridgeLen = d;
					indexCut0 = std::min(i, j);
					indexCut1 = std::max(i, j);
				}
			}
		} // for j

	}
	// if == -1, no one bridge is found in poly: some strange logic in poly...
	assert(indexCut0 >= 0);

	int i{ indexCut0 };
	while (i != indexCut1)
	{
		out.first.push_back(poly[i]);
		// next vertex
		i++;
		i = (i == n) ? 0 : i;
	}
	// add last
	if (i != -1)
	{
		out.first.push_back(poly[i]);
	}

	// second poly
	i = indexCut1;
	while (i != indexCut0)
	{
		out.second.push_back(poly[i]);
		// next vertex
		i++;
		i = (i == n) ? 0 : i;
	}
	// add last
	out.second.push_back(poly[i]);

	return out;
}

// NOLINTBEGIN
bool geo::ContourSplitter::edgeInside(
	const std::vector<geo::Point2f>& points,
	int iPrev,
	int iCur,
	int iNext,
	int j)
	// NOLINTEND
{
	const geo::Point2f dir0 = points[iCur] - points[iPrev];
	const geo::Point2f dir1 = points[iNext] - points[iCur];
	const geo::Point2f curToJ = points[j] - points[iCur];
	const geo::Point2f normal0{ -dir0.y_ , dir0.x_ };
	constexpr float kSmall{ 1.0e-8F };
	if (curToJ.dotProduct(normal0) < -kSmall)
	{
		return false;
	}
	const geo::Point2f normal1{ -dir1.y_ , dir1.x_ };
	return curToJ.dotProduct(normal1) >= -kSmall;
}

geo::ArrayContours2f geo::ContourSplitter::splitConcaveContour()
{
	switch (splitMethod_)
	{
	case SplitMethod::SHORTEST_BRIDGE: // NOLINT
		return splitConcaveContourByShortestBridge();
	case SplitMethod::TRIANGULATION:
		return splitConcaveContourByTriangulation();
	} // switch
	// should not be here
	assert(splitMethod_ == SplitMethod::SHORTEST_BRIDGE);
	geo::ArrayContours2f poly;
	return poly;
}


//! Input points is concave, counter-clockwise direction
//! Output: array of convex contours
geo::ArrayContours2f
geo::ContourSplitter::splitConcaveContourByShortestBridge()  // NOLINT
{
	std::stack<geo::ArrayContour2f> stackPolys;
	stackPolys.push(pointsSrc_);
	geo::ArrayContours2f out;
	while (!stackPolys.empty())
	{
		geo::ArrayContour2f poly = stackPolys.top();
		stackPolys.pop();
		if (pointsConvex(poly))
		{
			out.push_back(poly);
			continue;
		}
		// poly is concave contour:
		// split poly into two parts
		std::pair<geo::ArrayContour2f, geo::ArrayContour2f> two = splitPolyInTwo(poly);
		stackPolys.push(two.first);
		stackPolys.push(two.second);

	} // while
	return out;
}

int 
geo::ContourSplitter::findFarVertexRelateToLine( // NOLINT
	const geo::ArrayContour2f& poly, 
	const geo::Point2f& vBaseStart, 
	const geo::Point2f& vBaseEnd)
{
	const geo::Point2f vLook{ vBaseEnd  - vBaseStart };
	const geo::Point2f vNormal(-vLook.y_, vLook.x_);
	const auto numVertices = static_cast<int>(poly.size());
	int iFar{0};
	float maxDot{ 0.0F };
	for (int i = 0; i < numVertices; i++)
	{
		const geo::Point2f vDif{ poly[i] - vBaseStart };
		constexpr float kSmall{1.0e-10F};
		if (vDif.squaredNorm() > kSmall)
		{
			const float dot = std::abs(vDif.dotProduct(vNormal));
			if (dot > maxDot)
			{
				maxDot = dot;
				iFar = i;
			}
		}
	} // for i
	return iFar;
}

//! Detach single triangle with vertex, defined by index (and two adjacent vertices)
std::pair<geo::ArrayContour2f, geo::ArrayContour2f> 
geo::ContourSplitter::detachTriangle( // NOLINT
	const geo::ArrayContour2f& poly, // NOLINT
	int index)
{
	const auto numVertices = static_cast<int>(poly.size());
	const int iPrev = (index - 1 >= 0) ? (index - 1) : (numVertices - 1);
	const int iNext = (index + 1 < numVertices) ? (index + 1) : 0;

	geo::ArrayContour2f tri;
	tri.push_back(poly[iPrev]);
	tri.push_back(poly[index]);
	tri.push_back(poly[iNext]);

	geo::ArrayContour2f polyCut;
	// from index next to to index prev, with increment
	int i = iNext;
	while (i != iPrev)
	{
		polyCut.push_back(poly[i]);
		i++;
		if (i >= numVertices)
		{
			i = 0;
		}
	}
	polyCut.push_back(poly[iPrev]);

	return { polyCut , tri };
}



//! Input points is concave, counter-clockwise direction
//! Output: array of convex contours
geo::ArrayContours2f
geo::ContourSplitter::splitConcaveContourByTriangulation() // NOLINT
{
	geo::ArrayContours2f out;

	// find "base" edge: longest edge with all points from one side from it
	int iBaseStart{0};
	int iBaseEnd{ 0 };
	const auto numVertices = static_cast<int>(pointsSrc_.size());
	float distMax{0.0F};

	for (int i = 0; i < numVertices; i++)
	{
		const int iNext = (i + 1 < numVertices) ? (i + 1) : 0;
		if (!allPointsFromOneSide(pointsSrc_, i, iNext))
		{
			continue;
		}
		const geo::Point2f dif{ pointsSrc_[iNext] - pointsSrc_[i]};
		const float dist2 = dif.squaredNorm();
		if (dist2 > distMax)
		{
			distMax = dist2;
			iBaseStart = i;
			iBaseEnd = iNext;
		}
	}
	const geo::Point2f vBaseStart = pointsSrc_[iBaseStart];
	const geo::Point2f vBaseEnd = pointsSrc_[iBaseEnd];

	std::stack<geo::ArrayContour2f> stackPolys;
	stackPolys.push(pointsSrc_);
	while (!stackPolys.empty())
	{
		geo::ArrayContour2f poly = stackPolys.top();
		stackPolys.pop();
		if (pointsConvex(poly))
		{
			out.push_back(poly);
			continue;
		}
		const int indexFar{ findFarVertexRelateToLine(poly, vBaseStart, vBaseEnd) }; // NOLINT
		std::pair<geo::ArrayContour2f, geo::ArrayContour2f> two = detachTriangle(poly, indexFar);
		stackPolys.push(two.first);
		stackPolys.push(two.second);

	} // while stack full


	return out;
}
