project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputdir .. "/%{prj.name}")

	files {
		"include/GLFW/glfw3.h",
		"include/GLFW/glfw3native.h",
		
		"GLFW/src/internal.h",
		"GLFW/src/platform.h",
		"GLFW/src/mappings.h",

		"GLFW/src/context.c",
		"GLFW/src/init.c",
		"GLFW/src/input.c",
		"GLFW/src/monitor.c", 
		"GLFW/src/platform.c", 
		"GLFW/src/vulkan.c",
		"GLFW/src/window.c",
		"GLFW/src/egl_context.c",
		"GLFW/src/osmesa_context.c",
		    
		"GLFW/src/null_platform.h",
		"GLFW/src/null_joystick.h",
		"GLFW/src/null_init.c",
		"GLFW/src/null_monitor.c",
		"GLFW/src/null_window.c",
		"GLFW/src/null_joystick.c"
	}

	filter "system:windows"
		systemversion "latest"	-- to use the latest version of the SDK available
		staticruntime "on"	-- for Visual Studio sets <RuntimeLibrary> to "MultiThreaded", "off" for MultiThreadedDLL

		files {
			"GLFW/src/win32_time.h",
			"GLFW/src/win32_thread.h",
			"GLFW/src/win32_module.c",
			"GLFW/src/win32_time.c",
			"GLFW/src/win32_thread.c",

			"GLFW/src/win32_platform.h",
			"GLFW/src/win32_joystick.h",
			"GLFW/src/win32_init.c",
			"GLFW/src/win32_joystick.c",
			"GLFW/src/win32_monitor.c",
			"GLFW/src/win32_window.c",
			"GLFW/src/wgl_context.c"
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
			"GLFW/src/x11_platform.h",
			"GLFW/src/.h",
			"GLFW/src/x11_init.c",
			"GLFW/src/x11_monitor.c",
			"GLFW/src/x11_window.c",
			"GLFW/src/.c",
			"GLFW/src/glx_context.c",
			"GLFW/src/linux_joystick.h",
			"GLFW/src/linux_joystick.c"
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
