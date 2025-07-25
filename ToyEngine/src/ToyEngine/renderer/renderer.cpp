#include "pch.h"
#include "renderer.h"
#include "ToyEngine/services/locator.h"
#include "ToyEngine/renderer/render_api.h"
#include "ToyEngine/skybox.h"

namespace ToyEngine
{
	Renderer* Renderer::s_instance = nullptr; 

	Renderer::Renderer(eRenderAPI api, SceneData data) : api_(api), m_data(data)
	{
	}

	void Renderer::Init()
	{
#ifdef  TY_PLATFORM_OPENGL
		eRenderAPI api = eRenderAPI::kOpenGL;
#else
		eRenderAPI api = eRenderAPI::kNone;
#endif TY_PLATFORM_OPENGL
		TY_CORE_ASSERT(!s_instance, "Renderer already exist!"); 
		s_instance = new Renderer(api);
		RenderAPI::Init(api);

		// Enable face culling for performance
		RenderCommand::Enable(eParamType::kCULL_FACE);
		RenderCommand::CullFace(eCullFaceMode::kBACK);		// Cull back faces
		RenderCommand::FrontFace(eFrontFace::kCCW);			// Counter-clockwise winding is front face

		Renderer::GetUniformManager().CreateBuffer("ViewProjectMats", 2 * sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(float));
		Renderer::GetUniformManager().CreateBuffer("LightBlock", sizeof(LightBlock));
	}

	void Renderer::BeginScene(const Camera* camera, const LightBlock* light_block)
	{
		// Clear the background to prepare for rendering a new frame.
		RenderCommand::ClearSetBackground();

		// Update uniform buffer objects
		Ref<UniformBuffer> camera_uniforms = Renderer::GetUniformManager().GetBuffer("ViewProjectMats");
		camera_uniforms->SetData(0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
		camera_uniforms->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetProjectionMatrix()));
		camera_uniforms->SetData(2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(camera->position()));
		
		Ref<UniformBuffer> light_uniforms = Renderer::GetUniformManager().GetBuffer("LightBlock");
		//TY_CORE_ASSERT(sizeof(LightBlock) == 1024, "LightBlock Incorrect size");
		light_uniforms->SetData(0, sizeof(LightBlock), light_block);
	}

	void Renderer::Submit(SceneNode* node)
	{
		if(!node) return;

		node->UpdateWorldTransform();


		// Render non-skybox entities first
		if (node->GetEntity() && !dynamic_cast<Skybox*>(node->GetEntity().get()))
			node->GetEntity()->Render(node->GetWorldTransform());

		for (const Scope<SceneNode>& child : node->GetChildren()) {
			Renderer::Submit(child.get());
		}

		// Render skybox entities first
		if (node->GetEntity() && dynamic_cast<Skybox*>(node->GetEntity().get()))
			node->GetEntity()->Render(node->GetWorldTransform());
	}

	void Renderer::EndScene() {}

	void Renderer::SetFaceCulling(bool enabled, eCullFaceMode cullMode, eFrontFace frontFace)
	{
		if (enabled) {
			RenderCommand::Enable(eParamType::kCULL_FACE);
			RenderCommand::CullFace(cullMode);
			RenderCommand::FrontFace(frontFace);
		} else {
			RenderCommand::Disable(eParamType::kCULL_FACE);
		}
	}
}