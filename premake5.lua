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
IncludeDir["box2D"] = "TriEngine/vendor/box2D/include"
IncludeDir["ImGuizmo"] = "TriEngine/vendor/ImGuizmo"
IncludeDir["yaml_cpp"] = "TriEngine/vendor/yaml_cpp/include"
IncludeDir["nativefiledialog"] = "TriEngine/vendor/nativefiledialog/src/include"
IncludeDir["glm"] = "TriEngine/vendor/glm"
IncludeDir["entt"] = "TriEngine/vendor/entt"
IncludeDir["magic_enum"] = "TriEngine/vendor/magic_enum"
IncludeDir["angelscript"] = "TriEngine/vendor/angelscript/angelscript/include"
IncludeDir["asbind20"] = "TriEngine/vendor/asbind20/include"
IncludeDir["tracy"] = "TriEngine/vendor/tracy/public"

group "Dependancies"
	include "TriEngine/vendor/GLFW"
	include "TriEngine/vendor/Glad"
	include "TriEngine/vendor/imgui"
	include "TriEngine/vendor/box2D"
	include "TriEngine/vendor/yaml_cpp"
	include "TriEngine/vendor/nativefiledialog"
	include "TriEngine/vendor/angelscript"

group ""

project "TriEngine"
	location "TriEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	architecture "x86_64"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	pchheader "tripch.h"
	pchsource "TriEngine/src/tripch.cpp"

	prebuildcommands 
	{
		"python %{wks.location}/Scripts/embed_shaders.py"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE",
		"GLFW_INCLUDE_NONE"
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
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/tracy/public/TracyClient.cpp",
		"%{prj.name}/vendor/angelscript/add_on/datetime/**.cpp",
		"%{prj.name}/vendor/angelscript/add_on/datetime/**.h",
		"%{prj.name}/vendor/angelscript/add_on/scriptarray/**.cpp",
		"%{prj.name}/vendor/angelscript/add_on/scriptarray/**.h",
		"%{prj.name}/vendor/angelscript/add_on/scriptbuilder/**.cpp",
		"%{prj.name}/vendor/angelscript/add_on/scriptbuilder/**.h",
		"%{prj.name}/vendor/angelscript/add_on/scriptdictionary/**.cpp",
		"%{prj.name}/vendor/angelscript/add_on/scriptdictionary/**.h",
		"%{prj.name}/vendor/angelscript/add_on/scriptfile/**.cpp",
		"%{prj.name}/vendor/angelscript/add_on/scriptfile/**.h",
		"%{prj.name}/vendor/angelscript/add_on/scriptmath/**.cpp",
		"%{prj.name}/vendor/angelscript/add_on/scriptmath/**.h",
		"%{prj.name}/vendor/angelscript/add_on/scripthelper/**.cpp",
		"%{prj.name}/vendor/angelscript/add_on/scripthelper/**.h",
		"%{prj.name}/vendor/angelscript/add_on/scriptstdstring/**.cpp",
		"%{prj.name}/vendor/angelscript/add_on/scriptstdstring/**.h"
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
		"%{IncludeDir.box2D}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.nativefiledialog}",
		"%{IncludeDir.magic_enum}",
		"%{IncludeDir.angelscript}",
		"%{IncludeDir.asbind20}",
		"%{IncludeDir.tracy}",
		"TriEngine/vendor/angelscript/add_on"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"box2D",
		"yaml_cpp",
		"nativefiledialog",
		"angelscript"
	}

	filter "files:TriEngine/vendor/ImGuizmo/**.cpp"
		flags {"NoPCH"}

	filter "files:TriEngine/vendor/angelscript/add_on/**.cpp"
		flags {"NoPCH"}

	filter "files:TriEngine/vendor/tracy/public/TracyClient.cpp"
		flags {"NoPCH"}

	filter "system:linux"
		systemversion "latest"

		buildoptions {"-Wall"}

		links { 
			"GLU",
			"GL",
			"X11",
			"pthread",
			"dl"
		}

	filter "system:windows"
		systemversion "latest"

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
		defines {"TRI_RELEASE"}
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "TRI_DIST"
		optimize "speed"
		symbols "off"
		runtime "Release"

		flags 
		{
			"linktimeoptimization"
		}


project "TriEditor"
	location "TriEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	architecture "x86_64"

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
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.box2D}",
		"%{IncludeDir.nativefiledialog}",
		"%{IncludeDir.magic_enum}",
		"%{IncludeDir.angelscript}",
		"%{IncludeDir.asbind20}",
		"%{IncludeDir.tracy}",
		"TriEngine/vendor/angelscript/add_on",
		"TriEngine/src"
	}

	links
	{
		"TriEngine"
	}

	defines "YAML_CPP_STATIC_DEFINE"

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"

		links 
		{
			"GLU",
			"GL",
			"X11",
			"pthread",
			"dl",
			"GLFW",
			"Glad",
			"ImGui",
			"box2D",
			"yaml_cpp",
			"nativefiledialog",
			"angelscript"
		}


	filter "configurations:Debug"
		defines "TRI_DEBUG"
		symbols "on"
		optimize "debug"
		runtime "Debug"

	filter "configurations:Release"
		defines {"TRI_RELEASE"}
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "TRI_DIST"
		optimize "speed"
		symbols "off"
		runtime "Release"
		flags "linktimeoptimization"
	
project "TriRuntime"
	location "TriRuntime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}

	includedirs
	{
		"TriEngine/vendor/spdlog/include",
		"TriEngine/vendor/imgui",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.box2D}",
		"%{IncludeDir.nativefiledialog}",
		"%{IncludeDir.magic_enum}",
		"%{IncludeDir.angelscript}",
		"%{IncludeDir.asbind20}",
		"TriEngine/vendor/angelscript/add_on",
		"TriEngine/src"
	}

	links
	{
		"TriEngine"
	}

	defines "YAML_CPP_STATIC_DEFINE"

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"

		links 
		{
			"GLU",
			"GL",
			"X11",
			"pthread",
			"dl",
			"GLFW",
			"Glad",
			"ImGui",
			"box2D",
			"yaml_cpp",
			"angelscript"
		}


	filter "configurations:Debug"
		defines "TRI_DEBUG"
		symbols "on"
		optimize "debug"
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
		flags "linktimeoptimization"
