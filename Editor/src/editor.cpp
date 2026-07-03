#include <toy_engine.h>
//----------- entry point -------------
#include "ToyEngine/entry_point.h"
//-------------------------------------
#include "viewport.h"

void DrawSceneNodeTree(ToyEngine::SceneNode* node) {
  if(!node) return; 
  if (ImGui::TreeNode(node->GetName().c_str())) {
    for (const auto& child : node->GetChildren()) {
      DrawSceneNodeTree(child.get());
    }
    ImGui::TreePop();
  }
}

class Scene : public ToyEngine::Layer
{
public:
  Scene(ToyEngine::ResourceManager& resource_manager, ToyEngine::InputPoll& input) :
    m_resource_manager(resource_manager), m_input(input) { }

  virtual void OnAttach()
  {
    TY_INFO("Compiling shaders...");
    ToyEngine::Ref<ToyEngine::Shader> flatShader = ToyEngine::Shader::Create("flat_color", "../assets/shaders/flat_color.vs", "../assets/shaders/flat_color.fs");
    ToyEngine::Ref<ToyEngine::Shader> textureShader = ToyEngine::Shader::Create("flat_texture", "../assets/shaders/flat_texture.vs", "../assets/shaders/flat_texture.fs");
    ToyEngine::Ref<ToyEngine::Shader> phongShader = ToyEngine::Shader::Create("phong", "../assets/shaders/phong.vs", "../assets/shaders/phong.fs");
    ToyEngine::Ref<ToyEngine::Shader> skyboxShader = ToyEngine::Shader::Create("skybox", "../assets/shaders/skybox.vs", "../assets/shaders/skybox.fs");
    ToyEngine::Ref<ToyEngine::Shader> postfxShader = ToyEngine::Shader::Create("postfx", "../assets/shaders/post_process.vs", "../assets/shaders/post_process.fs");

    m_resource_manager.Add<ToyEngine::Shader>(flatShader->GetName(), flatShader);
    m_resource_manager.Add<ToyEngine::Shader>(textureShader->GetName(), textureShader);
    m_resource_manager.Add<ToyEngine::Shader>(phongShader->GetName(), phongShader);
    m_resource_manager.Add<ToyEngine::Shader>(skyboxShader->GetName(), skyboxShader);
    m_resource_manager.Add<ToyEngine::Shader>(postfxShader->GetName(), postfxShader);

    ToyEngine::Ref<ToyEngine::UniformBuffer> view_project_mat = ToyEngine::UniformBuffer::Create("ViewProjectMats", 2 * sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(float));
    ToyEngine::Ref<ToyEngine::UniformBuffer> light_block = ToyEngine::UniformBuffer::Create("LightBlock", sizeof(ToyEngine::LightBlock));

    m_resource_manager.Add<ToyEngine::UniformBuffer>(view_project_mat->GetName(), view_project_mat);
    m_resource_manager.Add<ToyEngine::UniformBuffer>(light_block->GetName(), light_block);

    flatShader->BindUniformBlock(view_project_mat->GetName(), view_project_mat->GetBindPoint());
    textureShader->BindUniformBlock(view_project_mat->GetName(), view_project_mat->GetBindPoint());
    phongShader->BindUniformBlock(view_project_mat->GetName(), view_project_mat->GetBindPoint());
    phongShader->BindUniformBlock(light_block->GetName(), light_block->GetBindPoint());
    skyboxShader->BindUniformBlock(view_project_mat->GetName(), view_project_mat->GetBindPoint());

    TY_INFO("Creating Camera...");
    ToyEngine::CameraControllerProps camera_control_props;
    camera_control_props.type = ToyEngine::eCameraControllerType::kFly;
    m_camera_controller = ToyEngine::MakeRef<ToyEngine::CameraController>(m_input, camera_control_props);

    TY_INFO("Creating scene...");
    m_scene_graph = ToyEngine::MakeScope<ToyEngine::SceneNode>("root");

    ToyEngine::Ref<ToyEngine::Model> backpack = (ToyEngine::Model::Create("../assets/models/backpack/backpack.obj", true));
    ToyEngine::Ref<ToyEngine::Model> cyborg = (ToyEngine::Model::Create("../assets/models/cyborg/cyborg.obj", false));
    backpack->m_shader = phongShader;
    cyborg->m_shader = phongShader;

    m_resource_manager.Add<ToyEngine::Model>("backpack", backpack);
    m_resource_manager.Add<ToyEngine::Model>("cyborg", cyborg);

    m_scene_graph->AddChild(ToyEngine::MakeScope<ToyEngine::SceneNode>("backepack_model", backpack));
    m_scene_graph->AddChild(ToyEngine::MakeScope<ToyEngine::SceneNode>("cyborg_model", cyborg));

    TY_INFO("Creating Environment Map...");
    ToyEngine::Ref<ToyEngine::TextureCube> sky_texture = ToyEngine::TextureCube::Create({
      "../assets/cubemaps/skybox/right.jpg", // +X (right)
      "../assets/cubemaps/skybox/left.jpg", // -X (left)
      "../assets/cubemaps/skybox/top.jpg", // +Y (top)
      "../assets/cubemaps/skybox/bottom.jpg", // -Y (bottom)
      "../assets/cubemaps/skybox/front.jpg", // +Z (front)
      "../assets/cubemaps/skybox/back.jpg", // -Z (back)
      });

    ToyEngine::Ref<ToyEngine::Skybox> skybox = ToyEngine::MakeRef<ToyEngine::Skybox>(sky_texture, skyboxShader);
    m_scene_graph->AddChild(ToyEngine::MakeScope<ToyEngine::SceneNode>("skybox", skybox));
    for (auto& mesh : backpack->m_meshes) {
      mesh->m_material->SetEnvironmentMap(sky_texture);
    }
    for (auto& mesh : cyborg->m_meshes) {
      mesh->m_material->SetEnvironmentMap(sky_texture);
    }

    TY_INFO("Creating lights...");
    m_light_block = ToyEngine::MakeScope<ToyEngine::LightBlock>();
    m_light_block->m_num_lights = 5;
    m_light_block->m_lights[0].m_type = int(ToyEngine::eLightType::kDirectional);
    m_light_block->m_lights[1].m_type = int(ToyEngine::eLightType::kPoint);
    m_light_block->m_lights[2].m_type = int(ToyEngine::eLightType::kPoint);
    m_light_block->m_lights[3].m_type = int(ToyEngine::eLightType::kPoint);
    m_light_block->m_lights[4].m_type = int(ToyEngine::eLightType::kSpot);
    m_light_block->m_lights[0].m_enabled = true;
    m_light_block->m_lights[1].m_enabled = true;
    m_light_block->m_lights[2].m_enabled = true;
    m_light_block->m_lights[3].m_enabled = true;
    m_light_block->m_lights[4].m_enabled = true;

    TY_INFO("Creating Viewport...");
    m_viewport.SetCameraController(m_camera_controller);

    TY_INFO("Creating Postfx Framebuffer...");
    ToyEngine::FrameBufferProps fb_props; 
    m_postfx_framebuffer = ToyEngine::FrameBuffer::Create(fb_props);
    m_quad_vertex_array = ToyEngine::VertexArray::Create();
    ToyEngine::Ref<ToyEngine::VertexBuffer>quad_vertex_buffer = ToyEngine::VertexBuffer::Create(ToyEngine::TextureQuadPrim::m_vertices.data(),
      sizeof(ToyEngine::TextureQuadPrim::m_vertices));
    quad_vertex_buffer->SetLayout(ToyEngine::TextureQuadPrim::m_layout);
    m_quad_vertex_array->AddBuffer(quad_vertex_buffer);
    ToyEngine::Ref<ToyEngine::IndexBuffer>quad_index_buffer = ToyEngine::IndexBuffer::Create(ToyEngine::TextureQuadPrim::m_indices.data(),
      ToyEngine::TextureQuadPrim::m_indices.size());
    m_quad_vertex_array->SetIndexBuffer(quad_index_buffer);
  }

  virtual void OnDetach() {}

  virtual void Update(float time_delta)
  {
    // Update camera
    if (m_viewport.GetProps().is_hovered)
      m_camera_controller->Update(time_delta);

    // Update lights
    m_light_block->m_lights[0].m_direction  = glm::vec4(m_directional_light_dir,0.0f);
    m_light_block->m_lights[0].m_value      = glm::vec4(m_directional_light_color, 0.0f);
    m_light_block->m_lights[1].m_position   = glm::vec4(m_point_light_position_1, 0.0f);
    m_light_block->m_lights[1].m_value      = glm::vec4(m_point_light_color_1, 0.0f);
    m_light_block->m_lights[2].m_position   = glm::vec4(m_point_light_position_2, 0.0f);
    m_light_block->m_lights[2].m_value      = glm::vec4(m_point_light_color_2, 0.0f);
    m_light_block->m_lights[3].m_position   = glm::vec4(m_point_light_position_3, 0.0f);
    m_light_block->m_lights[3].m_value      = glm::vec4(m_point_light_color_3, 0.0f);

    // update model material properties
    if (m_scene_graph->GetChildren().size() >= 2) {
      auto& backpack_node = m_scene_graph->GetChildren()[0];
      auto& cyborg_node = m_scene_graph->GetChildren()[1];

      if (auto& backpack_model = std::dynamic_pointer_cast<ToyEngine::Model>(backpack_node->GetEntity())) {
        for (auto& mesh : backpack_model->m_meshes) {
          mesh->m_material->SetRoughness(m_roughness);
          mesh->m_material->SetMetallic(m_metallic);
          mesh->m_material->SetTransmission(m_transmission);
          mesh->m_material->SetRefractiveIndex(m_refractive_index);
        }
      }
      if (auto& cyborg_model = std::dynamic_pointer_cast<ToyEngine::Model>(cyborg_node->GetEntity())) {
        for (auto& mesh : cyborg_model->m_meshes) {
          mesh->m_material->SetRoughness(m_roughness);
          mesh->m_material->SetMetallic(m_metallic);
          mesh->m_material->SetTransmission(m_transmission);
          mesh->m_material->SetRefractiveIndex(m_refractive_index);
        }
      }
    }

    // update model transforms
    m_scene_graph->SetLocalTransform(glm::translate(glm::mat4(1.0f), m_translate));
    m_scene_graph->GetChildren()[1]->SetLocalTransform(
      glm::rotate(glm::translate(glm::mat4(1.0f), m_translate_cyborg),
      glm::radians(m_rotation_degree), glm::vec3(0.0f, 1.0f, 0.0f)));
  }

  virtual void OnRender()
  {
    // Update uniform buffer objects
    ToyEngine::Ref<ToyEngine::UniformBuffer> camera_uniforms = m_resource_manager.Get<ToyEngine::UniformBuffer>("ViewProjectMats");
    camera_uniforms->SetData(0, sizeof(glm::mat4), glm::value_ptr(m_camera_controller->GetCamera().GetViewMatrix()));
    camera_uniforms->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera_controller->GetCamera().GetProjectionMatrix()));
    camera_uniforms->SetData(2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(m_camera_controller->GetCamera().GetProps().position));

    ToyEngine::Ref<ToyEngine::UniformBuffer> light_uniforms = m_resource_manager.Get<ToyEngine::UniformBuffer>("LightBlock");
    light_uniforms->SetData(0, sizeof(ToyEngine::LightBlock), m_light_block.get());

    // Draw Scene to frame buffer
    m_viewport.GetProps().framebuffer->Bind();
    ToyEngine::Renderer::BeginScene();
    ToyEngine::Renderer::Submit(m_scene_graph.get());
    ToyEngine::Renderer::EndScene();
    m_viewport.GetProps().framebuffer->Unbind();

    //// Draw quad with post-processing
    //ToyEngine::Ref<ToyEngine::Shader> postfxShader = m_shader_lib->Get("postfx");
    //postfxShader->Use();
    //postfxShader->SetInt("screenTexture", 0);
    //bool depth_test;
    //ToyEngine::RenderCommand::GetBooleanv(ToyEngine::eParamType::kDEPTH_TEST, &depth_test);
    //ToyEngine::RenderCommand::Disable(ToyEngine::eParamType::kDEPTH_TEST);
    //ToyEngine::RenderCommand::ClearSetBackground();
    //ToyEngine::RenderCommand::BindTexture(ToyEngine::eSamplerType::kTexture2D, m_viewport_framebuffer->GetColorAttachment());
    //ToyEngine::Renderer::Submit(m_quad_vertex_array.get());
    //if (depth_test) ToyEngine::RenderCommand::Enable(ToyEngine::eParamType::kDEPTH_TEST);
  }

  virtual void OnImGuiRender() 
  {
    // Define the GUI windows
    // ------------------------------
    ImGuiIO& io_ = ImGui::GetIO();
    (void)io_;

    // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
    // You can browse its code to learn more about Dear ImGui!).
    ImGui::ShowDemoWindow();

    // Main Dockspace
    {
      static bool dockspace_open  = true;
      static bool opt_fullscreen_persistant = true; 
      bool opt_fullscreen = true;
      static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

      // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
      // because it would be confusing to have two docking targets within each others.
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
      if (opt_fullscreen) 
      {
        ImGuiViewport* viewport = ImGui::GetMainViewport(); 
        ImGui::SetNextWindowPos(viewport->Pos); 
        ImGui::SetNextWindowSize(viewport->Size); 
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
      }

      // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
      // and handle the pass-thru hole, so we ask Begin() to not render a background.
      if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

      // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
      // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
      // all active windows docked into it will lose their parent and become undocked.
      // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
      // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f)); 
      ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags); 
      ImGui::PopStyleVar();

      if (opt_fullscreen)
        ImGui::PopStyleVar(2); 

      // DockSpace
      if (io_.ConfigFlags & ImGuiConfigFlags_DockingEnable)
      {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace"); 
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
      }

      if (ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("File"))
        {
          if (ImGui::MenuItem("Exit")) TY_WARN("TODO:: Implement application close option"); 
          ImGui::EndMenu(); 
        }
        ImGui::EndMenuBar(); 
      }
      
      m_viewport.ImGuiRender();

      // Scene Graph
      {
        ImGui::Begin("Scene Graph");
        DrawSceneNodeTree(m_scene_graph.get());
        ImGui::End();
      }

      // Controls
      {
        ImGui::Begin("Controls");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);

        if (ImGui::CollapsingHeader("Scene Controls", ImGuiTreeNodeFlags_DefaultOpen))
        {
          ImGui::DragFloat3("Translate Root##TranslateRoot", glm::value_ptr(m_translate), 0.1f, -10.0f, 10.0f, "%.1f");
          ImGui::DragFloat3("Translate Cyborg##TranslateCyborg", glm::value_ptr(m_translate_cyborg), 0.1f, -10.0f, 10.0f, "%.1f");
          ImGui::DragFloat("Rotate Cyborg##RotateCyborg", &m_rotation_degree, 0.1f, 0.0f, 360.0f, "%.1f", ImGuiSliderFlags_WrapAround);
        }

        if (ImGui::CollapsingHeader("Material Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::DragFloat("Roughness##MaterialRoughness", &m_roughness, 0.1f, 0.1f, 256.0f, "%.1f");
          ImGui::DragFloat("Metallic##MaterialMetallic", &m_metallic, 0.01f, 0.00f, 1.00f, "%.2f");
          ImGui::DragFloat("Transmission##MaterialMetallic", &m_transmission, 0.01f, 0.00f, 1.00f, "%.2f");
          ImGui::DragFloat("IOR##MaterialIOR", &m_refractive_index, 0.1f, 0.1f, 100.0f, "%.2f");
        }

        if (ImGui::CollapsingHeader("Light Controls", ImGuiTreeNodeFlags_DefaultOpen))
        {
          ImGui::Text("Directional Light");
          ImGui::ColorEdit3("Color##DirectionalLightColor", glm::value_ptr(m_directional_light_color), ImGuiColorEditFlags_Float);
          ImGui::DragFloat3("Direction##DirectionalLightDirection", glm::value_ptr(m_directional_light_dir), 0.01f, -1.0f, 1.0f, "%.2f");

          ImGui::Text("Point Light 1");
          ImGui::ColorEdit3("Color##PointLight1Color", glm::value_ptr(m_point_light_color_1), ImGuiColorEditFlags_Float);
          ImGui::DragFloat3("Position##PointLight1Position", glm::value_ptr(m_point_light_position_1), 0.01f, 0.0f, 0.0f, "%.2f");

          ImGui::Text("Point Light 2");
          ImGui::ColorEdit3("Color##PointLight2Color", glm::value_ptr(m_point_light_color_2), ImGuiColorEditFlags_Float);
          ImGui::DragFloat3("Position##PointLight2Position", glm::value_ptr(m_point_light_position_2), 0.01f, 0.0f, 0.0f, "%.2f");

          ImGui::Text("Point Light 3");
          ImGui::ColorEdit3("Color##PointLight3Color", glm::value_ptr(m_point_light_color_3), ImGuiColorEditFlags_Float);
          ImGui::DragFloat3("Position##PointLight3Position", glm::value_ptr(m_point_light_position_3), 0.01f, 0.0f, 0.0f, "%.2f");

          ImGui::Text("Spot Light");
          ImGui::ColorEdit3("Color##SpotLightColor", glm::value_ptr(m_spot_light_color), ImGuiColorEditFlags_Float);
        }
        // Controls end
        ImGui::End();
      }
      // DockSpace Demo end
      ImGui::End();
    }
  }

  virtual void OnEvent(ToyEngine::Event& e)
  {
    if (m_viewport.GetProps().is_hovered) {
      // Dispatch to viewport
      ToyEngine::EventDispatcher dispatcher(e);
      dispatcher.Dispatch<ToyEngine::Event>(TY_BINDFN(m_viewport.OnEvent));
    }
  }

public:
  ToyEngine::InputPoll& m_input;
  
  ToyEngine::ResourceManager& m_resource_manager;

  ToyEngine::Scope<ToyEngine::SceneNode> m_scene_graph;

  //
  ToyEngine::Ref<ToyEngine::FrameBuffer> m_postfx_framebuffer;
  ToyEngine::Ref<ToyEngine::VertexArray> m_quad_vertex_array;
  ToyEngine::Ref<ToyEngine::CameraController> m_camera_controller;

  // Model control parameters
  float m_rotation_degree = 0;
  glm::vec3 m_translate = glm::vec3(0.0f), m_translate_cyborg = glm::vec3(2.5f, -1.5f, 0.0f);
  float m_roughness = 32.0f, m_metallic = 0.0f, m_transmission = 0.0f, m_refractive_index = 1.52f;

  // Light control parameters
  ToyEngine::Scope<ToyEngine::LightBlock> m_light_block;
  glm::vec3 m_directional_light_color = glm::vec3(1.0f), m_directional_light_dir = glm::vec3(0.0, 0.0, -1.0);
  glm::vec3 m_point_light_color_1 = glm::vec3(0.5, 0.0, 0.0), m_point_light_color_2 = glm::vec3(0.0, 0.5, 0.0), m_point_light_color_3 = glm::vec3(0.0, 0.0, 0.5);
  glm::vec3 m_point_light_position_1 = glm::vec3(1.0, 0.0, 1.0), m_point_light_position_2 = glm::vec3(0.0, 1.0, 1.0), m_point_light_position_3 = glm::vec3(-1.0, 0.0, 1.0);
  glm::vec3 m_spot_light_color = glm::vec3(0.5, 0.5, 0.0);

  Viewport m_viewport;
};

class Editor : public ToyEngine::Application
{
public:
  Editor()
  {
    TY_INFO("Initialize application");
    PushLayer(
      new Scene(
        Application::GetServices().Get<ToyEngine::ResourceManager>(),
        Application::GetServices().Get<ToyEngine::InputPoll>()));
  }
  ~Editor() { }
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
  return new Editor();
}