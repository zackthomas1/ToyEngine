#include "pch.h"
#include "model.h"

#include "ToyEngine/renderer/texture/texture_2d.h"

namespace ToyEngine
{
	void Model::SetMaterialUniforms()
	{
		for (int i = 0; i < material_->GetTextures()->size(); i++) {
			int sampler_location = ((*material_->GetTextures())[i]->id()) - 1;
			TY_CORE_INFO("SetMaterialUniforms - name:{}", "text_sample_" + std::to_string(sampler_location));
			shader_->SetInt("text_sample_" + std::to_string(sampler_location), sampler_location);
		}
	}
}