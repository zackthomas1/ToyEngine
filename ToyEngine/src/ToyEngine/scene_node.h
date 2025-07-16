#pragma once
#include "core.h"
#include "ToyEngine/model/model.h"

namespace ToyEngine {
	class SceneNode {
	public:
			SceneNode(const char* name) :
				name_(name),
				is_dirty(false),
				local_transform_(glm::mat4(1.0f)),
				world_transform_(glm::mat4(1.0f)),
				parent_(nullptr) { }
			SceneNode(const char* name, std::shared_ptr<Model> model) :
				name_(name),
				is_dirty(false),
				local_transform_(glm::mat4(1.0f)),
				world_transform_(glm::mat4(1.0f)),
				entity_(model),
				parent_(nullptr) { }

			bool SetParent(SceneNode* parent);
			bool AddChild(Scope<SceneNode> child);
			const std::string& GetName() const { return name_; }
			const Vector<Scope<SceneNode>>& GetChildren() const { return children_; }
			const glm::mat4& GetWorldTransform() const { return world_transform_; }
			std::shared_ptr<Model> GetEntity() const { return entity_; }
			void SetLocalTransform(glm::mat4& mat);
			void MarkDirty() { is_dirty = true; }
			void UpdateWorldTransform();

			Vector<Scope<SceneNode>>::iterator ChildrenBegin() { return children_.begin(); }
			Vector<Scope<SceneNode>>::iterator ChildrenEnd() { return children_.end(); }
			Vector<Scope<SceneNode>>::const_iterator ChildrenBegin() const { return children_.begin(); }
			Vector<Scope<SceneNode>>::const_iterator ChildrenEnd() const { return children_.end(); }

	private:
			std::string name_;
			bool is_dirty;
			glm::mat4 local_transform_;
			glm::mat4 world_transform_;
			Vector<Scope<SceneNode>> children_;
			SceneNode* parent_;
			Ref<Model> entity_;
	};
}