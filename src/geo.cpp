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
// geo.cpp
// Geometry primitives: points, rectangles
//

#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "geo.h"

namespace geo
{

geo::Point2f operator-(const geo::Point2f& pa, const geo::Point2f& pb)
{
	return { pa.x_ - pb.x_, pa.y_ - pb.y_ };
}

geo::Point2f operator+(const geo::Point2f& pa, const geo::Point2f& pb)
{
	return { pa.x_ + pb.x_, pa.y_ + pb.y_ };
}

geo::Point3f operator-(const geo::Point3f& pa, const geo::Point3f& pb)
{
	return { pa.x_ - pb.x_, pa.y_ - pb.y_, pa.z_ - pb.z_ };
}

geo::Point3f operator+(const geo::Point3f& pa, const geo::Point3f& pb)
{
	return { pa.x_ + pb.x_, pa.y_ + pb.y_, pa.z_ + pb.z_ };
}

geo::Point3f geo::crossProduct(const geo::Point3f& pa, const geo::Point3f& pb)
{
	return {
		(pa.y_ * pb.z_) - (pa.z_ * pb.y_),
		(pa.z_ * pb.x_) - (pa.x_ * pb.z_),
		(pa.x_ * pb.y_) - (pa.y_ * pb.x_)
	};
}

} // namespace geo


void geo::Point3f::normalize()
{
	const float d = sqrtf((x_ * x_) + (y_ * y_) + (z_ * z_));
	constexpr float kSmall{ 1.0e-6F };
	if (std::abs(d) > kSmall)
	{
		const float scale = 1.0F / d;
		x_ *= scale;
		y_ *= scale;
		z_ *= scale;
	}
}

float geo::Point3f::dotProduct(const geo::Point3f& p) const
{
	const float dp = (x_ * p.x_) + (y_ * p.y_) + (z_ * p.z_);
	return dp;
}

float geo::Point3f::squaredNorm() const
{
	return (x_ * x_) + (y_ * y_) + (z_ * z_);
}

float geo::Point2f::squaredNorm() const
{
	return (x_ * x_) + (y_ * y_);
}

void geo::Point2f::normalize()
{
	const float d = sqrtf((x_ * x_) + (y_ * y_));
	constexpr float kSmall{ 1.0e-6F };
	if (std::abs(d) > kSmall)
	{
		const float scale = 1.0F / d;
		x_ *= scale;
		y_ *= scale;
	}
}

float geo::Point2f::dotProduct(const geo::Point2f& p) const
{
	const float dp = (x_ * p.x_) + (y_ * p.y_);
	return dp;
}


float geo::Rect2f::width() const
{
	return pMax_.x_ - pMin_.x_;
}

float geo::Rect2f::height() const
{
	return pMax_.y_ - pMin_.y_;
}

void geo::Rect2f::extend(const geo::Point2f& p)
{
	pMin_.x_ = std::min(pMin_.x_, p.x_);
	pMin_.y_ = std::min(pMin_.y_, p.y_);
	pMax_.x_ = std::max(pMax_.x_, p.x_);
	pMax_.y_ = std::max(pMax_.y_, p.y_);
}

geo::Rect2f geo::getPointsBoundingRect(const std::vector<geo::Point2f>& points)
{
	Rect2f rect;

	rect.pMin_.x_ = std::numeric_limits<float>::max();
	rect.pMin_.y_ = std::numeric_limits<float>::max();
	rect.pMax_.x_ = -std::numeric_limits<float>::max();
	rect.pMax_.y_ = -std::numeric_limits<float>::max();

	for (const geo::Point2f& p : points)
	{
		rect.extend(p);
	}

	return rect;

}

geo::Rect2f geo::getPointsBoundingRect(const std::vector<std::vector<Point2f>>& contours)
{
	Rect2f rect;

	rect.pMin_.x_ = std::numeric_limits<float>::max();
	rect.pMin_.y_ = std::numeric_limits<float>::max();
	rect.pMax_.x_ = -std::numeric_limits<float>::max();
	rect.pMax_.y_ = -std::numeric_limits<float>::max();

	for (const std::vector<geo::Point2f>& vec : contours)
	{
		for (const geo::Point2f& p : vec)
		{
			rect.extend(p);
		}
	}

	return rect;
}
