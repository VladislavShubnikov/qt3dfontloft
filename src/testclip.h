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
// testclip.h
// Tests for mesh clipping functions
//
#ifndef TESTCLIP_H
#define TESTCLIP_H

#include <QtTest>

namespace geo
{

//! test geometry
class TestGeo: public QObject
{
    Q_OBJECT
private slots:
    //! init common data for all tests
    void initTestCase();
    //! done common data for all tests
    void cleanupTestCase();

    //! already convex poly
    void testMeshPolyConvex();
    //! wrong clockwise direction
    void testMeshMakeCCW();
    //! split concave poly
    void testSplitConcave();
    //! check is clipped mesh
    void testMeshClipCheck();
    //! check is clipped mesh A
    void testMeshClipA();
    //! check is clipped mesh B
    void testMeshClipB();
    //! check is clipped line
    void testLineClip();
    //! check triangle clip
    void testTriangleClip();
    //! check fix mesh with bad vertex
    void testMeshFix();


}; // class TestGeo

} // namespace geo

#endif // TESTCLIP_H
