#include "pch.h"
#include "layer.h"
#include "ToyEngine/services/window.h"

// imgui
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace ToyEngine
{
  ImGuiLayer::ImGuiLayer(Window& window)
    : blocks_event_(true)
  {
    impl_ = IImGuiImpl::Create(window);
  }

  void ImGuiLayer::OnAttach()
  {
    TY_CORE_ASSERT(ImGui::GetCurrentContext() == nullptr, "ImGui - Already been initialized context.");
    
    // Setup Dear ImGui context
    // -------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io_ = ImGui::GetIO(); (void)io_;
    io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
    io_.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls 
    io_.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking 
    io_.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style 
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io_.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 10.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends 
    TY_CORE_ASSERT(io_.BackendPlatformUserData == nullptr, "ImGui - Already initialized a platform backend.");
    impl_->Init();
    // -------------------------
  }

  void ImGuiLayer::OnDetatch()
  {
    // Cleanup imgui
    impl_->Shutdown();
    ImGui::DestroyContext();
  }
  void ImGuiLayer::BeginDraw()
  {
    // ImGui
    // ------------------------------
    // Start the Dear Imgui frame
    impl_->NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiLayer::EndDraw()
  {
    // Rendering
    ImGui::Render();
    impl_->EndFrame();
  }

  void ImGuiLayer::OnEvent(Event& e)
  {
    if (blocks_event_)
    {
      // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
      // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
      ImGuiIO& io_ = ImGui::GetIO(); (void)io_;
      if (io_.WantCaptureMouse == true || io_.WantCaptureKeyboard == true)
      {
        e.SetEventHandled(true);
      }
    }
  }

  Scope<IImGuiImpl> ToyEngine::IImGuiImpl::Create(Window& window)
  {
#ifdef  TY_PLATFORM_WINDOWS
    return MakeScope<ImGuiImplGLFW>(static_cast<GLFWwindow*>(window.GetNativeWindow()));
#else
    TY_CORE_ERROR("Platform not supported")
    return nullptr;
#endif
  }

  void ImGuiImplGLFW::Init()
  {
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
  }

  void ImGuiImplGLFW::NewFrame()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
  }

  void ImGuiImplGLFW::EndFrame()
  {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
  }

  void ImGuiImplGLFW::Shutdown()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
  }
}