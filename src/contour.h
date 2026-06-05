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


/**
 * @class Contour
 * @brief Implement read json source files (in data folder) to create source 2D contours
 *
 * @note Here, in this app, font character contours are used, but it can be any
 * contours. Maybe, for better app testing, it can be nice to have a large set 
 * of artificially generated 2D contours with different forms
 */
class Contour
{
public:

	/**
	* @brief Read 2d contours from 
	* @param character character, used to build source json file name to read
	* @return true if read was successfull
	*/
	bool readCharacterFromJson(const QString& character);

	/**
	* @brief Read 2d contours from
	* @return loaded 2D contours data
	*/
	[[nodiscard]] ArrayContours2f getContours() const;

	/**
	* @brief Special operation to remove duplicated points in vertices data
	*/
	void removeDuplicates();

	/**
	* @brief Scale 2d contour vertices to the given scale
	* @param scale size to fit
	*/
	void scaleTo(float side = 1.0F);

private:
	//! Loaded 2d contours
	ArrayContours2f contours_{};
};

} // namespace geo



#endif // CONTOUR_H

