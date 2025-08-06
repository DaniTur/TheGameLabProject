-- This script is used to build ImGui library and set it up into TheGameLabProject
project "ImGui"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputdir .. "/%{prj.name}")

	-- files to compile
	files {
		"imgui/*.h",
		"imgui/*.cpp"
		-- to improve debugging experience with Visual Studio
		--"imgui/misc/debuggers/imgui.natvis",
		--"imgui/misc/debuggers/imgui.natstepfilter",
	}

	filter "system:windows"
		systemversion "latest"	-- to use the latest version of the SDK available
		cppdialect "C++20"
		staticruntime "on"	-- for Visual Studio sets <RuntimeLibrary> to "MultiThreaded", "off" for MultiThreadedDLL

	filter "system:linux"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "on"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"