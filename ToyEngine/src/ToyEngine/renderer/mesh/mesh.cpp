#include "pch.h"
#include "mesh.h"

#include "ToyEngine/renderer/render_api.h"

namespace ToyEngine
{
	Mesh::~Mesh()
	{
		RenderAPI::DeleteMesh(this);
	}
}