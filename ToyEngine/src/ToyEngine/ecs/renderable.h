#pragma once
#include "ToyEngine/ecs/component.h"
#include "ToyEngine/model/model.h"
#include "ToyEngine/renderer/shader.h"

namespace ToyEngine
{
	/// <summary>
	/// Renderable component allows entities to be rendered.
	/// It integrates with the existing ToyEngine renderer system.
	/// </summary>
	class Renderable : public TypedComponent<Renderable>
	{
	public:
		/// <summary>
		/// Creates a renderable component
		/// </summary>
		Renderable() = default;

		/// <summary>
		/// Creates a renderable component with a model
		/// </summary>
		/// <param name="model">Model to render</param>
		Renderable(Ref<Model> model);

		/// <summary>
		/// Creates a renderable component with a model and shader
		/// </summary>
		/// <param name="model">Model to render</param>
		/// <param name="shader">Shader to use for rendering</param>
		Renderable(Ref<Model> model, Ref<Shader> shader);

		/// <summary>
		/// Gets the model to render
		/// </summary>
		/// <returns>Model reference</returns>
		Ref<Model> GetModel() const { return model_; }

		/// <summary>
		/// Sets the model to render
		/// </summary>
		/// <param name="model">New model to render</param>
		void SetModel(Ref<Model> model) { model_ = model; }

		/// <summary>
		/// Gets the shader to use for rendering
		/// </summary>
		/// <returns>Shader reference</returns>
		Ref<Shader> GetShader() const { return shader_; }

		/// <summary>
		/// Sets the shader to use for rendering
		/// </summary>
		/// <param name="shader">New shader to use</param>
		void SetShader(Ref<Shader> shader) { shader_ = shader; }

		/// <summary>
		/// Checks if the renderable has a valid model
		/// </summary>
		/// <returns>True if model is valid</returns>
		bool HasModel() const { return model_ != nullptr; }

		/// <summary>
		/// Checks if the renderable has a valid shader
		/// </summary>
		/// <returns>True if shader is valid</returns>
		bool HasShader() const { return shader_ != nullptr; }

		/// <summary>
		/// Checks if the renderable is ready for rendering
		/// </summary>
		/// <returns>True if both model and shader are valid</returns>
		bool IsReadyForRendering() const { return HasModel() && HasShader(); }

		/// <summary>
		/// Checks if this renderable should be rendered
		/// </summary>
		/// <returns>True if renderable should be rendered</returns>
		bool IsVisible() const { return visible_; }

		/// <summary>
		/// Sets the visibility of this renderable
		/// </summary>
		/// <param name="visible">Whether the renderable should be visible</param>
		void SetVisible(bool visible) { visible_ = visible; }

		/// <summary>
		/// Gets the render layer/priority
		/// </summary>
		/// <returns>Render layer</returns>
		int GetRenderLayer() const { return render_layer_; }

		/// <summary>
		/// Sets the render layer/priority
		/// </summary>
		/// <param name="layer">New render layer</param>
		void SetRenderLayer(int layer) { render_layer_ = layer; }

		/// <summary>
		/// Checks if this renderable casts shadows
		/// </summary>
		/// <returns>True if casts shadows</returns>
		bool CastsShadows() const { return cast_shadows_; }

		/// <summary>
		/// Sets whether this renderable casts shadows
		/// </summary>
		/// <param name="cast_shadows">Whether to cast shadows</param>
		void SetCastsShadows(bool cast_shadows) { cast_shadows_ = cast_shadows; }

		/// <summary>
		/// Checks if this renderable receives shadows
		/// </summary>
		/// <returns>True if receives shadows</returns>
		bool ReceivesShadows() const { return receive_shadows_; }

		/// <summary>
		/// Sets whether this renderable receives shadows
		/// </summary>
		/// <param name="receive_shadows">Whether to receive shadows</param>
		void SetReceivesShadows(bool receive_shadows) { receive_shadows_ = receive_shadows; }

	private:
		Ref<Model> model_;
		Ref<Shader> shader_;
		bool visible_ = true;
		int render_layer_ = 0;
		bool cast_shadows_ = true;
		bool receive_shadows_ = true;
	};
}