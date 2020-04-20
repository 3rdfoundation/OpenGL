workspace "OpenGL"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

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
	    "%{prj.name}/src",
	    "%{prj.name}/dependencies/GLFW/include",
	    "%{prj.name}/dependencies/GLEW/include",
	    "%{prj.name}/src/vendor"
	}

    libdirs {
	    "%{prj.name}/dependencies/GLFW/lib-vc2017",
	    "%{prj.name}/dependencies/GLEW/lib/Release/x64"
    }

    links {
        "glfw3.lib",
        "opengl32.lib",
        "user32.lib", 
        "gdi32.lib", 
        "shell32.lib", 
        "vcruntime.lib", 
        "msvcrt.lib",
        "glew32s.lib"
    }

	-- FYI: Windows SDK version from properties = 10.0.15063.0
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		-- ommitted: HZ_BUILD_DLL
		defines {
			"GLEW_STATIC"
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