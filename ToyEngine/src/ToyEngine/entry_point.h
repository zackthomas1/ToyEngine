#pragma once
extern ToyEngine::Application* ToyEngine::CreateApplication();

int main(int argc, char** argv)
{
  // Initalize Logging
  ToyEngine::Log::Init();
  TY_CORE_INFO("Initialized logging");

  auto app = ToyEngine::CreateApplication();
  app->Run();
  delete app;
}