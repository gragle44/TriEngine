workspace "TriEngine"
	architecture "x64"
	startproject "Sandbox" 

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags "MultiProcessorCompile"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["stb_image"] = "TriEngine/vendor/stb_image"
IncludeDir["spdlog"] = "TriEngine/vendor/spdlog/include"
IncludeDir["GLFW"] = "TriEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "TriEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "TriEngine/vendor/imgui"
IncludeDir["ImGuizmo"] = "TriEngine/vendor/ImGuizmo"
IncludeDir["yaml_cpp"] = "TriEngine/vendor/yaml_cpp/include"
IncludeDir["nativefiledialog"] = "TriEngine/vendor/nativefiledialog/src/include"
IncludeDir["glm"] = "TriEngine/vendor/glm"
IncludeDir["entt"] = "TriEngine/vendor/entt"
IncludeDir["nameof"] = "TriEngine/vendor/nameof"

group "Dependancies"
	include "TriEngine/vendor/GLFW"
	include "TriEngine/vendor/Glad"
	include "TriEngine/vendor/imgui"
	include "TriEngine/vendor/yaml_cpp"
	include "TriEngine/vendor/nativefiledialog"

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
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h"
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
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.nameof}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.nativefiledialog}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml_cpp",
		"nativefiledialog"
	}

	filter "files:TriEngine/vendor/ImGuizmo/**.cpp"
		flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"GLFW_INCLUDE_NONE"
		}

		links 
		{
			"dwmapi.lib"
		}

	filter "configurations:Debug"
		defines "TRI_DEBUG"
		optimize "debug"
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

		flags {
			"LinkTimeOptimization"
		}


project "TriEditor"
	location "TriEditor"
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
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.nameof}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.nativefiledialog}",
		"TriEngine/src"
	}

	links
	{
		"TriEngine"
	}

	defines "YAML_CPP_STATIC_DEFINE"

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

	filter "configurations:Dist"
		defines "TRI_DIST"
		optimize "speed"
		symbols "off"
		runtime "Release"
		flags "LinkTimeOptimization"
	

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
		"%{IncludeDir.entt}",
		"%{IncludeDir.nameof}",
		"%{IncludeDir.yaml_cpp}",
		"TriEngine/src"
	}

	links
	{
		"TriEngine"
	}

	defines "YAML_CPP_STATIC_DEFINE"

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

	filter "configurations:Dist"
		defines "TRI_DIST"
		optimize "speed"
		symbols "off"
		runtime "Release"
		flags "LinkTimeOptimization"
