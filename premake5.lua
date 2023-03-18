VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "Grafik"
    architecture "x86_64"
    startproject "Grafik"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile",
        "FatalWarnings"
    }

output_dir = "%{ cfg.buildcfg }"

group "Dependencies"
    include "include/glad"
    include "include/imgui.lua"
group ""

project "Grafik"
    location ""
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    
    staticruntime "on"
    warnings "extra"
    functionlevellinking "on"
    conformancemode "on"

    targetdir ("bin/" .. output_dir .. "/%{ prj.name }")
    objdir ("intermediate/" .. output_dir .. "/%{ prj.name }")

    pchheader "gpch.h"
    pchsource "src/gpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",

        "data/**",

        "include/stb/**.h",
        "include/stb/**.cpp",
    }
    -- temp
    removefiles "src/f3d/**"

    includedirs
    {
        "src",
        "include",
        "include/glad/include",
        "include/glm",
        "include/GLFW",
        "include/imgui",
        "include/spdlog/include",
        "include/stb",
        "%{ VULKAN_SDK }/Include"
    }

    libdirs
    {
        "%{ VULKAN_SDK }/Lib",
        "lib/GLFW/%{ cfg.buildcfg }"
    }

    links
    {
        "vulkan-1.lib",
        "glfw3.lib",
        "glad",
        "imgui"
    }

    defines
	{
		"GLFW_INCLUDE_NONE"
	}

    filter "files:include/**.cpp"
        flags "NoPCH"

    filter "system:windows"
        systemversion "latest"
        links
        {
            "opengl32"
        }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        defines
        {
            "_DEBUG",
            "_CONSOLE"
        }
        
    filter "configurations:Release or Dist"
        runtime "Release"
        optimize "Speed"
        flags "LinkTimeOptimization"
        defines "NDEBUG"

    filter "configurations:Release"
        defines "_CONSOLE"
        
    filter "configurations:Dist"
        kind "WindowedApp"
        symbols "off"
        defines "GK_DISTR"
        libdirs
        {
            "lib/GLFW/Release"
        }
