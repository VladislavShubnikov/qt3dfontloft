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
// geo.h
// Geometry primitives: points, rectangles
//

#ifndef GEO_H
#define GEO_H

#include <vector>

namespace geo
{

//! Almost 1.0 value
constexpr float kFloatAlmostOne{ 0.99999F };

/**
* @struct Point2d
* @brief 2d integer point
*/
struct Point2d
{
    int x_{ 0 };
    int y_{ 0 };
};

/**
* @struct Point2f
* @brief 2d float point
*/
struct Point2f
{
    float x_{ 0.0F };
    float y_{ 0.0F };

    Point2f() = default;
    Point2f(float xx, float yy) // NOLINT(bugprone-easily-swappable-parameters)
    :x_(xx), y_(yy) { }
    Point2f(const Point2f& p) = default;
    ~Point2f() = default;

    Point2f& operator=(const Point2f&) = default;
    Point2f(Point2f&& p) = default;
    Point2f& operator=(Point2f&&) = default;


    [[nodiscard]] float dotProduct(const Point2f& p) const;
    void normalize();
    [[nodiscard]] float squaredNorm() const;
};

geo::Point2f operator-(const geo::Point2f& pa, const geo::Point2f& pb);
geo::Point2f operator+(const geo::Point2f& pa, const geo::Point2f& pb);


/**
* @struct Point3f
* @brief 3d float point
*/
struct Point3f
{
    float x_{ 0.0F };
    float y_{ 0.0F };
    float z_{ 0.0F };

    Point3f() = default;
    Point3f(float xx, float yy, float zz) // NOLINT(bugprone-easily-swappable-parameters)
        :x_(xx), y_(yy), z_(zz) {}
    Point3f(const Point3f& p) = default;
    ~Point3f() = default;

    Point3f& operator=(const Point3f&) = default;
    Point3f(Point3f&& p) = default;
    Point3f& operator=(Point3f&&) = default;

    void scale(const float s)
    {
        x_ *= s;
        y_ *= s;
        z_ *= s;
    }
    void normalize();
    [[nodiscard]] float dotProduct(const Point3f& p) const;
    [[nodiscard]] float squaredNorm() const;
};

geo::Point3f operator-(const geo::Point3f& pa, const geo::Point3f& pb);
geo::Point3f operator+(const geo::Point3f& pa, const geo::Point3f& pb);
geo::Point3f crossProduct(const geo::Point3f& pa, const geo::Point3f& pb);

using ArrayPoint3f = std::vector<geo::Point3f>;

/**
* @struct Rect2f
* @brief 2d float point rectangle, used as bounding box
*/
struct Rect2f
{
    Point2f pMin_;
    Point2f pMax_;

    void extend(const Point2f& p);
    [[nodiscard]] float width() const;
    [[nodiscard]] float height() const;
};

Rect2f getPointsBoundingRect(const std::vector<Point2f>& points);
Rect2f getPointsBoundingRect(const std::vector<std::vector<Point2f>>& contours);



} // namespace geo

#endif // GEO_H
