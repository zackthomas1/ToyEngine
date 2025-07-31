project "googletest"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/ToyEngine/vendor/googletest/googletest/src/gtest-all.cc",
        "%{wks.location}/ToyEngine/vendor/googletest/googlemock/src/gmock-all.cc"
    }

    includedirs
    {
        "%{wks.location}/ToyEngine/vendor/googletest/googletest/include",
        "%{wks.location}/ToyEngine/vendor/googletest/googletest",
        "%{wks.location}/ToyEngine/vendor/googletest/googlemock/include",
        "%{wks.location}/ToyEngine/vendor/googletest/googlemock"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"