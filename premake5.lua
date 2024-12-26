workspace "Orange"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 包括相对于根文件夹（解决方案目录）的目录
IncludeDir = {}
IncludeDir["GLFW"] = "Orange/vendor/GLFW/include"

include "Orange/vendor/GLFW"

project "Orange"
	location "Orange"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outdir .. "/%{prj.name}")

	pchheader "ogpch.h"
	pchsource "Orange/src/ogpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"OG_PLATFORM_WINDOWS",
			"OG_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "OG_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "OG_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "OG_DIST"
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Orange/vendor/spdlog/include",
		"Orange/src"
	}

	links
	{
		"Orange"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"OG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "OG_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "OG_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "OG_DIST"
		buildoptions "/MD"
		optimize "On"