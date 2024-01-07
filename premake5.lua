workspace "TriEngine"
	architecture "x64"
	startproject "Sandbox" 

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "TriEngine/vendor/spdlog/include"
IncludeDir["GLFW"] = "TriEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "TriEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "TriEngine/vendor/imgui"
IncludeDir["glm"] = "TriEngine/vendor/glm"

group "Dependancies"
	include "TriEngine/vendor/GLFW"
	include "TriEngine/vendor/Glad"
	include "TriEngine/vendor/imgui"

group ""

project "TriEngine"
	location "TriEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	pchheader "tripch.h"
	pchsource "TriEngine/src/tripch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"TriEngine/src",
		"TriEngine/src/Core",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		defines 
		{
			"TRI_PLATFORM_WINDOWS",
			"TRI_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "TRI_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "TRI_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "TRI_DIST"
		optimize "On"
		runtime "Release"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"TriEngine/vendor/spdlog/include",
		"TriEngine/vendor/imgui",
		"%{IncludeDir.glm}",
		"TriEngine/src"
	}

	links
	{
		"TriEngine"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"TRI_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TRI_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "TRI_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "TRI_DIST"
		optimize "On"
		runtime "Release"
