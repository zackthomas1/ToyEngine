#pragma once
#include "ToyEngine/core.h"
#include "ToyEngine/model/model.h"
#include "ToyEngine/renderer/shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include <memory>

namespace ToyEngine
{
	// Base interface for all render commands
	class IRenderCommand
	{
	public:
		virtual ~IRenderCommand() = default;
		virtual void Execute() = 0;
	};

	// Command to draw a mesh with a specific world transform
	class DrawMeshCommand : public IRenderCommand
	{
	public:
		DrawMeshCommand(Ref<Model> model, const glm::mat4& world_transform);
		virtual void Execute() override;

	private:
		Ref<Model> m_model;
		glm::mat4 m_world_transform;
	};

	// Command to clear the background
	class ClearBackgroundCommand : public IRenderCommand
	{
	public:
		ClearBackgroundCommand(const glm::vec4& clear_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		virtual void Execute() override;

	private:
		glm::vec4 m_clear_color;
	};

	// Command queue that stores and executes render commands
	class RenderCommandQueue
	{
	public:
		RenderCommandQueue() = default;
		~RenderCommandQueue() = default;

		// Add a command to the queue
		void Enqueue(Scope<IRenderCommand> command);

		// Execute all queued commands and clear the queue
		void ExecuteAll();

		// Clear all queued commands without executing them
		void Clear();

		// Get the number of queued commands
		size_t Size() const { return m_commands.size(); }

		// Check if the queue is empty
		bool Empty() const { return m_commands.empty(); }

	private:
		std::vector<Scope<IRenderCommand>> m_commands;
	};
}