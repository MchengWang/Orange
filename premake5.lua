workspace "Orange"
	architecture "x86_64"
	startproject "Orangefruit"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 包括相对于根文件夹（解决方案目录）的目录 
IncludeDir = {}
IncludeDir["GLFW"] = "Orange/vendor/GLFW/include"
IncludeDir["Glad"] = "Orange/vendor/Glad/include"
IncludeDir["ImGui"] = "Orange/vendor/imgui"
IncludeDir["glm"] = "Orange/vendor/glm"
IncludeDir["stb_image"] = "Orange/vendor/stb_image"
IncludeDir["entt"] = "Orange/vendor/entt/include"

group "Dependencies"
	include "Orange/vendor/GLFW"
	include "Orange/vendor/Glad"
	include "Orange/vendor/imgui"
group ""

project "Orange"
	location "Orange"
	kind "StaticLib"
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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

		--postbuildcommands
		--{
		--	("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outdir .. "/Sandbox/\"")
		--}

	filter "configurations:Debug"
		defines "OG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OG_DIST"
		runtime "Release"
		optimize "on"

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
		"Orange/src",
		"Orange/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Orange"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "OG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OG_DIST"
		runtime "Release"
		optimize "on"

project "Orangefruit"
	location "Orangefruit"
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
		"Orange/src",
		"Orange/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Orange"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "OG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OG_DIST"
		runtime "Release"
		optimize "on"