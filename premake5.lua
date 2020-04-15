workspace "OpenGL"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

-- this uses tokens (see premake wiki for a list of them)
--  cfg.buildcfg (Debug/Release/Dist)
--  cfg.system (Windows/Mac/Linux)
--  cfg.architecture (x64)
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "OpenGL"
	location "OpenGL"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "opengl.h"
	pchsource "OpenGL/src/opengl.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
	    "%{prj.name}/src"
	}

	-- FYI: Windows SDK version from properties = 10.0.15063.0
	filter "system:windows"
		-- cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		-- ommitted: HZ_BUILD_DLL
		defines {
			"_SCL_SECURE_NO_WARNINGS",
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "OPENGL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "OPENGL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "OPENGL_DIST"
		optimize "On"