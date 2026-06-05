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
    //! x component
    int x_{ 0 };
    //! y component
    int y_{ 0 };
};

/**
* @struct Point2f
* @brief 2d float point
*/
struct Point2f
{
    //! x component
    float x_{ 0.0F };
    //! y component
    float y_{ 0.0F };

    //! constructor
    Point2f() = default;
    //! constructor with 2 params
    Point2f(float xx, float yy) // NOLINT(bugprone-easily-swappable-parameters)
    :x_(xx), y_(yy) { }
    //! copy constructor
    Point2f(const Point2f& p) = default;
    //! destructor
    ~Point2f() = default;

    //! asignment operator
    Point2f& operator=(const Point2f&) = default;
    //! constructor with move
    Point2f(Point2f&& p) = default;
    //! assigmwn with move semantic
    Point2f& operator=(Point2f&&) = default;

    /**
     * @brief Get dot product
     * @param p anotherpoint
     * @return dot product with other vector
     */
    [[nodiscard]] float dotProduct(const Point2f& p) const;

    /**
     * @brief Normalize vector
     */
    void normalize();

    /**
     * @brief Get length ^ 2
     * @return squared self dot product
     */
    [[nodiscard]] float squaredNorm() const;
};

//! a - b operator
geo::Point2f operator-(const geo::Point2f& pa, const geo::Point2f& pb);
//! a + b operator
geo::Point2f operator+(const geo::Point2f& pa, const geo::Point2f& pb);


/**
* @struct Point3f
* @brief 3d float point
*/
struct Point3f
{
    //! x component
    float x_{ 0.0F };
    //! y component
    float y_{ 0.0F };
    //! z component
    float z_{ 0.0F };

    //! constructor
    Point3f() = default;
    //! constructor with parameters
    Point3f(float xx, float yy, float zz) // NOLINT(bugprone-easily-swappable-parameters)
        :x_(xx), y_(yy), z_(zz) {}
    //! constructor with copy
    Point3f(const Point3f& p) = default;
    //! destructor
    ~Point3f() = default;

    //! assignment
    Point3f& operator=(const Point3f&) = default;
    //! constructor with copy
    Point3f(Point3f&& p) = default;
    //! assignment with move semantic
    Point3f& operator=(Point3f&&) = default;

    /**
     * @brief Scale with scalar
     * @param s scale
     */
    void scale(const float s)
    {
        x_ *= s;
        y_ *= s;
        z_ *= s;
    }

    /**
     * @brief Normalize vector
     */
    void normalize();

    /**
     * @brief Get dot product
     * @param p point
     * @return dot product with other vector
     */
    [[nodiscard]] float dotProduct(const Point3f& p) const;

    /**
     * @brief norm ^ 2
     * @return dot product squared
     */
    [[nodiscard]] float squaredNorm() const;
};

//! operator a - b
geo::Point3f operator-(const geo::Point3f& pa, const geo::Point3f& pb);
//! operator a + b
geo::Point3f operator+(const geo::Point3f& pa, const geo::Point3f& pb);
//! cross product of a * b
geo::Point3f crossProduct(const geo::Point3f& pa, const geo::Point3f& pb);

using ArrayPoint3f = std::vector<geo::Point3f>;

/**
* @struct Rect2f
* @brief 2d float point rectangle, used as bounding box
*/
struct Rect2f
{
    //! Minimum rectangle poiint coordinates
    Point2f pMin_;
    //! Maximum rectangle poiint coordinates
    Point2f pMax_;

    /**
     * @brief Extend rectangle with given point
     * @param p point to enlarge
     */
    void extend(const Point2f& p);

    /**
     * @brief Get width
     * @return width of rectangle
     */
    [[nodiscard]] float width() const;

    /**
     * @brief Get height
     * @return height of rectangle
     */
    [[nodiscard]] float height() const;
};

/**
* @brief Get point bounding rectangle
* @param points polygone points
* @return bounding rectangle
*/
Rect2f getPointsBoundingRect(const std::vector<geo::Point2f>& points);

/**
* @brief Get contours bounding rectangle
* @param contours set of contours
* @return bounding rectangle
*/
Rect2f getPointsBoundingRect(const std::vector<std::vector<geo::Point2f>>& contours);



} // namespace geo

#endif // GEO_H
