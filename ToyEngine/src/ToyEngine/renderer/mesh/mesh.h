#pragma once

namespace ToyEngine
{
	class Mesh
	{
	public:
		virtual ~Mesh();
	
		virtual void Draw() const;

	protected:
		unsigned int vao_, vbo_, ebo_;
		unsigned int vertex_count_;
	};
}
