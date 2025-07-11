#pragma once
#include "ToyEngine/core.h"
#include <cstdint>

namespace ToyEngine
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		/**
		* @brief Binds the vertex buffer for use in the rendering pipeline.
		*/
		virtual void Bind() const = 0;

		/**
		* @brief Unbinds the vertex buffer from the rendering pipeline.
		*/
		virtual void Unbind() const = 0;

		/**
		* @brief Sets data in the vertex buffer.
		* @param data Pointer to the vertex data to upload.
		* @param size The size in bytes of the data to upload.
		*/
		virtual void SetData(const void* data, uint32_t size) = 0;

		/**
		* @brief Gets the size of the vertex buffer.
		* @return The size in bytes of the buffer.
		*/
		uint32_t GetSize() const { return size_; }

		/**
		* @brief Creates a vertex buffer with the specified size.
		* @param size The size in bytes of the buffer to create.
		* @return A shared pointer to the created vertex buffer.
		*/
		static Ref<VertexBuffer> Create(uint32_t size);

		/**
		* @brief Creates a vertex buffer with the specified data.
		* @param data Pointer to the vertex data to upload.
		* @param size The size in bytes of the data to upload.
		* @return A shared pointer to the created vertex buffer.
		*/
		static Ref<VertexBuffer> Create(const void* data, uint32_t size);

	protected:
		VertexBuffer(uint32_t size) : size_(size) {}

	protected:
		uint32_t id_ = 0;
		uint32_t size_ = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		/**
		* @brief Binds the index buffer for use in the rendering pipeline.
		*/
		virtual void Bind() const = 0;

		/**
		* @brief Unbinds the index buffer from the rendering pipeline.
		*/
		virtual void Unbind() const = 0;

		/**
		* @brief Sets data in the index buffer.
		* @param indices Pointer to the index data to upload.
		* @param count The number of indices to upload.
		*/
		virtual void SetData(const uint32_t* indices, uint32_t count) = 0;

		/**
		* @brief Gets the number of indices in the buffer.
		* @return The number of indices.
		*/
		uint32_t GetCount() const { return count_; }

		/**
		* @brief Gets the capacity of the index buffer.
		* @return The maximum number of indices the buffer can hold.
		*/
		uint32_t GetCapacity() const { return capacity_; }

		/**
		* @brief Creates an index buffer with the specified indices.
		* @param indices Pointer to the index data to upload.
		* @param count The number of indices to upload.
		* @return A shared pointer to the created index buffer.
		*/
		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);

	protected:
		IndexBuffer(uint32_t count) : count_(count), capacity_(count) {}

	protected:
		uint32_t id_ = 0;
		uint32_t count_ = 0;
		uint32_t capacity_ = 0;
	};
}