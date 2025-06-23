project "angelscript"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "angelscript/source/**"
	}

	filter "system:windows"
		systemversion "latest"

    filter "system:linux"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
        
    filter "configurations:Dist"
		defines "TRI_DIST"
		optimize "speed"
		symbols "off"
		runtime "Release"

		flags 
		{
			"linktimeoptimization"
		}
