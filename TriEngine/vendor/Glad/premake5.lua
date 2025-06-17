project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c",
    }
    
    includedirs
    {
        "include"
    }

	filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        systemversion "latest"
        pic "on"

    filter "configurations:Debug"
		defines "TRI_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "TRI_RELEASE"
		optimize "on"
		runtime "Release"

    filter "configurations:Dist"
		defines "TRI_DIST"
		optimize "speed"
        symbols "off"
		runtime "Release"
