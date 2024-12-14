#pragma once
#include "ToyEngine/Renderer/mesh/mesh.h"
namespace ToyEngine
{
	class Plane : public Mesh
	{
	public:
		Plane();
	private:
		float vertices_[30] = {
			// positions			// uv
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,	// top right 
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	// top left

			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f	// top left
		};
	};
}