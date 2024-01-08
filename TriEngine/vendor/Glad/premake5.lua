project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/Glad/Glad.h",
        "include/KHR/khrplatform.h",
        "src/Glad.c",
    }
    
    includedirs
    {
        "include"
    }

	filter "system:windows"
        systemversion "latest"

    	filter "configurations:Debug"
		defines "TRI_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "TRI_RELEASE"
		optimize "on"
		runtime "Release"
