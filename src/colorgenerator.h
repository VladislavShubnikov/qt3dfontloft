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
// colorgenerator.h
// Assign different colors to 3D mesh parts
//
#ifndef COLORGENERATOR_H
#define COLORGENERATOR_H

#include "geo.h"

namespace app
{

/**
 * @class ColorGenerator
 * @brief Simple color generator for produced 3D meshed suring build or clipping
 */
class ColorGenerator
{
public:
    ColorGenerator();

	/**
	* @brief Get next color value for produced meshes
	* @return color with 3 components
	*/
	geo::Point3f nextColor();

private:
	//! internal counter, used to generate colors
    int decComponent_{ 1 };
	//! iterated color value
    geo::Point3f color_{ 0.0F, geo::kFloatAlmostOne, 0.0F };
}; // class ColorGenerator

} // namespace app

#endif // COLORGENERATOR_H

