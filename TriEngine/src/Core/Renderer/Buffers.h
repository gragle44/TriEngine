#pragma once

namespace TriEngine {
	enum class ShaderDataType : uint8_t {
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
		case TriEngine::ShaderDataType::None:
			return 0;
		case TriEngine::ShaderDataType::Float:
			return 4;
		case TriEngine::ShaderDataType::Float2:
			return 4 * 2;
		case TriEngine::ShaderDataType::Float3:
			return 4 * 3;
		case TriEngine::ShaderDataType::Float4:
			return 4 * 4;
		case TriEngine::ShaderDataType::Int:
			return 4;
		case TriEngine::ShaderDataType::Int2:
			return 4 * 2;
		case TriEngine::ShaderDataType::Int3:
			return 4 * 3;
		case TriEngine::ShaderDataType::Int4:
			return 4 * 4;
		case TriEngine::ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case TriEngine::ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case TriEngine::ShaderDataType::Bool:
			return 1;
		default:
			TRI_CORE_ASSERT(false, "Invalid ShaderDataType!");
			return 0;
		}
	}


	struct BufferElement {
		std::string Name;
		ShaderDataType DataType;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement(const std::string& name, ShaderDataType dataType, bool normalized = false)
			:Name(name), DataType(dataType), Size(ShaderDataTypeSize(dataType)), Normalized(normalized) {
			
		}

		uint32_t GetComponentCount() const
		{
			switch (DataType)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			TRI_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements) 
		{
			CalculateElementOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;

		void CalculateElementOffsetsAndStride() {
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		static VertexBuffer* Create(float* vertices, uint32_t size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetID() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetID() const = 0;

		virtual uint32_t GetCount() const = 0;

	};
}
