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
// meshclip.h
// Clipping of the Mesh3d
//
#ifndef MESHCLIP_H
#define MESHCLIP_H

#include <utility>
#include "mesh3d.h"
#include "plane3d.h"

namespace geo
{

enum class ClipResult : uint8_t
{
	kOnPositiveSide = 0,
	kOnNegativeSide = 1,
	kClipped = 2
};

class Mesh3dClip
{
public:
	static [[nodiscard]] bool canBeClipped(const geo::Mesh3d& mesh, const geo::Plane3d& plane);

	static [[nodiscard]]  std::pair<geo::Mesh3d, geo::Mesh3d> clipMesh(
		const geo::Mesh3d& mesh, 
		const geo::Plane3d& plane);

	static [[nodiscard]] geo::ClipResult triangleClipped(const ArrayPoint3f& pointsTri, const geo::Plane3d& plane);

	static std::pair<geo::ArrayPoint3f, geo::ArrayPoint3f> clipTriangle(
		const ArrayPoint3f& pointsTri,
		const geo::Plane3d& plane);

	static [[nodiscard]] bool lineClipped(
		const geo::Point3f& va, 
		const geo::Point3f& vb, 
		const geo::Plane3d& plane, 
		geo::Point3f& vOutClip,
		bool& aPos,
		bool& bPos);

	static [[nodiscard]] bool checkTopology(const geo::Mesh3d& mesh);
};

} // namespace geo
#endif // MESHCLIP_H

