#pragma once
#include <cstdint>
#include "ToyEngine/core.h"
#include "ToyEngine/renderer/shader.h"

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
    UniformManager() {}
    ~UniformManager() {}
    
    /**
    * @brief Creates a new UniformBuffer with the specified name and size, and adds it to the manager.
    * Binds to the next available binding point index and increments the internal counter 
    * @param name The name to associate with the new UniformBuffer.
    * @param size The size in bytes of the UniformBuffer to create.
    */
    void CreateBuffer(std::string name, uint32_t size);

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
    * @brief Binds a named uniform block in the given shader to the binding point of the associated UniformBuffer.
    *
    * This function looks up the UniformBuffer associated with the given uniform block name,
    * retrieves its binding point, and binds the uniform block in the specified shader to that binding point.
    *
    * @param shader The shader in which to bind the uniform block.
    * @param ubo_name The name of the uniform block to bind.
    */
    void BindUniformBlockToShader(Ref<Shader> shader, const char* ubo_name);

    /**
    * @brief Checks if a UniformBuffer with the specified name exists in the manager.
    * @param name The name to check.
    * @return True if the buffer exists, false otherwise.
    */
    bool HasBuffer(const std::string& name) const;
  private:
    std::unordered_map<std::string, Ref<UniformBuffer>> buffer_lib_;
    
        static uint32_t s_next_binding_point;
  };
}