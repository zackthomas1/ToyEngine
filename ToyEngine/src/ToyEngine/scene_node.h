#pragma once

#include <memory>
#include <vector>
#include "ToyEngine/model/model.h"
#include "core.h"

namespace ToyEngine {
	   class SceneNode {
	   public:
			   SceneNode() :
					   is_dirty(false),
					   local_transform_(glm::mat4(1.0f)),
					   world_transform_(glm::mat4(1.0f)),
					   parent_(nullptr) { }
			   SceneNode(std::shared_ptr<Model> model) :
					   is_dirty(false),
					   local_transform_(glm::mat4(1.0f)),
					   world_transform_(glm::mat4(1.0f)),
					   entity_(model),
					   parent_(nullptr) { }

			   bool SetParent(SceneNode* parent);
			   bool AddChild(std::unique_ptr<SceneNode> child);
			   const std::vector<std::unique_ptr<SceneNode>>& GetChildren() const { return children_; }
			   const glm::mat4& GetWorldTransform() const { return world_transform_; }
			   std::shared_ptr<Model> GetEntity() const { return entity_; }
			   void SetLocalTransform(glm::mat4& mat);
			   void MarkDirty() { is_dirty = true; }
			   void UpdateWorldTransform();

	   private:
			   bool is_dirty;
			   glm::mat4 local_transform_;
			   glm::mat4 world_transform_;
			   std::vector<std::unique_ptr<SceneNode>> children_;
			   SceneNode* parent_;
			   std::shared_ptr<Model> entity_;
	   };
}