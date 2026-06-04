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
// mesh3dsaver.cpp
// Save 3d mesh to OBJ text format
//
#include <cstdint>

#include <QTextStream>

#include "geo.h"
#include "mesh3d.h"
#include "mesh3dsaver.h"

QString geo::Mesh3dSaver::saveToObjString(const geo::Mesh3d& mesh) // NOLINT(readability-convert-member-functions-to-static)
{
	// # Simple Triangle with Vertex Normals
	// # 3 Vertices(x, y, z)
	//	v 0.0 0.0 0.0
	//	v 1.0 0.0 0.0
	//	v 0.5 1.0 0.0

	//	# 3 Vertex Normals(nx, ny, nz) - pointing up
	//	vn 0.0 0.0 1.0
	//	vn 0.0 0.0 1.0
	//	vn 0.0 0.0 1.0

	//	# Faces(f v1//vn1 v2//vn2 v3//vn3)
	//	# Uses // to indicate no texture coordinates
	//	f 1//1 2//2 3//3
	// vertices

	QString sOut;
	QTextStream streamOut(&sOut);

	streamOut << "o objectMesh\n";

	const auto numVertices = static_cast<int>(mesh.vertices_.size());
	streamOut << "# " << numVertices << " vertices \n";
	for (int i = 0; i < numVertices; i++)
	{
		const geo::Point3f& v = mesh.vertices_[i];
		QString sVertex;
		QTextStream(&sVertex) << "v " << v.x_ << " " << v.y_ << " " << v.z_ << "\n";
		streamOut << sVertex;
	}

	streamOut << "# " << numVertices << " normals \n";
	for (int i = 0; i < numVertices; i++)
	{
		const geo::Point3f& v = mesh.normals_[i];
		QString sVertex;
		QTextStream(&sVertex) << "vn " << v.x_ << " " << v.y_ << " " << v.z_ << "\n";
		streamOut << sVertex;
	}

	constexpr int k3{ 3 };
	const auto numTriangles = static_cast<int>(mesh.indices_.size()) / k3;

	streamOut << "# " << numTriangles << " faces \n";
	int j{ 0 };
	for (int i = 0; i < numTriangles; i++)
	{
		const uint32_t i0 = mesh.indices_[j++] + 1;
		const uint32_t i1 = mesh.indices_[j++] + 1;
		const uint32_t i2 = mesh.indices_[j++] + 1;

		QString sFace;
		QTextStream(&sFace) << "f " <<
			i0 << "//" << i0 << " " <<
			i1 << "//" << i1 << " " <<
			i2 << "//" << i2 << " " <<
			"\n";
		streamOut << sFace;
	}

	streamOut.flush();
	return sOut;
}
QString geo::Mesh3dSaver::saveToObjString(const std::vector<geo::Mesh3d>& meshes) // NOLINT(readability-convert-member-functions-to-static)
{
	QString sOut;
	QTextStream streamOut(&sOut);

	int numVerticesWritten{ 0 };
	const auto numMeshes = static_cast<int>(meshes.size());
	for (int m = 0; m < numMeshes; m++)
	{
		const geo::Mesh3d& mesh = meshes[m];
		streamOut << "o objectMesh" << (m + 1) << "\n";

		const auto numVertices = static_cast<int>(mesh.vertices_.size());
		streamOut << "# " << numVertices << " vertices \n";
		for (int i = 0; i < numVertices; i++)
		{
			const geo::Point3f& v = mesh.vertices_[i];
			QString sVertex;
			QTextStream(&sVertex) << "v " << v.x_ << " " << v.y_ << " " << v.z_ << "\n";
			streamOut << sVertex;
		}

		streamOut << "# " << numVertices << " normals \n";
		for (int i = 0; i < numVertices; i++)
		{
			const geo::Point3f& v = mesh.normals_[i];
			QString sVertex;
			QTextStream(&sVertex) << "vn " << v.x_ << " " << v.y_ << " " << v.z_ << "\n";
			streamOut << sVertex;
		}

		constexpr int k3{ 3 };
		const auto numTriangles = static_cast<int>(mesh.indices_.size()) / k3;

		streamOut << "# " << numTriangles << " faces \n";
		int j{ 0 };
		for (int i = 0; i < numTriangles; i++)
		{
			const uint32_t i0 = mesh.indices_[j++] + numVerticesWritten + 1;
			const uint32_t i1 = mesh.indices_[j++] + numVerticesWritten + 1;
			const uint32_t i2 = mesh.indices_[j++] + numVerticesWritten + 1;

			QString sFace;
			QTextStream(&sFace) << "f " <<
				i0 << "//" << i0 << " " <<
				i1 << "//" << i1 << " " <<
				i2 << "//" << i2 << " " <<
				"\n";
			streamOut << sFace;
		}
		numVerticesWritten += numVertices;
	} // for m
	streamOut.flush();
	return sOut;
}

