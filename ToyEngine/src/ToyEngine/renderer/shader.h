#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ToyEngine
{
	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void Use() = 0;
		virtual void SetBool(const std::string& name, bool value) const = 0; 
		virtual void SetInt(const std::string& name, int value) const = 0;
		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetFloat2(const std::string& name, float v0, float v1) const = 0;
		virtual void SetFloat3(const std::string& name, float v0, float v1, float v2) const = 0;
		virtual void SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const = 0;
		virtual void SetMat4(const std::string& name, glm::mat4 value) const = 0;

		uint32_t id() const { return id_; }
		const char* GetName() const { return m_name; }

		static Ref<Shader> Create(const char* shader_name, const char* vertex_path, const char* fragment_path);
	public:
		const char* m_name;
	protected:
		// constructor reads and builds the shader
		Shader(const char* name) : m_name(name) {}
		uint32_t id_;
	};

    // ShaderLibrary manages a collection of Shader objects, allowing shaders to be loaded and retrieved by name.
    // It stores shaders in an unordered_map for efficient lookup and ensures that each shader is uniquely identified by its name.
    class ShaderLibrary
    {
    public:
		ShaderLibrary() {}
		~ShaderLibrary() {}

		void Load(Ref<Shader> shader);
		Ref<Shader> Get(const std::string& name);
    private:
		std::unordered_map<std::string, Ref<Shader>> library_;
    };
}