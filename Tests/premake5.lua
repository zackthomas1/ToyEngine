-- Tests project
project "Tests"
    location "Tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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
        "ToyEngine/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.dearimgui}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.assimp_build}",
        "%{IncludeDir.googletest}",
        "%{IncludeDir.googlemock}",
    }

    libdirs
    {
        "%{LibDir.assimp}",
    }
    
    links
    {
        "ToyEngine",
        "GLFW",
        "glad",
        "opengl32.lib",
        "ImGui",
        "assimp",
        "googletest",
    }

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

        defines
        {
            "TY_PLATFORM_WINDOWS",
            "TY_PLATFORM_OPENGL",
            "GLFW_INCLUDE_NONE",
        }

        filter "configurations:Debug"
            defines {"TY_DEBUG", "TY_ENABLE_ASSERTS"}
            runtime "Debug"
            symbols "on"
            links { "zlibstaticd" } -- link the debug‐suffix zlib

        filter "configurations:Release"
            defines {"TY_RELEASE"}
            runtime "Release"
            optimize "on"
            links { "zlibstatic" }  -- link the release zlib

        filter "configurations:Dist"
            defines {"TY_DIST"}
            runtime "Release"
            optimize "on"
            links { "zlibstatic" }  -- link the release zlib