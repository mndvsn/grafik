project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "on"
    systemversion "latest"

    targetdir ("../bin/" .. output_dir .. "/%{ prj.name }")
    objdir ("../intermediate/" .. output_dir .. "/%{ prj.name }")

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	files
	{
		"glfw/include/GLFW/glfw3.h",
		"glfw/include/GLFW/glfw3native.h",
		"glfw/src/glfw_config.h",
		"glfw/src/context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/monitor.c",
		"glfw/src/vulkan.c",
		"glfw/src/window.c"
	}

	filter "system:linux"
		pic "on"
		files
		{
			"src/x11_init.c",
			"src/x11_monitor.c",
			"src/x11_window.c",
			"src/xkb_unicode.c",
			"src/posix_time.c",
			"src/posix_thread.c",
			"src/glx_context.c",
			"src/egl_context.c",
			"src/osmesa_context.c",
			"src/linux_joystick.c"
		}
		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		files
		{
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter { "configurations:Release or Dist" }
        runtime "Release"
        optimize "speed"
