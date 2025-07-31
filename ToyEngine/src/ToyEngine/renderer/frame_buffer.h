#pragma once
namespace ToyEngine
{
	struct FrameBufferProps 
	{
		uint32_t m_width			= 800;
		uint32_t m_height			= 600;
		bool m_multisampling		= false;
		uint32_t m_samples			= 1; 
		bool m_depth_attachment		= true;
		bool m_stencil_attachment	= true;

		FrameBufferProps(uint32_t width		= 800,
				uint32_t height				= 600,
				bool multisampling			= false,
				uint32_t samples			= 1,
				bool depth_attachment		= true,
				bool stencil_attachment		= true)
			: m_width(width),
			m_height(height),
			m_multisampling(multisampling),
			m_samples(samples),
			m_depth_attachment(depth_attachment),
			m_stencil_attachment(stencil_attachment) 
		{}

		// Validation method
		bool IsValid() const {
			return m_width > 0 && m_height > 0 && 
					(!m_multisampling || m_samples > 1) &&
					m_samples <= 16; // reasonable limit
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
		uint32_t GetWidth() const { return data_.m_width; }
		uint32_t GetHeight() const { return data_.m_height; }

		static Ref<FrameBuffer> Create(const FrameBufferProps& props = FrameBufferProps());
	protected:
		FrameBuffer(const FrameBufferProps& props);
		
		struct FrameBufferData {
			uint32_t m_width, m_height;
			bool m_multisampling;
			uint32_t m_samples;
			bool m_depth_attachment, m_stencil_attachment;
		};
		FrameBufferData data_;
	};
}