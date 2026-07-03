#pragma once

namespace ToyEngine
{
  class FileManager
  {
  public:
    FileManager() = delete;
    ~FileManager() = delete;

      static std::string ReadSourceFile(const std::string& path);
  private:

  };
}