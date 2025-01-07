project "Orange"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-obj/" .. outdir .. "/%{prj.name}")

	pchheader "ogpch.h"
	pchsource "src/ogpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
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