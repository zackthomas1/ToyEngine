#pragma once
#include "ToyEngine/services/time_step.h"
#include <glm/mat4x4.hpp>

namespace ToyEngine {
  class Entity
  {
  public:
    virtual ~Entity() {}
  
    virtual void Update(const TimeStep& time_step) {}
    virtual void Render(const glm::mat4& world_transform) const {}
  protected:
    Entity() {}
  };
}