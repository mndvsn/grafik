project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    systemversion "latest"

    targetdir ("bin/" .. output_dir .. "/%{ prj.name }")
    objdir ("intermediate/" .. output_dir .. "/%{ prj.name }")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release or Dist" }
        runtime "Release"
        optimize "speed"
