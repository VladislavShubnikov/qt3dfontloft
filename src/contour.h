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
// contour.h
// Read 2d contour from json, normalize contour
//
#ifndef CONTOUR_H
#define CONTOUR_H

#include <vector>
#include <QString> // NOLINT
#include "geo.h"

namespace geo
{

using ArrayContour2f = std::vector<geo::Point2f>;
using ArrayContours2f = std::vector<ArrayContour2f>;

class Contour
{
public:
	bool readCharacterFromJson(const QString& character);
	[[nodiscard]] ArrayContours2f getContours() const;
	void removeDuplicates();
	void scaleTo(float side = 1.0F);

private:
	ArrayContours2f contours_{};
};

} // namespace geo



#endif // CONTOUR_H

