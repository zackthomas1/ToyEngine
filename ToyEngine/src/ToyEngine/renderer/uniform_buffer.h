#pragma once
#include <cstdint>
#include "ToyEngine/core.h"
#include "ToyEngine/renderer/shader.h"

namespace ToyEngine
{
  class UniformBuffer
  {
  public:
    virtual ~UniformBuffer() = default;

    /// @brief Binds the uniform buffer for use in the rendering pipeline.
    virtual void Bind() const = 0;

    /// @brief Unbinds the uniform buffer from the rendering pipeline.
    virtual void Unbind() const = 0;

    /// @brief Sets data in the uniform buffer at the specified offset and size.
    /// @param offset The offset in bytes from the start of the buffer.
    /// @param size The size in bytes of the data to set.
    /// @param data Pointer to the data to copy into the buffer.
    virtual void SetData(uint32_t offset, uint32_t size, const void* data) = 0;

    /// @brief Binds the uniform buffer to a specific binding point.
    /// @param binding_point The binding point index to bind the buffer to.
    virtual void BindToBindingPoint(uint32_t binding_point) const = 0;

    /// @brief Gets the binding point index associated with this uniform buffer.
    /// @return The binding point index.
    uint32_t GetBindPoint() const { return binding_point_; }

    /// @brief Gets the name of the uniform buffer.
    /// @return The name of the uniform buffer.
    const std::string& GetName() const { return name_; }

    /// @brief Creates a new UniformBuffer of the appropriate type based on the current rendering API.
    /// @param size The size in bytes of the UniformBuffer to create.
    /// @return A reference to the newly created UniformBuffer.
    static Ref<UniformBuffer> Create(const std::string& name, uint32_t size);
  protected:
    UniformBuffer(const std::string& name, uint32_t size, uint32_t binding_point)
      : name_(name), size_(size), binding_point_(binding_point) {}
  protected:
    std::string name_;
    uint32_t id_, size_, binding_point_;
    static uint32_t s_next_binding_point;
  };
}