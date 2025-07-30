#pragma once
#include <cstdint>
#include <string>
#include <initializer_list>
#include <vector>

namespace ToyEngine {

	enum class eShaderDataType 
	{
		None	= 0,
		Bool,
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4,
	};

	static  uint32_t ShaderDataTypeSize(eShaderDataType type)
	{
		switch (type)
		{
			case eShaderDataType::Bool	:	return 1;
			case eShaderDataType::Int	:	return 4;
			case eShaderDataType::Float	:	return 4;
			case eShaderDataType::Vec2	:	return 2 * 4;
			case eShaderDataType::Vec3	:	return 3 * 4;
			case eShaderDataType::Vec4	:	return 4 * 4;
			case eShaderDataType::Mat3	:	return 3 * 3 * 4;
			case eShaderDataType::Mat4	:	return 4 * 4 * 4;
		}
		TY_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		eShaderDataType m_type; 
		std::string m_name;
		uint32_t m_size, m_offset; 
		bool m_normalize;

		BufferElement(eShaderDataType type, std::string name, bool normalize = false) 
			: m_type(type), m_name(name), m_size(ShaderDataTypeSize(type)), m_offset(0), m_normalize(normalize) 
		{}

		uint32_t GetComponentCount() const
		{
			switch (m_type)
			{
				case eShaderDataType::Bool	:	return 1;
				case eShaderDataType::Int	:	return 1;
				case eShaderDataType::Float	:	return 1;
				case eShaderDataType::Vec2	:	return 2;
				case eShaderDataType::Vec3	:	return 3;
				case eShaderDataType::Vec4	:	return 4;
				case eShaderDataType::Mat3	:	return 3 * 3;
				case eShaderDataType::Mat4	:	return 4 * 4;
			}
			TY_CORE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements);
		~BufferLayout() {}

		inline const Vector<BufferElement>& GetElements() const  { return elements_; }
		inline const uint32_t GetStride() const { return stride_; }

		Vector<BufferElement>::iterator begin()				{ return elements_.begin(); }
		Vector<BufferElement>::iterator end()				{ return elements_.end(); }
		Vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
		Vector<BufferElement>::const_iterator end()	const	{ return elements_.end(); }
	private:
		void CalculateOffsetAndStride();
		Vector<BufferElement> elements_;
		uint32_t stride_ = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0; 
		virtual void Unbind() const = 0; 
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(const void* data, uint32_t size);
	protected:
		VertexBuffer() {}
	};
	
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0; 
		virtual void Unbind() const = 0; 

		virtual uint32_t GetCount() const = 0; 

		static Ref<IndexBuffer> Create(uint32_t* data, uint32_t count);
	protected:
		IndexBuffer() {}
	};
}