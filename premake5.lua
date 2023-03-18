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

    targetdir ("bin/" .. output_dir)
    objdir ("intermediate/" .. output_dir)

    pchheader "gpch.h"
    pchsource "src/gpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",

        "data/**",

        "include/stb/**.h",
        "include/stb/**.cpp",

        "include/imgui/imgui.h",
        "include/imgui/imgui_internal.h",
        "include/imgui/imconfig.h",
        "include/imgui/imstb_rectpack.h",
        "include/imgui/imstb_textedit.h",
        "include/imgui/imstb_truetype.h",
        "include/imgui/backends/imgui_impl_glfw.h",
        "include/imgui/backends/imgui_impl_opengl3_loader.h",
        "include/imgui/backends/imgui_impl_opengl3.cpp",

        "include/imgui/imgui.cpp",
        "include/imgui/imgui_demo.cpp",
        "include/imgui/imgui_draw.cpp",
        "include/imgui/imgui_tables.cpp",
        "include/imgui/imgui_widgets.cpp",
        "include/imgui/backends/imgui_impl_glfw.cpp",
        "include/imgui/backends/imgui_impl_opengl3.h",
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
        "glad"
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
