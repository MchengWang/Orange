include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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
