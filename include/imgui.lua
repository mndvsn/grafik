project "ImGui"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    systemversion "latest"

    targetdir ("../bin/" .. output_dir .. "/%{ prj.name }")
    objdir ("../intermediate/" .. output_dir .. "/%{ prj.name }")

    files
    {
        "imgui/imconfig.h",
        "imgui/imgui.h",
        "imgui/imgui.cpp",
        "imgui/imgui_internal.h",
        "imgui/imstb_rectpack.h",
        "imgui/imstb_textedit.h",
        "imgui/imstb_truetype.h",
        "imgui/imgui_demo.cpp",
        "imgui/imgui_draw.cpp",
        "imgui/imgui_tables.cpp",
        "imgui/imgui_widgets.cpp",

        "imgui/backends/imgui_impl_glfw.h",
        "imgui/backends/imgui_impl_glfw.cpp",

        "imgui/backends/imgui_impl_opengl3.h",
        "imgui/backends/imgui_impl_opengl3_loader.h",
        "imgui/backends/imgui_impl_opengl3.cpp",

        "imgui/backends/imgui_impl_vulkan.h",
        "imgui/backends/imgui_impl_vulkan.cpp",
    }

    links "GLFW"

    includedirs
    {
        "imgui",
        "glfw/include",
        "%{ VULKAN_SDK }/Include"
    }

    libdirs
    {
        "%{ VULKAN_SDK }/Lib"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release or Dist" }
        runtime "Release"
        optimize "speed"
