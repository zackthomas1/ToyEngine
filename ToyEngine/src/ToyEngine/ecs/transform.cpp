#include "pch.h"
#include "ToyEngine/ecs/transform.h"
#include <algorithm>

namespace ToyEngine
{
	Transform::Transform()
		: local_position_(0.0f, 0.0f, 0.0f)
		, local_rotation_(1.0f, 0.0f, 0.0f, 0.0f)  // Identity quaternion
		, local_scale_(1.0f, 1.0f, 1.0f)
	{
	}

	Transform::Transform(const glm::vec3& position)
		: local_position_(position)
		, local_rotation_(1.0f, 0.0f, 0.0f, 0.0f)  // Identity quaternion
		, local_scale_(1.0f, 1.0f, 1.0f)
	{
	}

	Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
		: local_position_(position)
		, local_rotation_(rotation)
		, local_scale_(scale)
	{
	}

	void Transform::SetLocalPosition(const glm::vec3& position)
	{
		local_position_ = position;
		MarkDirty();
	}

	void Transform::SetLocalRotation(const glm::quat& rotation)
	{
		local_rotation_ = rotation;
		MarkDirty();
	}

	void Transform::SetLocalScale(const glm::vec3& scale)
	{
		local_scale_ = scale;
		MarkDirty();
	}

	glm::vec3 Transform::GetWorldPosition() const
	{
		if (parent_ == nullptr)
		{
			return local_position_;
		}
		
		glm::mat4 world_matrix = GetWorldMatrix();
		return glm::vec3(world_matrix[3]);
	}

	glm::quat Transform::GetWorldRotation() const
	{
		if (parent_ == nullptr)
		{
			return local_rotation_;
		}
		
		return parent_->GetWorldRotation() * local_rotation_;
	}

	glm::vec3 Transform::GetWorldScale() const
	{
		if (parent_ == nullptr)
		{
			return local_scale_;
		}
		
		return parent_->GetWorldScale() * local_scale_;
	}

	const glm::mat4& Transform::GetLocalMatrix() const
	{
		if (local_matrix_dirty_)
		{
			UpdateLocalMatrix();
		}
		return local_matrix_;
	}

	glm::mat4 Transform::GetWorldMatrix() const
	{
		if (parent_ == nullptr)
		{
			return GetLocalMatrix();
		}
		
		return parent_->GetWorldMatrix() * GetLocalMatrix();
	}

	void Transform::Translate(const glm::vec3& offset)
	{
		local_position_ += offset;
		MarkDirty();
	}

	void Transform::Rotate(const glm::quat& rotation)
	{
		local_rotation_ = rotation * local_rotation_;
		MarkDirty();
	}

	void Transform::Rotate(float angle, const glm::vec3& axis)
	{
		glm::quat rotation = glm::angleAxis(angle, glm::normalize(axis));
		Rotate(rotation);
	}

	void Transform::Scale(const glm::vec3& scale)
	{
		local_scale_ *= scale;
		MarkDirty();
	}

	glm::vec3 Transform::GetForward() const
	{
		glm::quat world_rotation = GetWorldRotation();
		return world_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
	}

	glm::vec3 Transform::GetRight() const
	{
		glm::quat world_rotation = GetWorldRotation();
		return world_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	glm::vec3 Transform::GetUp() const
	{
		glm::quat world_rotation = GetWorldRotation();
		return world_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	void Transform::SetParent(Transform* parent)
	{
		// Remove from current parent
		if (parent_ != nullptr)
		{
			parent_->RemoveChild(this);
		}
		
		// Set new parent
		parent_ = parent;
		
		// Add to new parent
		if (parent_ != nullptr)
		{
			parent_->AddChild(this);
		}
		
		MarkDirty();
	}

	void Transform::MarkDirty()
	{
		if (!dirty_)
		{
			dirty_ = true;
			local_matrix_dirty_ = true;
			
			// Mark all children as dirty too
			for (Transform* child : children_)
			{
				child->MarkDirty();
			}
		}
	}

	void Transform::UpdateLocalMatrix() const
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), local_position_);
		glm::mat4 rotation = glm::mat4_cast(local_rotation_);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), local_scale_);
		
		local_matrix_ = translation * rotation * scale;
		local_matrix_dirty_ = false;
	}

	void Transform::AddChild(Transform* child)
	{
		if (child != nullptr && std::find(children_.begin(), children_.end(), child) == children_.end())
		{
			children_.push_back(child);
		}
	}

	void Transform::RemoveChild(Transform* child)
	{
		auto it = std::find(children_.begin(), children_.end(), child);
		if (it != children_.end())
		{
			children_.erase(it);
		}
	}
}