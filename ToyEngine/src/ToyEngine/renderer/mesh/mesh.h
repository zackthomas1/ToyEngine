#pragma once

namespace ToyEngine
{
	struct AttribPointerData
	{
		unsigned int size, stride, offset;
	};

	class Mesh
	{
	public:
		virtual ~Mesh();
	
		// getters
		unsigned int GetVBO() { return vbo_; }
		unsigned int GetVAO() { return vao_; }
		unsigned int GetVertexCount() { return vertex_count_; }

	protected:
		unsigned int vao_, vbo_, ebo_;
		unsigned int vertex_count_;
		std::vector<AttribPointerData> attribs_;
	};
}
