project "Orange-scriptCode"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	
	targetdir ("%{wks.location}/Orangefruit/Resources/Scripts")
	objdir ("%{wks.location}/Orangefruit/Resources/Scripts/Intermediates")

	files
	{
		"Source/**.cs",
		"Properties/**.cs"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Default"
