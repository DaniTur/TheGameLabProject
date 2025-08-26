workspace "TheGameLabProject"
	architecture "x64"

	configurations 
	{ 
		"Debug", 
		"Release" 
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDirs = {}
IncludeDirs["GLFW"] = "TheGameLabProject/vendor/GLFW/include"
IncludeDirs["ImGui"] = "TheGameLabProject/vendor/imgui"
IncludeDirs["NlohmannJson"] = "TheGameLabProject/vendor/nlohmanJson/single_include"

-- Include premake5 files for dependencies before building the actual project
include "TheGameLabProject/vendor/premake5_glfw.lua"
include "TheGameLabProject/vendor/premake5_imgui.lua"

project "TheGameLabProject"
	location "TheGameLabProject"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"	-- glfw3 uses the dynamic version of the CRT (MSVCRT.dll)

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputdir .. "/%{prj.name}")

	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/GLAD/**.c",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/**.hpp",
		"%{prj.name}/vendor/glm/**.inl",
		-- ImGui Build Backends (dependencies of ImGui)
		"%{prj.name}/src/ImGui/ImGuiBuild.cpp"
	}

	defines { 
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS" 
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/src/Core",
		"%{prj.name}/vendor/spdlog/include", 
		"%{prj.name}/vendor/GLAD/include",
		"%{IncludeDirs.GLFW}",
		"%{prj.name}/vendor/stb_image",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/assimp/include",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.NlohmannJson}"
	}

	-- Compiled libraries
	libdirs { 
		"%{prj.name}/vendor/assimp/lib"
	}

	links {
		"GLFW",
		"ImGui"
	}

	-- Precompiled header
	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	filter "files:**.c"
		flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"
		defines { 
			"PLATFORM_WINDOWS",
			"GLFW_STATIC"
		}

		buildoptions "/utf-8"

	-- Filters for Debug and Release configurations of the dynamic libraries that manage different .dll files
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "On"
		links { 
			"assimp-vc143-mtd"
		}
		-- Requiers relative path from the project this block of commands is, in this case: project "TheGameLabProject"
		postbuildcommands { 
			"{COPYFILE} vendor/assimp/bin/assimp-vc143-mtd.dll %{cfg.targetdir}"
		}

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "On"
		links {
			"assimp-vc143-mt"
		}
		postbuildcommands {
			"{COPYFILE} vendor/assimp/bin/assimp-vc143-mt.dll %{cfg.targetdir}"
		}

	filter {"system:linux", "configurations:Debug"}
		runtime "Debug"
		symbols "On"
		systemversion "latest"
		defines {
			"PLATFORM_LINUX",
			"DEBUG",
			"GLFW_STATIC"
		}
		links { 
			"assimpd"
		}
		-- Requiers relative path from the project this block of commands is, in this case: project "TheGameLabProject"
		postbuildcommands { 
			"{COPYFILE} %{prj.location}/vendor/assimp/bin/libassimpd.so %{cfg.targetdir}"
		}
		
	filter {"system:linux", "configurations:Release"}
		runtime "Release"
		optimize "On"
		defines {
			"PLATFORM_LINUX",
			"RELEASE",
			"GLFW_STATIC"
		}
		links {
			"assimp"
		}
		postbuildcommands {
			"{COPYFILE} %{prj.location}/vendor/assimp/bin/libassimp.so %{cfg.targetdir}"
		}


		