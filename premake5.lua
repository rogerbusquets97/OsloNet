workspace "OsloNet"
	architecture "x64"
	startproject "Client"

	configurations
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


	--Oslo Net project--
	project "OsloNet"
	location "OsloNet"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"OsloNet/**.h",
		"OsloNet/**.cpp"
	}

	includedirs
	{
		"OsloNet/include"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

		undefines
		{
			"_UNICODE",
			"UNICODE"
		}

		links
		{
			"WS2_32",
			"Winmm"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"

	--Client project--
	project "Client"
	location "Client"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"Client/**.h",
		"Client/**.cpp",
		"MessageTypes.h"
	}

	includedirs
	{
		"OsloNet/include"
	}

	links 
	{
		"OsloNet"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

		undefines
		{
			"_UNICODE",
			"UNICODE"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"

	--Server project--
	project "Server"
	location "Server"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"Server/**.h",
		"Server/**.cpp",
		"MessageTypes.h"
	}

	links 
	{
		"OsloNet"
	}

	includedirs
	{
		"OsloNet/include"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

		undefines
		{
			"_UNICODE",
			"UNICODE"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"