#include "pch.h"
#include "texture_cube.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_texture_cube.h"

namespace ToyEngine {
    Ref<TextureCube> TextureCube::Create(Array<std::string, CUBE_SIDES>& files)
    {
        switch (Renderer::API())
        {
        case(eRenderAPI::kOpenGL): {
            return MakeRef<OpenGLTextureCube>(files);
        }
        default:
            TY_CORE_ERROR("Unknown render api");
            return nullptr;
        }
    }
}

