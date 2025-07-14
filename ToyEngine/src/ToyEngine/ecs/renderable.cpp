#include "pch.h"
#include "ToyEngine/ecs/renderable.h"

namespace ToyEngine
{
	Renderable::Renderable(Ref<Model> model)
		: model_(model)
	{
	}

	Renderable::Renderable(Ref<Model> model, Ref<Shader> shader)
		: model_(model), shader_(shader)
	{
	}
}