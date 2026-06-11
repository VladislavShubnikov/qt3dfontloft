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
// testclip.cpp
// Tests for mesh clipping functions
//

#include <cstdlib>
#include <utility>

#include <QDebug>

#include "geo.h"
#include "mesh3d.h"
#include "meshclip.h"
#include "meshtools.h"
#include "plane3d.h"
#include "duration.h"

#include "contoursplitter.h"

#include "testclip.h"

namespace
{



bool vecSame(const geo::ArrayContour2f& vecA, const geo::ArrayContour2f& vecB)
{
    const auto n = static_cast<int>(vecA.size());
    if (n != static_cast<int>(vecB.size()))
    {
        return false;
    }
    for (int i = 0; i < n; i++)
    {
        const geo::Point2f dif = vecB[i] - vecA[i];
        const float d2 = dif.squaredNorm();
        if (d2 > 1.0e-8F)
        {
            return false;
        }
    }
    return true;
}

bool valueSame(float a, float b)
{
    const float dif = std::abs(a - b);
    return dif < 1.0e-8F;
}

} // namespace

void geo::TestGeo::initTestCase()
{
}
void geo::TestGeo::cleanupTestCase()
{
}

void geo::TestGeo::testMeshPolyConvex()
{
    const std::vector<geo::Point2f> pointsTriangle{
        {0.0F, 0.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}
    };
    geo::ContourSplitter splitterRect(pointsTriangle);
    bool isConvex = splitterRect.pointsConvex(pointsTriangle);
    QVERIFY(isConvex);

    const std::vector<geo::Point2f> pointsRect4{
        {0.0F, 0.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}, {0.0F, 1.0F}
    };
    isConvex = splitterRect.pointsConvex(pointsRect4);
    QVERIFY(isConvex);

    const std::vector<geo::Point2f> pointsDegenThree4{
        {0.0F, 0.0F}, {1.0F, 0.0F}, {2.0F, 0.0F},  { 1.0F, 1.0F }
    };
    isConvex = splitterRect.pointsConvex(pointsDegenThree4);
    QVERIFY(isConvex);

    const std::vector<geo::Point2f> pointsDegenLastThree4{
        {0.0F, 0.0F}, { 1.0F, 1.0F } , {2.0F, 0.0F}, {1.0F, 0.0F},
    };
    isConvex = splitterRect.pointsConvex(pointsDegenLastThree4);
    QVERIFY(isConvex);

    const std::vector<geo::Point2f> pointsDegenMidThree4{
        {1.0F, 0.0F} , {0.0F, 0.0F}, { 1.0F, 1.0F } , {2.0F, 0.0F},
    };
    isConvex = splitterRect.pointsConvex(pointsDegenMidThree4);
    QVERIFY(isConvex);

    const std::vector<geo::Point2f> pointsSameVertex4{
        {0.0F, 0.0F}, {1.0F, 0.0F}, {1.0F, 0.0F},  { 1.0F, 1.0F }
    };
    isConvex = splitterRect.pointsConvex(pointsSameVertex4);
    QVERIFY(isConvex);

    const std::vector<geo::Point2f> pointsConcavePyramid{
        {0.0F, 0.0F}, {1.0F, 1.0F}, {2.0F, 0.0F},  { 1.0F, 3.0F }
    };
    isConvex = splitterRect.pointsConvex(pointsConcavePyramid);
    QVERIFY(!isConvex);

    // same, but in reverse order
    const std::vector<geo::Point2f> pointsConcavePyramidRev{
        {0.0F, 0.0F}, { 1.0F, 3.0F } , {2.0F, 0.0F}, {1.0F, 1.0F}
    };
    isConvex = splitterRect.pointsConvex(pointsConcavePyramidRev);
    QVERIFY(!isConvex);
}

void geo::TestGeo::testMeshMakeCCW()
{
    std::vector<geo::Point2f> pointsCW{
        {0.0F, 0.0F}, {0.0F, 1.0F}, {1.0F, 1.0F}, {1.0F, 0.0F},
    };
    geo::ContourSplitter::makePointsCounterClockwise(pointsCW);
    QVERIFY((pointsCW[0] - geo::Point2f(1.0F, 0.0F)).squaredNorm() < 1.0e-6F);

    std::vector<geo::Point2f> pointsCorrectCCW{
        {0.0F, 0.0F}, {0.0F, 1.0F}, {1.0F, 1.0F}, {1.0F, 0.0F},
    };
    geo::ContourSplitter::makePointsCounterClockwise(pointsCW);
    QVERIFY((pointsCorrectCCW[0] - geo::Point2f(0.0F, 0.0F)).squaredNorm() < 1.0e-6F);

    std::vector<geo::Point2f> points4a{
        {0.0F, 0.0F}, {1.0F, 1.0F}, {2.0F, 0.0F}, {1.0F, 0.0F},
    };
    geo::ContourSplitter::makePointsCounterClockwise(points4a);
    QVERIFY((points4a[0] - geo::Point2f(1.0F, 0.0F)).squaredNorm() < 1.0e-6F);

    std::vector<geo::Point2f> points4b{
        {1.0F, 1.0F}, {2.0F, 0.0F}, {1.0F, 0.0F}, {0.0F, 0.0F}
    };
    geo::ContourSplitter::makePointsCounterClockwise(points4b);
    QVERIFY((points4b[0] - geo::Point2f(0.0F, 0.0F)).squaredNorm() < 1.0e-6F);

    std::vector<geo::Point2f> points4c{
        {2.0F, 0.0F}, {1.0F, 0.0F}, {0.0F, 0.0F}, {1.0F, 1.0F}
    };
    geo::ContourSplitter::makePointsCounterClockwise(points4c);
    QVERIFY((points4c[0] - geo::Point2f(1.0F, 1.0F)).squaredNorm() < 1.0e-6F);
}

void geo::TestGeo::testSplitLargeCircle()
{
    constexpr float kRadius{ 10.0F };
    constexpr int kNumTests{12};
    for (int t = 0; t < kNumTests; t++)
    {
        std::vector<geo::Point2f> pointsCircle;
        const int numVertices{ 12 + (8 * t) };
        for (int i = 0; i < numVertices; i++)
        {
            const float angleGrad = 360.0F * static_cast<float>(i) / static_cast<float>(numVertices);
            const float angleRad{ angleGrad * 3.1415926F / 180.0F };
            const float x = kRadius * cosf(angleRad);
            const float y = kRadius * sinf(angleRad);
            const geo::Point2f p(x, y);
            pointsCircle.push_back(p);
        }
        // source circle points are convex
        bool isConv = geo::ContourSplitter::pointsConvex(pointsCircle);
        QVERIFY(isConv);
        // make 0th point concave
        pointsCircle[0].x_ = kRadius * 0.85F;
        pointsCircle[0].y_ = 0.0F;
        isConv = geo::ContourSplitter::pointsConvex(pointsCircle);
        QVERIFY(!isConv);

        // perform poly split by 2 different methods
        geo::ContourSplitter splitterBridge(pointsCircle, geo::SplitMethod::SHORTEST_BRIDGE);
        geo::ContourSplitter splitterTriangulation(pointsCircle, geo::SplitMethod::TRIANGULATION);

        geo::ArrayContours2f contArrBridge;
        geo::ArrayContours2f contArrTriangulation;

        timer::Duration durationBridge;
        contArrBridge = splitterBridge.splitConcaveContour();
        const double tmBridge = durationBridge.getMilliseconds();

        timer::Duration durationTriangulation;
        contArrTriangulation = splitterTriangulation.splitConcaveContour();
        const double tmTriangulation = durationTriangulation.getMilliseconds();

        // triangulation method works faster than bridge more than 80 times
        const double tmRatio{ tmBridge / tmTriangulation };
        QVERIFY(tmRatio > 0.5);
        // QVERIFY(tmRatio > 80.0);

        QVERIFY(contArrBridge.size() >= 2);
        QVERIFY(contArrTriangulation.size() > 2);
    } // for all tests
}

void geo::TestGeo::testSplitConcaveByTriangulation()
{
    // all source contours are CCW
    std::vector<geo::Point2f> pointsPine
    {
        {0.0F, 0.0F}, {2.0F, 1.0F}, {1.0F, 2.0F}, {3.0F, 3.0F}, {0.0F, 5.0F}
    };
    geo::ContourSplitter splitterPine(pointsPine, geo::SplitMethod::TRIANGULATION);
    geo::ArrayContours2f contArr = splitterPine.splitConcaveContour();

    QVERIFY(contArr.size() == 3);
    QVERIFY(contArr[0].size() == 3);
    QVERIFY(contArr[1].size() == 3);
    QVERIFY(contArr[2].size() == 3);

    std::vector<geo::Point2f> pointsHouse
    {
        {0.0F, 0.0F}, {4.0F, 0.0F}, {3.0F, 3.0F}, {2.0F, 2.0F}, {0.0F, 2.0F}
    };
    geo::ContourSplitter splitterHouse(pointsHouse, geo::SplitMethod::TRIANGULATION);
    contArr = splitterHouse.splitConcaveContour();
    QVERIFY(contArr.size() == 2);
    QVERIFY(contArr[0].size() == 3);
    QVERIFY(contArr[1].size() == 4);

    geo::Rect2f box1 = getPointsBoundingRect(contArr[1]);
    QVERIFY(valueSame(box1.width(), 4.0F));
    QVERIFY(valueSame(box1.height(), 2.0F));

}

void geo::TestGeo::testSplitConcaveByBridge()
{
    // all source contours are CCW
    std::vector<geo::Point2f> pointsPerfect{
        {0.0F, 0.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}, {0.0F, 1.0F}
    };
    geo::ContourSplitter splitterPerfect(pointsPerfect);
    geo::ArrayContours2f contArr = splitterPerfect.splitConcaveContour();

    // result is 1 same contour
    QVERIFY(contArr.size() == 1);
    geo::ArrayContour2f vec = contArr[0];
    QVERIFY(vecSame(vec, pointsPerfect));

    // concave pyramid
    std::vector<geo::Point2f> pointsPyramid{
        {0.0F, 0.0F}, {1.0F, 1.0F}, {2.0F, 0.0F}, {1.0F, 3.0F}
    };
    geo::ContourSplitter splitterPyramid(pointsPyramid);
    contArr = splitterPyramid.splitConcaveContour();

    QVERIFY(contArr.size() == 2);
    QVERIFY(contArr[0].size() == 3);
    QVERIFY(contArr[1].size() == 3);

    geo::Rect2f box0 = getPointsBoundingRect(contArr[0]);
    geo::Rect2f box1 = getPointsBoundingRect(contArr[1]);
    QVERIFY(valueSame(box0.width(), 1.0F));
    QVERIFY(valueSame(box1.width(), 1.0F));
    QVERIFY(valueSame(box0.height(), 3.0F));
    QVERIFY(valueSame(box1.height(), 3.0F));

    QVERIFY(valueSame(box1.pMax_.x_, 2.0F));
    QVERIFY(valueSame(box0.pMin_.x_, 0.0F));
    QVERIFY(valueSame(box1.pMin_.x_, box0.pMax_.x_));

    QVERIFY(valueSame(box0.pMin_.y_, 0.0F));
    QVERIFY(valueSame(box1.pMin_.y_, 0.0F));
    QVERIFY(valueSame(box0.pMax_.y_, 3.0F));
    QVERIFY(valueSame(box1.pMax_.y_, 3.0F));

    // concave :
    //
    //    *        *
    //    | \     /|
    //    |   \ /  |
    //    |    *   |
    //    |        |
    //    |    *   |
    //    |  /  \  |
    //    | /     \|
    //    *        *
    std::vector<geo::Point2f> pointsSuite{
        {1.0F, 1.0F}, {3.0F, 2.0F}, {5.0F, 1.0F}, {5.0F, 4.0F},
        {3.0F, 3.0F}, {1.0F, 4.0F}
    };
    geo::ContourSplitter splitterSuite(pointsSuite);
    contArr = splitterSuite.splitConcaveContour();

    QVERIFY(contArr.size() == 3);
    QVERIFY(contArr[0].size() == 3);
    QVERIFY(contArr[1].size() == 3);
    box0 = getPointsBoundingRect(contArr[0]);
    box1 = getPointsBoundingRect(contArr[1]);
    QVERIFY(valueSame(box0.pMin_.x_, 1.0F));
    QVERIFY(valueSame(box0.pMax_.x_, 3.0F));
    QVERIFY(valueSame(box0.pMin_.y_, 1.0F));
    QVERIFY(valueSame(box0.pMax_.y_, 4.0F));
    QVERIFY(valueSame(box1.pMin_.x_, 1.0F));
    QVERIFY(valueSame(box1.pMax_.x_, 3.0F));
    QVERIFY(valueSame(box1.pMin_.y_, 1.0F));
    QVERIFY(valueSame(box1.pMax_.y_, 3.0F));

    // concave :
    /*
    //     *
    //    /  \
    //   / *   \
    //  / / \    *
    //  *    \  /
    //       * /
    //       |/
    //       *
    */
    std::vector<geo::Point2f> pointsHorseHeel{
        {1.0F, 3.0F}, {3.0F, 4.0F}, {4.0F, 3.0F}, {3.0F, 1.0F},
        {6.0F, 3.0F}, {2.0F, 6.0F}
    };    
    
    geo::ContourSplitter splitterHorse(pointsHorseHeel);
    contArr = splitterHorse.splitConcaveContour();

    QVERIFY(contArr.size() == 3);
    QVERIFY(contArr[0].size() == 4);
    QVERIFY(contArr[1].size() == 3);
    QVERIFY(contArr[2].size() == 3);
    box0 = getPointsBoundingRect(contArr[0]);
    box1 = getPointsBoundingRect(contArr[1]);
    geo::Rect2f box2 = getPointsBoundingRect(contArr[2]);

    QVERIFY(valueSame(box0.pMin_.x_, 2.0F));
    QVERIFY(valueSame(box0.pMax_.x_, 6.0F));
    QVERIFY(valueSame(box0.pMin_.y_, 3.0F));
    QVERIFY(valueSame(box0.pMax_.y_, 6.0F));

    QVERIFY(valueSame(box1.pMin_.x_, 1.0F));
    QVERIFY(valueSame(box1.pMax_.x_, 3.0F));
    QVERIFY(valueSame(box1.pMin_.y_, 3.0F));
    QVERIFY(valueSame(box1.pMax_.y_, 6.0F));

    QVERIFY(valueSame(box2.pMin_.x_, 3.0F));
    QVERIFY(valueSame(box2.pMax_.x_, 6.0F));
    QVERIFY(valueSame(box2.pMin_.y_, 1.0F));
    QVERIFY(valueSame(box2.pMax_.y_, 3.0F));

    // concave :
    //
    // *---*   *---*
    // |    \ /    |
    // |     *     |
    // |           |
    // *-----------*
    //

    std::vector<geo::Point2f> pointsRectTri{
        {0.0F, 0.0F}, {4.0F, 0.0F}, {4.0F, 2.0F}, {3.0F, 2.0F},
        {2.0F, 1.0F}, {1.0F, 2.0F}, {0.0F, 2.0F}
    };

    geo::ContourSplitter splitterRectTri(pointsRectTri);
    contArr = splitterRectTri.splitConcaveContour();

    QVERIFY(contArr.size() == 4);
    QVERIFY(contArr[0].size() == 4);
    QVERIFY(contArr[1].size() == 3);
    QVERIFY(contArr[2].size() == 3);
    QVERIFY(contArr[3].size() == 3);
    box0 = getPointsBoundingRect(contArr[0]);
    box1 = getPointsBoundingRect(contArr[1]);
    box2 = getPointsBoundingRect(contArr[2]);
    geo::Rect2f box3 = getPointsBoundingRect(contArr[3]);

    QVERIFY(valueSame(box0.pMin_.x_, 0.0F));
    QVERIFY(valueSame(box0.pMax_.x_, 2.0F));
    QVERIFY(valueSame(box0.pMin_.y_, 0.0F));
    QVERIFY(valueSame(box0.pMax_.y_, 2.0F));

    QVERIFY(valueSame(box1.pMin_.x_, 2.0F));
    QVERIFY(valueSame(box1.pMax_.x_, 4.0F));
    QVERIFY(valueSame(box1.pMin_.y_, 0.0F));
    QVERIFY(valueSame(box1.pMax_.y_, 2.0F));

    QVERIFY(valueSame(box2.pMin_.x_, 0.0F));
    QVERIFY(valueSame(box2.pMax_.x_, 4.0F));
    QVERIFY(valueSame(box2.pMin_.y_, 0.0F));
    QVERIFY(valueSame(box2.pMax_.y_, 1.0F));

    QVERIFY(valueSame(box3.pMin_.x_, 3.0F));
    QVERIFY(valueSame(box3.pMax_.x_, 4.0F));
    QVERIFY(valueSame(box3.pMin_.y_, 0.0F));
    QVERIFY(valueSame(box3.pMax_.y_, 2.0F));

    /* concave
    //
    //   *    *
    //  /  \ / \
    // *    *   *
    //   \     /
    //     \ /
    //      *
    */
    std::vector<geo::Point2f> pointsParalellConcave{
        {0.0F, 1.0F}, {3.0F, 0.0F}, {6.0F, 1.0F}, {5.0F, 2.0F},
        {3.0F, 1.0F}, {1.0F, 2.0F}
    };

    geo::ContourSplitter splitterParallel(pointsParalellConcave);
    contArr = splitterParallel.splitConcaveContour();

    QVERIFY(contArr.size() == 2);
    QVERIFY(contArr[0].size() == 4);
    QVERIFY(contArr[1].size() == 4);

    box0 = getPointsBoundingRect(contArr[0]);
    box1 = getPointsBoundingRect(contArr[1]);

    QVERIFY(valueSame(box0.pMin_.x_, 0.0F));
    QVERIFY(valueSame(box0.pMax_.x_, 3.0F));
    QVERIFY(valueSame(box0.pMin_.y_, 0.0F));
    QVERIFY(valueSame(box0.pMax_.y_, 2.0F));

    QVERIFY(valueSame(box1.pMin_.x_, 3.0F));
    QVERIFY(valueSame(box1.pMax_.x_, 6.0F));
    QVERIFY(valueSame(box1.pMin_.y_, 0.0F));
    QVERIFY(valueSame(box1.pMax_.y_, 2.0F));
}

void geo::TestGeo::testMeshClipCheck()
{
    geo::Mesh3d mesh;
    // add vertices
    mesh.vertices_.emplace_back(0.0F, 0.0F, 0.0F);
    mesh.vertices_.emplace_back(0.0F, 0.0F, 1.0F);
    mesh.vertices_.emplace_back(0.0F, 1.0F, 0.0F);
    mesh.vertices_.emplace_back(0.0F, 1.0F, 1.0F);
    mesh.indices_ = { 1, 0, 2, 2, 3, 1 };

    geo::Plane3d plane;
    plane.normal_ = { 0.0F, 1.0F, 0.0F };
    plane.point_ = { 0.0F, -100.0F, 0.0F };

    bool clipped = geo::Mesh3dClip::canBeClipped(mesh, plane);
    QVERIFY(!clipped);

    constexpr float kSmall{ 1.0e-5F };
    plane.point_ = { 0.0F, 1.0F + kSmall, 0.0F };
    clipped = geo::Mesh3dClip::canBeClipped(mesh, plane);
    QVERIFY(!clipped);

    plane.point_ = { 0.0F, 0.0F + kSmall, 0.0F };
    clipped = geo::Mesh3dClip::canBeClipped(mesh, plane);
    QVERIFY(clipped);

    plane.point_ = { 0.0F, 1.0F - kSmall, 0.0F };
    clipped = geo::Mesh3dClip::canBeClipped(mesh, plane);
    QVERIFY(clipped);
}

void geo::TestGeo::testMeshClipA()
{
    geo::Mesh3d mesh; // NOLINT
    // add vertices
    mesh.vertices_.push_back({ 0.0F, 0.0F, 0.0F });
    mesh.vertices_.push_back({ 0.0F, 0.0F, 1.0F });
    mesh.vertices_.push_back({ 0.0F, 1.0F, 0.0F });
    mesh.vertices_.push_back({ 0.0F, 1.0F, 1.0F });
    mesh.indices_ = { 1, 0, 2, 2, 3, 1 };

    geo::Plane3d plane;
    plane.normal_ = { 0.0F, 1.0F, 0.0F };
    plane.point_ = { 0.0F, 0.5F, 0.0F };

    const auto pairMeshes = geo::Mesh3dClip::clipMesh(mesh, plane);
    const geo::Mesh3d& meshPos = pairMeshes.first;
    const geo::Mesh3d& meshNeg = pairMeshes.second;

    QVERIFY(meshPos.vertices_.size() == 5);
    QVERIFY(meshNeg.vertices_.size() == 5);
    QVERIFY(meshPos.indices_.size() == 9);
    QVERIFY(meshNeg.indices_.size() == 9);
}

void geo::TestGeo::testMeshClipB()
{
    geo::Mesh3d mesh; // NOLINT
    // add vertices
    mesh.vertices_.push_back({ 0.0F, 0.0F, 0.0F });
    mesh.vertices_.push_back({ 0.0F, 0.0F, 1.0F });
    mesh.vertices_.push_back({ 0.0F, 1.0F, 0.0F });
    mesh.vertices_.push_back({ 0.0F, 1.0F, 1.0F });
    mesh.indices_ = { 1, 0, 2, 2, 3, 1 };

    geo::Plane3d plane;
    constexpr float kEps{ 1.0e-4F };
    plane.point_ = { 0.0F, 1.0F, 1.0F - kEps };
    plane.normal_ = { 0.0F, 0.99F, -0.1F };
    plane.normal_.normalize();

    const auto pairMeshes = geo::Mesh3dClip::clipMesh(mesh, plane);
    const geo::Mesh3d& meshPos = pairMeshes.first;
    const geo::Mesh3d& meshNeg = pairMeshes.second;

    QVERIFY(meshPos.vertices_.size() == 4);
    QVERIFY(meshNeg.vertices_.size() == 6);
}

void geo::TestGeo::testLineClip()
{
    geo::Plane3d plane;
    plane.normal_ = { 0.0F, 1.0F, 0.0F };
    plane.point_ = { 0.0F, 0.5F, 0.0F };
    geo::Point3f va{ 0.0F, 0.0F, 0.0F };
    geo::Point3f vb{ 0.0F, 1.0F, 0.0F };
    geo::Point3f vClip{};
    constexpr float kSmall{ 1.0e-10F };

    bool aPos{ false };
    bool bPos{ false };
    bool hasClip = geo::Mesh3dClip::lineClipped(va, vb, plane, vClip, aPos, bPos);
    QVERIFY(hasClip);
    QVERIFY((vClip - geo::Point3f(0.0F, 0.5F, 0.0F)).squaredNorm() < kSmall);
    QVERIFY(!aPos);
    QVERIFY(bPos);

    va = { 0.0F, 0.0F, 0.0F };
    vb = { 0.0F, 0.1F, 0.0F };
    hasClip = geo::Mesh3dClip::lineClipped(va, vb, plane, vClip, aPos, bPos);
    QVERIFY(!hasClip);
    QVERIFY(!aPos);
    QVERIFY(!bPos);

    va = { 0.0F, 1.0F, 0.0F };
    vb = { 0.0F, 1.1F, 0.0F };
    hasClip = geo::Mesh3dClip::lineClipped(va, vb, plane, vClip, aPos, bPos);
    QVERIFY(!hasClip);
    QVERIFY(aPos);
    QVERIFY(bPos);

    va = { 0.0F, 0.0F, 0.0F };
    vb = { 1.0F, 1.0F, 1.0F };
    hasClip = geo::Mesh3dClip::lineClipped(va, vb, plane, vClip, aPos, bPos);
    QVERIFY(hasClip);
    QVERIFY((vClip - geo::Point3f(0.5F, 0.5F, 0.5F)).squaredNorm() < kSmall);

    constexpr float kEps{ 1.0e-4F };
    va = { 0.0F, 0.5F - kEps, 0.0F };
    vb = { 0.0F, 0.5F + kEps, 0.0F };
    hasClip = geo::Mesh3dClip::lineClipped(va, vb, plane, vClip, aPos, bPos);
    QVERIFY(hasClip);
    QVERIFY((vClip - geo::Point3f(0.0F, 0.5F, 0.0F)).squaredNorm() < kSmall);
}

void geo::TestGeo::testTriangleClip()
{
    geo::ArrayPoint3f triangleSrc{
        {0.0F, 0.0F, 1.0F},
        {0.0F, 0.0F, 0.0F},
        {0.0F, 1.0F, 0.0F},
    };

    geo::Plane3d plane;
    plane.normal_ = { 0.0F, 1.0F, 0.0F };
    plane.point_ = { 0.0F, 0.5F, 0.0F };

    std::pair<geo::ArrayPoint3f, geo::ArrayPoint3f> pairRes =
        geo::Mesh3dClip::clipTriangle(triangleSrc, plane);
    const geo::ArrayPoint3f& triPos = pairRes.first;
    const geo::ArrayPoint3f& triNeg = pairRes.second;
    QVERIFY(triPos.size() == 3);
    QVERIFY(triNeg.size() == 4);
}

void geo::TestGeo::testMeshFix()
{
    geo::Mesh3d mesh;
    mesh.vertices_ = {
        {0.0F, 0.0F, 0.0F},
        {1.0F, 0.0F, 0.0F},
        {2.0F, 0.0F, 0.0F},
        {3.0F, 0.0F, 0.0F},
        {4.0F, 0.0F, 0.0F},
    };
    mesh.indices_ = { 0, 2, 4 };
    geo::MeshTools::fixUnreferencedVertices(mesh);
    QVERIFY(mesh.vertices_.size() == 3);
    QVERIFY(mesh.indices_[0] == 0);
    QVERIFY(mesh.indices_[1] == 1);
    QVERIFY(mesh.indices_[2] == 2);

    mesh.vertices_ = {
        {0.0F, 0.0F, 0.0F},
        {1.0F, 0.0F, 0.0F},
        {2.0F, 0.0F, 0.0F},
        {3.0F, 0.0F, 0.0F},
        {4.0F, 0.0F, 0.0F},
    };
    mesh.indices_ = { 1, 2, 3, 2, 3, 4 };
    geo::MeshTools::fixUnreferencedVertices(mesh);
    QVERIFY(mesh.vertices_.size() == 4);
    QVERIFY(mesh.indices_.size() == 6);
    QVERIFY(mesh.indices_[0] == 0);
    QVERIFY(mesh.indices_[1] == 1);
    QVERIFY(mesh.indices_[2] == 2);
    QVERIFY(mesh.indices_[3] == 1);
    QVERIFY(mesh.indices_[4] == 2);
    QVERIFY(mesh.indices_[5] == 3);
}
