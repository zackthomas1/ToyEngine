#pragma once
#include "ToyEngine/core.h"
#include <cstdint>

namespace ToyEngine
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {};

		/**
		* @brief Binds the uniform buffer for use in the rendering pipeline.
		*/
		virtual void Bind() const = 0;

		/**
		* @brief Unbinds the uniform buffer from the rendering pipeline.
		*/
		virtual void Unbind() const = 0;

		/**
		* @brief Sets data in the uniform buffer at the specified offset and size.
		* @param offset The offset in bytes from the start of the buffer.
		* @param size The size in bytes of the data to set.
		* @param data Pointer to the data to copy into the buffer.
		*/
		virtual void SetData(uint32_t offset, uint32_t size, const void* data) = 0;

		/**
		* @brief Binds the uniform buffer to a specific binding point.
		* @param binding_point The binding point index to bind the buffer to.
		*/
		virtual void BindToBindingPoint(uint32_t binding_point) const = 0;

		/**
		* @brief Gets the binding point index associated with this uniform buffer.
		* @return The binding point index.
		*/
		uint32_t GetBindPoint() const { return binding_point_; }
	
		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding_point = 0);
	protected:
		UniformBuffer(uint32_t size, uint32_t binding_point) : size_(size), binding_point_(binding_point) {}
	protected:
		uint32_t id_, size_, binding_point_;
	};

	class UniformManager
	{
	public:
		UniformManager() : m_binding_index(0) {}
		~UniformManager() {}

		/**
		* @brief Adds a UniformBuffer to the manager with the specified name.
		* @param name The name to associate with the UniformBuffer.
		* @param uniform_buffer The UniformBuffer to add.
		*/
		void Add(std::string name, Ref<UniformBuffer> uniform_buffer);

		/**
		* @brief Retrieves the UniformBuffer associated with the given name.
		* @param name The name of the UniformBuffer to retrieve.
		* @return Reference to the UniformBuffer, or nullptr if not found.
		*/
		Ref<UniformBuffer> GetBuffer(const std::string& name) const;

		/**
		* @brief Gets the binding point of the UniformBuffer associated with the given name.
		* @param name The name of the UniformBuffer.
		* @return The binding point index.
		*/
		uint32_t GetBindPoint(const std::string& name) const;

		/**
		* @brief Checks if a UniformBuffer with the specified name exists in the manager.
		* @param name The name to check.
		* @return True if the buffer exists, false otherwise.
		*/
		bool HasBuffer(const std::string& name) const;

		/**
		* @brief Returns the next available binding point index and increments the internal counter.
		* @return The next binding point index.
		*/
		uint32_t GetNextBindingPoint() { return m_binding_index++; }
	private:
		uint32_t m_binding_index;
		std::unordered_map<std::string, Ref<UniformBuffer>> buffer_lib_;
	};
}