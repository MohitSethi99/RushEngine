workspace "RushEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "RushEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "RushEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "RushEngine/vendor/imgui"
IncludeDir["glm"] = "RushEngine/vendor/glm"
IncludeDir["stb_image"] = "RushEngine/vendor/stb_image"

group "Dependencies"
	include "RushEngine/vendor/GLFW"
	include "RushEngine/vendor/Glad"
	include "RushEngine/vendor/imgui"

group ""

project "RushEngine"
	location "RushEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rspch.h"
	pchsource "RushEngine/src/rspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src/",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RS_PLATFORM_WINDOWS",
			"RS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "RS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RS_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	links
	{
		"RushEngine"
	}

	includedirs
	{
		"RushEngine/vendor/spdlog/include",
		"RushEngine/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RS_DIST"
		runtime "Release"
		optimize "on"

