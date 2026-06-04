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
// contourtools.cpp
// Geometry opertations with 2d contour(s)
//
#ifndef CONTOURTOOLS_H
#define CONTOURTOOLS_H

#include "contour.h"

namespace geo
{

class ContourTools
{
public:
    static [[nodiscard]] ArrayContours2f splitToConvexContours(const geo::ArrayContour2f& points);

    static [[nodiscard]] bool pointsConvex(const geo::ArrayContour2f& points);
    static void makePointsCounterClockwise(geo::ArrayContour2f& points);

    static [[nodiscard]] geo::ArrayContours2f
        splitConcaveContour(const geo::ArrayContour2f& points);

    static std::pair<geo::ArrayContour2f, geo::ArrayContour2f> splitPolyInTwo(const geo::ArrayContour2f& poly);

private:
    static [[nodiscard]] bool allPointsFromOneSide(const geo::ArrayContour2f& points, int i, int iNext);
    static [[nodiscard]] bool vertexConvex(const geo::ArrayContour2f& points, int i);
    static [[nodiscard]] bool edgeInside(const geo::ArrayContour2f& points, int iPrev, int i, int iNext, int j);

};

} // namespace geo


#endif // CONTOURTOOLS_H
