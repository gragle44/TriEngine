project "nativefiledialog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    includedirs 
    {
        "src/include"
    }

    files
    {
        "src/nfd_common.c",
        "src/**.h",
        "src/include/**.h",
    }

	filter "system:windows"
        files
        {
            "src/nfd_win.cpp"
        }
        systemversion "latest"

    filter "system:linux"
        files
        {
            "src/nfd_zenity.c"
        }
        systemversion "latest"

    filter "configurations:Debug"
		defines "TRI_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "TRI_RELEASE"
		optimize "on"
		runtime "Release"
