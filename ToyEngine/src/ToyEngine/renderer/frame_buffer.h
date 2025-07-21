#pragma once
#include <cstdint>
#include "ToyEngine/core.h"

namespace ToyEngine
{
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {};

		/**
		* @brief Binds the frame buffer for use in the rendering pipeline.
		*/
		virtual void Bind() const = 0;

		/**
		* @brief Unbinds the frame buffer from the rendering pipeline.
		*/
		virtual void Unbind() const = 0;

		/**
		* @brief Gets the color attachment texture ID.
		* @return The OpenGL texture ID of the color attachment.
		*/
		virtual uint32_t GetColorAttachment() const = 0;

		/**
		* @brief Gets the depth attachment texture ID.
		* @return The OpenGL texture ID of the depth attachment.
		*/
		virtual uint32_t GetDepthAttachment() const = 0;

		/**
		* @brief Gets the width of the frame buffer.
		* @return The width in pixels.
		*/
		uint32_t GetWidth() const { return width_; }

		/**
		* @brief Gets the height of the frame buffer.
		* @return The height in pixels.
		*/
		uint32_t GetHeight() const { return height_; }

		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);
	protected:
		FrameBuffer(uint32_t width, uint32_t height) : width_(width), height_(height) {}
	protected:
		uint32_t id_, width_, height_;
	};
}