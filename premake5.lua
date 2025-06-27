include ("dependencies.lua")

--premake5.lua 
workspace "ToyEngine" 
    architecture "x64"
    startproject "Sandbox"

    configurations 
    { 
        "Debug", 
        "Release", 
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "ToyEngine/vendor/GLFW"
    include "ToyEngine/vendor/glad"
    include("ToyEngine/vendor/dearimgui_premake.lua")
group ""

group "Applications"
    include "Editor"
group ""

-- Toy Engine project --
project "ToyEngine"
    location "ToyEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "ToyEngine/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.dearimgui}",
    }

    links
    {
        "GLFW",
        "glad",
        "opengl32.lib",
        "ImGui"
    }

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

        defines
        {
            "TY_PLATFORM_WINDOWS",
            "TY_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

        filter "configurations:Debug"
            defines {"TY_DEBUG", "TY_ENABLE_ASSERTS"}
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines {"TY_RELEASE"}
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines {"TY_DIST"}
            runtime "Release"
            optimize "on"