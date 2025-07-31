-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]          = "%{wks.location}/ToyEngine/vendor/GLFW/include"
IncludeDir["glad"]          = "%{wks.location}/ToyEngine/vendor/glad/include"
IncludeDir["stb"]           = "%{wks.location}/ToyEngine/vendor/stb"
IncludeDir["glm"]           = "%{wks.location}/ToyEngine/vendor/glm"
IncludeDir["spdlog"]        = "%{wks.location}/ToyEngine/vendor/spdlog/include"
IncludeDir["dearimgui"]     = "%{wks.location}/ToyEngine/vendor/dearimgui"
IncludeDir["assimp"]        = "%{wks.location}/ToyEngine/vendor/assimp/include"
IncludeDir["assimp_build"]  = "%{wks.location}/ToyEngine/vendor/assimp/build/" .. outputdir .. "/include"
IncludeDir["googletest"]    = "%{wks.location}/ToyEngine/vendor/googletest/googletest/include"
IncludeDir["googlemock"]    = "%{wks.location}/ToyEngine/vendor/googletest/googlemock/include"

-- Library directories relative to root folder (solution directory)
LibDir = {}
LibDir["assimp"] = "%{wks.location}/ToyEngine/vendor/assimp/bin/" .. outputdir .. "/assimp"