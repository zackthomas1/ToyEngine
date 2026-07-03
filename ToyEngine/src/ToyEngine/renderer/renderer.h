#pragma once
#include "ToyEngine/scene_node.h"
#include "ToyEngine/model/model.h"
#include "ToyEngine/renderer/light.h"
#include "ToyEngine/renderer/camera.h"
#include "ToyEngine/renderer/shader.h"
#include "ToyEngine/renderer/uniform_buffer.h"
#include "ToyEngine/renderer/render_api.h"

namespace ToyEngine
{
  class Renderer
  {
  public:
    static void Init();

    /// @brief Begins a new rendering scene by setting up camera matrices and lighting data in the uniform buffer.
    /// This function prepares the renderer for drawing by clearing the background and updating
    /// the uniform buffer with the current camera's view and projection matrices, as well as the provided lighting information.
    /// @param camera The camera to use for view and projection matrices.
    /// @param light_block Pointer to the LightBlock containing scene lighting data.
    static void BeginScene();

    /// @brief Submits a VertexArray for rendering. This function binds the provided VertexArray and issues a draw call.
    /// @param vao Pointer to the VertexArray to be rendered.
    static void Submit(const VertexArray* vao);

    /// @brief Submits a SceneNode and its associated entity for rendering.
    /// Traverses the scene graph starting from the provided node, updating world transforms and submitting each node's entity for rendering.
    /// @param node Pointer to the root SceneNode to start traversal from.
    static void Submit(SceneNode* node);

    /// @brief Ends the current rendering scene. This function can be used to perform any necessary cleanup or state resets after rendering is complete.
    static void EndScene();

    /// @brief Retrieves the current rendering API being used by the renderer.
    /// @return The eRenderAPI enum value representing the current rendering API.
    static eRenderAPI API() { return Renderer::s_instance->api_; }

    ///// @brief Retrieves the UniformManager instance used by the renderer for managing uniform buffers.
    ///// @return Reference to the UniformManager instance.
    //static UniformManager& GetUniformManager() { return Renderer::s_instance->m_uniform_manager; }

  protected:
    /// @brief Constructs a Renderer instance with the specified rendering API and optional scene data.
    /// @param api An enum indicating the rendering API to use (e.g., OpenGL).
    /// @param data Optional SceneData to initialize the renderer with.
    Renderer(eRenderAPI api);
    
    /// Destructor for the Renderer class. Cleans up any resources used by the renderer.
    ~Renderer() = default;
  private:
    /* Pointer to the singleton instance of the Renderer */
    static Renderer* s_instance;
    /* The rendering API being used by the renderer */
    eRenderAPI api_;
    ///* The UniformManager instance used by the renderer for managing uniform buffers */
    //UniformManager m_uniform_manager;
  };
}