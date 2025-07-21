#include "pch.h"
#include "render_command_queue.h"
#include "ToyEngine/renderer/render_api.h"
#include "ToyEngine/model/model.h"
#include "ToyEngine/model/mesh.h"
#include "ToyEngine/renderer/shader.h"

namespace ToyEngine
{
	// DrawMeshCommand implementation
	DrawMeshCommand::DrawMeshCommand(Ref<Model> model, const glm::mat4& world_transform)
		: m_model(model), m_world_transform(world_transform)
	{
	}

	void DrawMeshCommand::Execute()
	{
		if (!m_model) return;

		Ref<Shader> shader = m_model->m_shader;
		shader->Use();
		
		for (Ref<Mesh> mesh : m_model->m_meshes) {
			shader->SetMat4("uModel", m_world_transform);
			mesh->m_material->BindTextures(shader);

			// draw mesh
			RenderCommand::BindVertexArray(mesh->m_vao);
			RenderCommand::DrawIndexed(mesh->m_indices.size());
			RenderCommand::BindVertexArray(0);
		}
	}

	// ClearBackgroundCommand implementation
	ClearBackgroundCommand::ClearBackgroundCommand(const glm::vec4& clear_color)
		: m_clear_color(clear_color)
	{
	}

	void ClearBackgroundCommand::Execute()
	{
		RenderCommand::ClearSetBackground(m_clear_color);
	}

	// SetPolygonModeCommand implementation
	SetPolygonModeCommand::SetPolygonModeCommand(uint32_t face, uint32_t mode)
		: m_face(face), m_mode(mode)
	{
	}

	void SetPolygonModeCommand::Execute()
	{
		RenderCommand::PolygonMode(m_face, m_mode);
	}

	// RenderCommandQueue implementation
	void RenderCommandQueue::Enqueue(Scope<IRenderCommand> command)
	{
		if (command) {
			m_commands.push_back(std::move(command));
		}
	}

	void RenderCommandQueue::ExecuteAll()
	{
		for (auto& command : m_commands) {
			if (command) {
				command->Execute();
			}
		}
		Clear();
	}

	void RenderCommandQueue::Clear()
	{
		m_commands.clear();
	}
}