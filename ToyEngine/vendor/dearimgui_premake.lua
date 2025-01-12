-- dearimgui project --
project "imgui"
    location "ToyEngine/vendor/dearimgui"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{IncludeDir.dearimgui}/imconfig.h",
        "%{IncludeDir.dearimgui}/imgui.h",
        "%{IncludeDir.dearimgui}/imgui.cpp",
        "%{IncludeDir.dearimgui}/imgui_draw.cpp",
        "%{IncludeDir.dearimgui}/imgui_internal.h",
        "%{IncludeDir.dearimgui}/imgui_widgets.cpp",
        "%{IncludeDir.dearimgui}/imstb_rectpack.h",
        "%{IncludeDir.dearimgui}/imstb_textedit.h",
        "%{IncludeDir.dearimgui}/imstb_truetype.h",
        "%{IncludeDir.dearimgui}/imgui_demo.cpp",
        "%{IncludeDir.dearimgui}/imgui_tables.cpp",

        "%{IncludeDir.dearimgui}/backends/imgui_impl_glfw.cpp",
        "%{IncludeDir.dearimgui}/backends/imgui_impl_opengl3.cpp",

        "%{IncludeDir.dearimgui}/misc/cpp/imgui_stdlib.h",
        "%{IncludeDir.dearimgui}/misc/cpp/imgui_stdlib.cpp",

        "%{IncludeDir.dearimgui}/misc/debugger/imgui.natvis",
        "%{IncludeDir.dearimgui}/misc/debugger/imgui.natstepfilter",
    }

    includedirs
    {
        "%{IncludeDir.dearimgui}",
        "%{IncludeDir.dearimgui}/backends",
        "%{IncludeDir.dearimgui}/misc/cpp",
        "%{IncludeDir.GLFW}",
    }

    links
    {
        "GLFW",
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"