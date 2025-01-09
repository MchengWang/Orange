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

-- 引用路径
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Orange/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Orange/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Orange/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Orange/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Orange/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Orange/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Orange/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Orange/vendor/ImGuizmo"

group "Dependencies"
	include "vendor/premake"
	include "Orange/vendor/GLFW"
	include "Orange/vendor/Glad"
	include "Orange/vendor/imgui"
	include "Orange/vendor/yaml-cpp"
group ""

include "Orange"
include "Orangefruit"
include "Sandbox"
