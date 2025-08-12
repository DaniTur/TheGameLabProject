project "GLFW"
	kind "StaicLib"
	language "C"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputdir .. "/%{prj.name}")

	files {
		"include/GLFW/glfw3.h",
		"include/GLFW/glfw3native.h",
		"src/context.c",
		"src/init.c",
		"src/input.c",
		"src/monitor.c", 
		"src/platform.c", 
		"src/vulkan.c",
		"src/window.c"
	}

	filter "system:windows"
		systemversion "latest"	-- to use the latest version of the SDK available
		staticruntime "on"	-- for Visual Studio sets <RuntimeLibrary> to "MultiThreaded", "off" for MultiThreadedDLL

		files {
			"win32_platform.h",
			"win32_joystick.h",
			"win32_init.c",
			"win32_joystick.c",
			"win32_monitor.c",
			"win32_window.c",
			"wgl_context.c"
		}

		defines {
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS",
			"_UNICODE"
		}

	filter "system:linux"
		systemversion "latest"
		staticruntime "on"

		files {
			"x11_platform.h",
			"xkb_unicode.h",
			"x11_init.c",
			"x11_monitor.c",
			"x11_window.c",
			"xkb_unicode.c",
			"glx_context.c",
			"linux_joystick.h",
			"linux_joystick.c"
		}

		defines {
			"_GLFW_X11",
			"_DEFAULT_SOURCE"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
