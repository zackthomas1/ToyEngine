#pragma once
#include "light.h"
#include "shader.h"
#include "ToyEngine/core/toyengine_types.h"
#include <memory>
#include <vector>

namespace ToyEngine
{
	// Light manager handles multiple light sources and their uniform updates
	class LightManager
	{
	public:
		static constexpr int MAX_DIRECTIONAL_LIGHTS = 4;
		static constexpr int MAX_POINT_LIGHTS = 32;
		static constexpr int MAX_SPOT_LIGHTS = 16;

		LightManager();
		~LightManager() = default;

		// Add lights to the manager
		void AddDirectionalLight(const DirectionalLight& light);
		void AddPointLight(const PointLight& light);
		void AddSpotLight(const SpotLight& light);

		// Remove lights
		void RemoveDirectionalLight(int index);
		void RemovePointLight(int index);
		void RemoveSpotLight(int index);

		// Clear all lights
		void ClearAll();
		void ClearDirectionalLights();
		void ClearPointLights();
		void ClearSpotLights();

		// Get lights
		const std::vector<DirectionalLight>& GetDirectionalLights() const { return m_directionalLights; }
		const std::vector<PointLight>& GetPointLights() const { return m_pointLights; }
		const std::vector<SpotLight>& GetSpotLights() const { return m_spotLights; }

		// Get light by index
		DirectionalLight* GetDirectionalLight(int index);
		PointLight* GetPointLight(int index);
		SpotLight* GetSpotLight(int index);

		// Update shader uniforms with current light data
		void UpdateShaderUniforms(Ref<Shader> shader, const glm::vec3& viewPos);

		// Enable/disable lights
		void SetDirectionalLightEnabled(int index, bool enabled);
		void SetPointLightEnabled(int index, bool enabled);
		void SetSpotLightEnabled(int index, bool enabled);

		// Get counts
		int GetDirectionalLightCount() const { return static_cast<int>(m_directionalLights.size()); }
		int GetPointLightCount() const { return static_cast<int>(m_pointLights.size()); }
		int GetSpotLightCount() const { return static_cast<int>(m_spotLights.size()); }

	private:
		std::vector<DirectionalLight> m_directionalLights;
		std::vector<PointLight> m_pointLights;
		std::vector<SpotLight> m_spotLights;

		// Helper methods to update specific light types
		void UpdateDirectionalLights(Ref<Shader> shader);
		void UpdatePointLights(Ref<Shader> shader);
		void UpdateSpotLights(Ref<Shader> shader);
	};
}
