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
// contoursplitter.h
// Geometry operations with 2d contour(s)
//
#ifndef CONTOURSPLITTER_H
#define CONTOURSPLITTER_H

#include "contour.h"

namespace geo
{

/**
* How 2D contour can be split: method selection
*/
enum class SplitMethod: int
{
    //! By find shortest bridge edge inside contour
    SHORTEST_BRIDGE = 0,
    //! By detaching triangles one be one
    TRIANGULATION = 1,
};

/**
* @brief Split concave contour into set of convex contours
* @note this operation if performed only if contour is concave.
* Please see @ref splitConcaveContour for more details
* @param points source points of concave contour
*/
ArrayContours2f splitToConvexContours(const geo::ArrayContour2f& points, const SplitMethod split = SplitMethod::SHORTEST_BRIDGE);

/**
 * @class ContourSplitter
 * @brief Split concave contour into set of convex ones
 */
class ContourSplitter
{
public:
    /**
    * @brief Init splitter with source contour
    * @param points source points of any contour(convex/concave, cw/ccw)
    * @param split method to split contour, see @ref SplitMethod
    */
    ContourSplitter(const geo::ArrayContour2f& points, const SplitMethod split = SplitMethod::SHORTEST_BRIDGE);

    /**
    * @brief Split concave contour into set of convex contours
    * @note this operation if performed only if contour is concave.
    * Please see @ref splitConcaveContour for more details
    * @param points source points of concave contour
    */
    // ArrayContours2f splitToConvexContours();

    /**
    * @brief Check is contour points convex
    * @param points source points of contour
    */
    static bool pointsConvex(const geo::ArrayContour2f& points);

    /**
    * @brief Invert vertices order in contour (if need) to make
    * vertices order counter clock wise (used later to produce 3D mesh as loft)
    * @param points source points of contour
    */
    static void makePointsCounterClockwise(geo::ArrayContour2f& points);


    /**
    * @brief Split concave contour into set (may be more than two) of convex contours
    * @note Split is performed depending on @ref splitMethod_
    * @return array of contours
    */
    geo::ArrayContours2f splitConcaveContour();

private:

    static bool allPointsFromOneSide(const geo::ArrayContour2f& points, int i, int iNext);
    static std::pair<geo::ArrayContour2f, geo::ArrayContour2f> splitPolyInTwo(const geo::ArrayContour2f& poly);
    static bool edgeInside(const geo::ArrayContour2f& points, int iPrev, int i, int iNext, int j);
    static bool vertexConvex(const geo::ArrayContour2f& poly, int vertexIndex);
    static bool edgeIntersectsPoly(const geo::ArrayContour2f& poly, int i0, int i1);

    geo::ArrayContours2f splitConcaveContourByShortestBridge();
    geo::ArrayContours2f splitConcaveContourByTriangulation();

    static int findFarVertexRelateToLine(const geo::ArrayContour2f& poly, const geo::Point2f& vBaseStart, const geo::Point2f& vBaseEnd);

    static std::pair<geo::ArrayContour2f, geo::ArrayContour2f> detachTriangle(const geo::ArrayContour2f& poly, int index);

    //! source contour points
    const geo::ArrayContour2f& pointsSrc_;

    //! split method
    SplitMethod splitMethod_{ SplitMethod::SHORTEST_BRIDGE };
};


} // namespace geo

#endif // CONTOURSPLITTER_H
