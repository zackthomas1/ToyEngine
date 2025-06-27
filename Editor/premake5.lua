-- Application Project
project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h", 
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/ToyEngine/src", 
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.dearimgui}",
        "%{IncludeDir.dearimgui}/backends",
    }

    links 
    {
        "ToyEngine"
    }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/utf-8" }

        defines
        {
            "TY_PLATFORM_WINDOWS",
        }

        filter "configurations:Debug"
            defines {"TY_DEBUG"}
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