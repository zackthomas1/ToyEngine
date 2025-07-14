#pragma once
#include "ToyEngine/ecs/component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ToyEngine
{
	/// <summary>
	/// Transform component handles position, rotation, and scale of entities.
	/// This component is essential for scene graph functionality.
	/// </summary>
	class Transform : public TypedComponent<Transform>
	{
	public:
		/// <summary>
		/// Creates a transform with default values (identity)
		/// </summary>
		Transform();

		/// <summary>
		/// Creates a transform with specified position
		/// </summary>
		/// <param name="position">Initial position</param>
		Transform(const glm::vec3& position);

		/// <summary>
		/// Creates a transform with specified position, rotation, and scale
		/// </summary>
		/// <param name="position">Initial position</param>
		/// <param name="rotation">Initial rotation (quaternion)</param>
		/// <param name="scale">Initial scale</param>
		Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

		/// <summary>
		/// Gets the local position
		/// </summary>
		/// <returns>Local position vector</returns>
		const glm::vec3& GetLocalPosition() const { return local_position_; }

		/// <summary>
		/// Sets the local position
		/// </summary>
		/// <param name="position">New local position</param>
		void SetLocalPosition(const glm::vec3& position);

		/// <summary>
		/// Gets the local rotation
		/// </summary>
		/// <returns>Local rotation quaternion</returns>
		const glm::quat& GetLocalRotation() const { return local_rotation_; }

		/// <summary>
		/// Sets the local rotation
		/// </summary>
		/// <param name="rotation">New local rotation quaternion</param>
		void SetLocalRotation(const glm::quat& rotation);

		/// <summary>
		/// Gets the local scale
		/// </summary>
		/// <returns>Local scale vector</returns>
		const glm::vec3& GetLocalScale() const { return local_scale_; }

		/// <summary>
		/// Sets the local scale
		/// </summary>
		/// <param name="scale">New local scale</param>
		void SetLocalScale(const glm::vec3& scale);

		/// <summary>
		/// Gets the world position (computed from local transform and parent hierarchy)
		/// </summary>
		/// <returns>World position vector</returns>
		glm::vec3 GetWorldPosition() const;

		/// <summary>
		/// Gets the world rotation (computed from local transform and parent hierarchy)
		/// </summary>
		/// <returns>World rotation quaternion</returns>
		glm::quat GetWorldRotation() const;

		/// <summary>
		/// Gets the world scale (computed from local transform and parent hierarchy)
		/// </summary>
		/// <returns>World scale vector</returns>
		glm::vec3 GetWorldScale() const;

		/// <summary>
		/// Gets the local transformation matrix
		/// </summary>
		/// <returns>Local transformation matrix</returns>
		const glm::mat4& GetLocalMatrix() const;

		/// <summary>
		/// Gets the world transformation matrix
		/// </summary>
		/// <returns>World transformation matrix</returns>
		glm::mat4 GetWorldMatrix() const;

		/// <summary>
		/// Translates the transform by the given offset
		/// </summary>
		/// <param name="offset">Translation offset</param>
		void Translate(const glm::vec3& offset);

		/// <summary>
		/// Rotates the transform by the given rotation
		/// </summary>
		/// <param name="rotation">Rotation to apply</param>
		void Rotate(const glm::quat& rotation);

		/// <summary>
		/// Rotates the transform around the given axis by the given angle
		/// </summary>
		/// <param name="angle">Rotation angle in radians</param>
		/// <param name="axis">Rotation axis</param>
		void Rotate(float angle, const glm::vec3& axis);

		/// <summary>
		/// Scales the transform by the given scale factors
		/// </summary>
		/// <param name="scale">Scale factors</param>
		void Scale(const glm::vec3& scale);

		/// <summary>
		/// Gets the forward vector (negative Z in local space)
		/// </summary>
		/// <returns>Forward vector in world space</returns>
		glm::vec3 GetForward() const;

		/// <summary>
		/// Gets the right vector (positive X in local space)
		/// </summary>
		/// <returns>Right vector in world space</returns>
		glm::vec3 GetRight() const;

		/// <summary>
		/// Gets the up vector (positive Y in local space)
		/// </summary>
		/// <returns>Up vector in world space</returns>
		glm::vec3 GetUp() const;

		/// <summary>
		/// Sets the parent transform for hierarchical transformations
		/// </summary>
		/// <param name="parent">Parent transform, or nullptr to remove parent</param>
		void SetParent(Transform* parent);

		/// <summary>
		/// Gets the parent transform
		/// </summary>
		/// <returns>Parent transform, or nullptr if no parent</returns>
		Transform* GetParent() const { return parent_; }

		/// <summary>
		/// Gets the children transforms
		/// </summary>
		/// <returns>Vector of child transforms</returns>
		const Vector<Transform*>& GetChildren() const { return children_; }

		/// <summary>
		/// Checks if this transform has been modified since last update
		/// </summary>
		/// <returns>True if transform has been modified</returns>
		bool IsDirty() const { return dirty_; }

		/// <summary>
		/// Marks the transform as clean (not modified)
		/// </summary>
		void SetClean() { dirty_ = false; }

	private:
		void MarkDirty();
		void UpdateLocalMatrix() const;
		void AddChild(Transform* child);
		void RemoveChild(Transform* child);

	private:
		glm::vec3 local_position_;
		glm::quat local_rotation_;
		glm::vec3 local_scale_;

		mutable glm::mat4 local_matrix_;
		mutable bool local_matrix_dirty_ = true;
		bool dirty_ = true;

		Transform* parent_ = nullptr;
		Vector<Transform*> children_;
	};
}