workspace "Paxel"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
project "Paxel"
	location "Paxel"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp"
	}

	includedirs
	{
		"./third/spdlog/include",
		"./third/glfw/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.14393.0"

		defines
		{
			"PX_PLATFORM_WINDOWS",
			"PX_BUILD_DLL",
			"_WINDLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
	filter "configurations:Debug"
		defines "PX_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PX_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "PX_DIST"
		optimize "On"
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp"
	}

	includedirs
	{
		"./third/spdlog/include",
		"./third/glfw/include",
		"./Paxel/Src"
	}

	links
	{
		"Paxel"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.14393.0"

		defines
		{
			"PX_PLATFORM_WINDOWS",
			"_MBCS"
		}
	filter "configurations:Debug"
		defines "PX_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PX_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "PX_DIST"
		optimize "On"