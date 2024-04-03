project "nativefiledialog"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    includedirs 
    {
        "src/include"
    }

	filter "system:windows"
        systemversion "latest"
        cppdialect "C++20"

        files
        {
            "src/nfd_win.cpp",
            "src/nfd_common.c",
            "src/**.h",
            "src/include/**.h",
        }

    filter "configurations:Debug"
		defines "TRI_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "TRI_RELEASE"
		optimize "on"
		runtime "Release"
