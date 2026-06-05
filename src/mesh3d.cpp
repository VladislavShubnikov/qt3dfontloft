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
// mesh3d.cpp
// Basic 3d mesh data (without rendering functionality)
//

#include <QDebug>

#include "mesh3d.h"


geo::Mesh3d::~Mesh3d()
{
    // set next constant to true if you want to check avoiding unnecessary vector's copying
    // during mesh copy with move semantics
    const bool needLog{false};
    if (needLog)
    {
        const auto numVert = vertices_.size();
        qDebug() << "~Mesh3d with " << numVert << " vertices \n";
    }
}

geo::Mesh3d::Mesh3d(geo::Mesh3d&& other) noexcept
:vertices_(std::move(other.vertices_))
,normals_(std::move(other.normals_))
,indices_(std::move(other.indices_))
{
}

