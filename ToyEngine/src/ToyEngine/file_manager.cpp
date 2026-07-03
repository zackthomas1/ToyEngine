#include "pch.h"
#include "file_manager.h"

namespace ToyEngine
{
  std::string FileManager::ReadSourceFile(const std::string& path)
  {
    // retrieve source code from filePath
    std::string source_code;
    std::ifstream source_file;

    // ensure ifstream objects can throw exceptions
    source_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
      // open files
      source_file.open(path);

      // read file's buffer contents into streams 
      std::stringstream source_stream;
      source_stream << source_file.rdbuf();

      // close file handlers 
      source_file.close();

      // convert stream into string 
      source_code = source_stream.str();
    }
    catch (std::ifstream::failure e)
    {
      TY_CORE_ERROR("File not successfully read.");
    }
    return source_code;
  }
}