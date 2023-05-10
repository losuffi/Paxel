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
IncludeDir = {}
IncludeDir["GLFW"] = "third/glfw/include"
IncludeDir["Vulkan"] = "third/vulkan/Include"
IncludeDir["ImGui"] = "third/imgui/"
includedirs{"third/glfw"}
project "Paxel"
	location "Paxel"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")

	pchheader "PXPCH.h"
	pchsource "%{prj.name}/Src/PXPCH.cpp"
	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp"
	}

	includedirs
	{
		"third/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Vulkan}",
		"%{prj.name}/Src",
		"%{IncludeDir.ImGui}",
	}
	libdirs
	{
		"third/vulkan/Lib",
	}

	links
	{
		"GLFW",
		"opengl32.lib",
		"vulkan-1.lib",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.18362.0"

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
		"third/spdlog/include",
		"Paxel/Src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Vulkan}",
	}

	links
	{
		"Paxel"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.18362.0"

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
-- project "GLFW"
    -- kind "StaticLib"
    -- language "C"
    
	-- targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    -- objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- files
	-- {
        -- "third/glfw/include/GLFW/glfw3.h",
        -- "third/glfw/include/GLFW/glfw3native.h",
        -- "third/glfw/src/glfw_config.h",
        -- "third/glfw/src/context.c",
        -- "third/glfw/src/init.c",
        -- "third/glfw/src/input.c",
        -- "third/glfw/src/monitor.c",
        -- "third/glfw/src/vulkan.c",
        -- "third/glfw/src/window.c"
    -- }
    
	-- filter "system:windows"
        -- buildoptions { "-std=c11", "-lgdi32" }
        -- systemversion "10.0.18362.0"
        -- staticruntime "On"
        
        -- files
        -- {
            -- "third/glfw/src/win32_init.c",
            -- "third/glfw/src/win32_joystick.c",
            -- "third/glfw/src/win32_monitor.c",
            -- "third/glfw/src/win32_time.c",
            -- "third/glfw/src/win32_thread.c",
            -- "third/glfw/src/win32_window.c",
            -- "third/glfw/src/wgl_context.c",
            -- "third/glfw/src/egl_context.c",
            -- "third/glfw/src/osmesa_context.c"
        -- }

		-- defines 
		-- { 
            -- "_GLFW_WIN32",
            -- "_CRT_SECURE_NO_WARNINGS"
		-- }
    -- filter { "system:windows", "configurations:Release" }
        -- buildoptions "/MT"