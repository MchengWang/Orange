include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Orange"
	architecture "x86_64"
	startproject "Orangefruit"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ��������ڸ��ļ��У��������Ŀ¼����Ŀ¼ 
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Orange/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Orange/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Orange/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Orange/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Orange/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Orange/vendor/entt/include"

group "Dependencies"
	include "vendor/premake"
	include "Orange/vendor/GLFW"
	include "Orange/vendor/Glad"
	include "Orange/vendor/imgui"
group ""

include "Orange"
include "Orangefruit"
include "Sandbox"