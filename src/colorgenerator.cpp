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
// colorgenerator.cpp
// Assign different colors to 3D mesh parts
//

#include "geo.h"
#include "colorgenerator.h"

app::ColorGenerator::ColorGenerator()
    : color_(0.0F, geo::kFloatAlmostOne, 0.0F)
{
}

geo::Point3f app::ColorGenerator::nextColor()
{
    constexpr float kStep{ 0.4F };
    geo::Point3f colorRet{ color_ };
    if (decComponent_ == 1)
    {
        color_.y_ -= kStep;
        color_.z_ += kStep;
        if (color_.y_ < 0.0F)
        {
            color_.y_ = 0.0F;
            color_.z_ = 1.0F - kStep;
            color_.x_ = kStep;
            decComponent_ = 2;
            return colorRet;
        }
    }
    else if (decComponent_ == 2)
    {
        color_.z_ -= kStep;
        color_.x_ += kStep;
        if (color_.z_ < 0.0F)
        {
            color_.z_ = 0.0F;
            color_.x_ = 1.0F - kStep;
            color_.y_ = kStep;
            decComponent_ = 0;
            return colorRet;
        }
    }
    else if (decComponent_ == 0)
    {
        color_.x_ -= kStep;
        color_.y_ += kStep;
        if (color_.x_ < 0.0F)
        {
            color_.x_ = 0.0F;
            color_.y_ = 1.0F - kStep;
            color_.z_ = kStep;
            decComponent_ = 1;
            return colorRet;
        }
    }
    return colorRet;
}

