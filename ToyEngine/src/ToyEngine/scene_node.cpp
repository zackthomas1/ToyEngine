#include "pch.h"
#include "scene_node.h"

namespace ToyEngine {
	bool SceneNode::SetParent(SceneNode* parent)
	{
			if (parent_ == parent)
					return false; // No change in parent

			// Remove this node from the current parent's children if applicable
			if (parent_)
			{
					auto& siblings = parent_->children_;
					siblings.erase(
							std::remove_if(siblings.begin(), siblings.end(),
									[this](const std::unique_ptr<SceneNode>& ptr) { return ptr.get() == this; }),
							siblings.end());
			}

			// Set the new parent
			parent_ = parent;

			return true;
	}

	bool SceneNode::AddChild(Scope<SceneNode> child)
	{
		if (!child)
				return false; // Child is null

		// Prevent adding the same child twice
		for (const auto& c : children_) {
				if (c.get() == child.get())
						return false;
		}

		child->SetParent(this);
		children_.push_back(std::move(child));
			return true;
	}

	void SceneNode::UpdateWorldTransform()
	{
		if (parent_)
				world_transform_ = parent_->GetWorldTransform() * local_transform_;
		else
				world_transform_ = local_transform_;

		if (is_dirty) {
			for (auto& child : children_) {
				child->UpdateWorldTransform();
			}
			is_dirty = false;
	}
	}

	void SceneNode::SetLocalTransform(glm::mat4& mat)
	{
		local_transform_ = mat;
		MarkDirty();
		for (auto& child : children_)
			child->MarkDirty();
	}
}