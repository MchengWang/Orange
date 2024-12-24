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

project "Orange"
	location "Orange"
	kind "SharedLib"
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
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
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
		symbols "On"

	filter "configurations:Release"
		defines "OG_DEBUG"
		optimize "On"

	filter "configurations:Dist"
		defines "OG_DEBUG"
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
		symbols "On"

	filter "configurations:Release"
		defines "OG_DEBUG"
		optimize "On"

	filter "configurations:Dist"
		defines "OG_DEBUG"
		optimize "On"