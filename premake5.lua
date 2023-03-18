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

project "Grafik"
    location ""
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    
    staticruntime "on"
    warnings "extra"
    functionlevellinking "on"
    conformancemode "on"

    targetdir ("bin/%{ cfg.buildcfg }")
    objdir ("intermediate/%{ cfg.buildcfg }")

    pchheader "gpch.h"
    pchsource "src/gpch.cpp"

    files
    {
        "src/**.h",
        "src/**.c",
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
        "include/glad",
        "include/glm",
        "include/GLFW",
        "include/imgui",
        "include/KHR",
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
        "glfw3.lib"
    }

    defines
	{
		"GLFW_INCLUDE_NONE"
	}

    filter "files:src/glad.c"
        flags "NoPCH"

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
        
    filter "configurations:Release"
        runtime "Release"
        optimize "Speed"
        flags "LinkTimeOptimization"
        defines
        {
            "NDEBUG",
            "_CONSOLE"
        }
        
    filter "configurations:Dist"
        kind "WindowedApp"
        runtime "Release"
        optimize "Speed"
        symbols "off"
        flags "LinkTimeOptimization"
        defines
        {
            "NDEBUG",
            "GK_DISTR"
        }
        libdirs
        {
            "lib/GLFW/Release"
        }
