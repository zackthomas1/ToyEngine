#include "pch.h"
#include "light_manager.h"
#include <algorithm>

namespace ToyEngine
{
	LightManager::LightManager()
	{
		// Reserve space for lights to avoid frequent reallocations
		m_directionalLights.reserve(MAX_DIRECTIONAL_LIGHTS);
		m_pointLights.reserve(MAX_POINT_LIGHTS);
		m_spotLights.reserve(MAX_SPOT_LIGHTS);
	}

	void LightManager::AddDirectionalLight(const DirectionalLight& light)
	{
		if (m_directionalLights.size() < MAX_DIRECTIONAL_LIGHTS)
		{
			m_directionalLights.push_back(light);
		}
		else
		{
			TY_CORE_WARN("Maximum directional lights ({}) reached. Light not added.", MAX_DIRECTIONAL_LIGHTS);
		}
	}

	void LightManager::AddPointLight(const PointLight& light)
	{
		if (m_pointLights.size() < MAX_POINT_LIGHTS)
		{
			m_pointLights.push_back(light);
		}
		else
		{
			TY_CORE_WARN("Maximum point lights ({}) reached. Light not added.", MAX_POINT_LIGHTS);
		}
	}

	void LightManager::AddSpotLight(const SpotLight& light)
	{
		if (m_spotLights.size() < MAX_SPOT_LIGHTS)
		{
			m_spotLights.push_back(light);
		}
		else
		{
			TY_CORE_WARN("Maximum spot lights ({}) reached. Light not added.", MAX_SPOT_LIGHTS);
		}
	}

	void LightManager::RemoveDirectionalLight(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_directionalLights.size()))
		{
			m_directionalLights.erase(m_directionalLights.begin() + index);
		}
	}

	void LightManager::RemovePointLight(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_pointLights.size()))
		{
			m_pointLights.erase(m_pointLights.begin() + index);
		}
	}

	void LightManager::RemoveSpotLight(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_spotLights.size()))
		{
			m_spotLights.erase(m_spotLights.begin() + index);
		}
	}

	void LightManager::ClearAll()
	{
		m_directionalLights.clear();
		m_pointLights.clear();
		m_spotLights.clear();
	}

	void LightManager::ClearDirectionalLights()
	{
		m_directionalLights.clear();
	}

	void LightManager::ClearPointLights()
	{
		m_pointLights.clear();
	}

	void LightManager::ClearSpotLights()
	{
		m_spotLights.clear();
	}

	DirectionalLight* LightManager::GetDirectionalLight(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_directionalLights.size()))
		{
			return &m_directionalLights[index];
		}
		return nullptr;
	}

	PointLight* LightManager::GetPointLight(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_pointLights.size()))
		{
			return &m_pointLights[index];
		}
		return nullptr;
	}

	SpotLight* LightManager::GetSpotLight(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_spotLights.size()))
		{
			return &m_spotLights[index];
		}
		return nullptr;
	}

	void LightManager::UpdateShaderUniforms(Ref<Shader> shader, const glm::vec3& viewPos)
	{
		shader->Use();
		shader->SetFloat3("viewPos", viewPos.x, viewPos.y, viewPos.z);
		
		UpdateDirectionalLights(shader);
		UpdatePointLights(shader);
		UpdateSpotLights(shader);
	}

	void LightManager::SetDirectionalLightEnabled(int index, bool enabled)
	{
		if (index >= 0 && index < static_cast<int>(m_directionalLights.size()))
		{
			m_directionalLights[index].enabled = enabled;
		}
	}

	void LightManager::SetPointLightEnabled(int index, bool enabled)
	{
		if (index >= 0 && index < static_cast<int>(m_pointLights.size()))
		{
			m_pointLights[index].enabled = enabled;
		}
	}

	void LightManager::SetSpotLightEnabled(int index, bool enabled)
	{
		if (index >= 0 && index < static_cast<int>(m_spotLights.size()))
		{
			m_spotLights[index].enabled = enabled;
		}
	}

	void LightManager::UpdateDirectionalLights(Ref<Shader> shader)
	{
		// Set the number of directional lights
		shader->SetInt("u_numDirectionalLights", static_cast<int>(m_directionalLights.size()));
		
		// Update each directional light
		for (int i = 0; i < static_cast<int>(m_directionalLights.size()); ++i)
		{
			const DirectionalLight& light = m_directionalLights[i];
			std::string baseName = "u_directionalLights[" + std::to_string(i) + "]";
			
			shader->SetFloat3(baseName + ".direction", light.direction.x, light.direction.y, light.direction.z);
			shader->SetFloat3(baseName + ".ambient", light.ambient.x, light.ambient.y, light.ambient.z);
			shader->SetFloat3(baseName + ".diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
			shader->SetFloat3(baseName + ".specular", light.specular.x, light.specular.y, light.specular.z);
			shader->SetBool(baseName + ".enabled", light.enabled);
		}
	}

	void LightManager::UpdatePointLights(Ref<Shader> shader)
	{
		// Set the number of point lights
		shader->SetInt("u_numPointLights", static_cast<int>(m_pointLights.size()));
		
		// Update each point light
		for (int i = 0; i < static_cast<int>(m_pointLights.size()); ++i)
		{
			const PointLight& light = m_pointLights[i];
			std::string baseName = "u_pointLights[" + std::to_string(i) + "]";
			
			shader->SetFloat3(baseName + ".position", light.position.x, light.position.y, light.position.z);
			shader->SetFloat3(baseName + ".ambient", light.ambient.x, light.ambient.y, light.ambient.z);
			shader->SetFloat3(baseName + ".diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
			shader->SetFloat3(baseName + ".specular", light.specular.x, light.specular.y, light.specular.z);
			shader->SetFloat(baseName + ".constant", light.constant);
			shader->SetFloat(baseName + ".linear", light.linear);
			shader->SetFloat(baseName + ".quadratic", light.quadratic);
			shader->SetBool(baseName + ".enabled", light.enabled);
		}
	}

	void LightManager::UpdateSpotLights(Ref<Shader> shader)
	{
		// Set the number of spot lights
		shader->SetInt("u_numSpotLights", static_cast<int>(m_spotLights.size()));
		
		// Update each spot light
		for (int i = 0; i < static_cast<int>(m_spotLights.size()); ++i)
		{
			const SpotLight& light = m_spotLights[i];
			std::string baseName = "u_spotLights[" + std::to_string(i) + "]";
			
			shader->SetFloat3(baseName + ".position", light.position.x, light.position.y, light.position.z);
			shader->SetFloat3(baseName + ".direction", light.direction.x, light.direction.y, light.direction.z);
			shader->SetFloat3(baseName + ".ambient", light.ambient.x, light.ambient.y, light.ambient.z);
			shader->SetFloat3(baseName + ".diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
			shader->SetFloat3(baseName + ".specular", light.specular.x, light.specular.y, light.specular.z);
			shader->SetFloat(baseName + ".innerCone", light.innerCone);
			shader->SetFloat(baseName + ".outerCone", light.outerCone);
			shader->SetFloat(baseName + ".constant", light.constant);
			shader->SetFloat(baseName + ".linear", light.linear);
			shader->SetFloat(baseName + ".quadratic", light.quadratic);
			shader->SetBool(baseName + ".enabled", light.enabled);
		}
	}
}
