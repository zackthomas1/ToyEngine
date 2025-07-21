#pragma once
#include "ToyEngine/core.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace ToyEngine
{
	// Forward declarations to avoid circular dependencies
	class Model;
	class Mesh;
	class Shader;

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

	// Command to set polygon mode (wireframe, filled, etc.)
	class SetPolygonModeCommand : public IRenderCommand
	{
	public:
		SetPolygonModeCommand(uint32_t face, uint32_t mode);
		virtual void Execute() override;

	private:
		uint32_t m_face;
		uint32_t m_mode;
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

		// Reserve space for commands to avoid reallocations
		void Reserve(size_t capacity) { m_commands.reserve(capacity); }

	private:
		std::vector<Scope<IRenderCommand>> m_commands;
	};
}