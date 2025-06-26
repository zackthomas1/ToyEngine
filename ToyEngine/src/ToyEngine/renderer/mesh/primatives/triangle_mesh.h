#pragma once
#include "ToyEngine/renderer/mesh/mesh.h"

namespace ToyEngine
{
	class TriangleMesh : public Mesh
	{
	public:
		TriangleMesh();
		~TriangleMesh() {}
	private:
		float vertices_[15] = {
			// positions         // uv
			 0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  // bottom left
			 0.0f,  0.5f, 0.0f,  0.5f,  1.0f,  // top 		
		};
	};
}