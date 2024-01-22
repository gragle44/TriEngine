workspace "TriEngine"
	architecture "x64"
	startproject "Sandbox" 

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["stb_image"] = "TriEngine/vendor/stb_image"
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
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	pchheader "tripch.h"
	pchsource "TriEngine/src/tripch.cpp"

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"TriEngine/src",
		"TriEngine/src/Core",
		"%{IncludeDir.stb_image}",
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
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"TRI_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

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
		optimize "on"
		runtime "Release"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
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
		systemversion "latest"

		defines
		{
			"TRI_PLATFORM_WINDOWS"
		}

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
		optimize "on"
		runtime "Release"
