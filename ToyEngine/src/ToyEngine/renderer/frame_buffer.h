#pragma once
namespace ToyEngine
{
	struct FrameBufferProps 
	{
		uint32_t width;
		uint32_t height;
		bool multisampling;
		uint32_t samples;
		bool depth_attachment;
		bool stencil_attachment;

		FrameBufferProps(uint32_t width		= TY_DEFAULT_WINDOW_HEIGHT,
				uint32_t height				= TY_DEFAULT_WINDOW_WIDTH,
				bool multisampling			= false,
				uint32_t samples			= 1,
				bool depth_attachment		= true,
				bool stencil_attachment		= true)
			: width(width),
			height(height),
			multisampling(multisampling),
			samples(samples),
			depth_attachment(depth_attachment),
			stencil_attachment(stencil_attachment) 
		{}

		// Validation method
		bool IsValid() const {
			return width > 0 && height > 0 && 
					(!multisampling || samples > 1) &&
					samples <= 16; // reasonable limit
		}
	};
	
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void CopyColorBuffer() const = 0;
		virtual void CopyDepthBuffer() const = 0;
		virtual void CopyStencilBuffer() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Status() const = 0;

		virtual uint32_t GetColorAttachment() const = 0;
		uint32_t GetWidth() const { return data_.width; }
		uint32_t GetHeight() const { return data_.height; }

		static Ref<FrameBuffer> Create(const FrameBufferProps& props = FrameBufferProps());
	protected:
		FrameBuffer(const FrameBufferProps& props);
		
		FrameBufferProps data_;
	};
}