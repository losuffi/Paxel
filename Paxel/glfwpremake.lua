project "GLFW"
    kind "StaticLib"
    language "C"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "third/glfw/include/GLFW/glfw3.h",
        "third/glfw/include/GLFW/glfw3native.h",
        "third/glfw/src/glfw_config.h",
        "third/glfw/src/context.c",
        "third/glfw/src/init.c",
        "third/glfw/src/input.c",
        "third/glfw/src/monitor.c",
        "third/glfw/src/vulkan.c",
        "third/glfw/src/window.c"
    }
    
	filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "10.0.18362.0"
        staticruntime "On"
        
        files
        {
            "third/glfw/src/win32_init.c",
            "third/glfw/src/win32_joystick.c",
            "third/glfw/src/win32_monitor.c",
            "third/glfw/src/win32_time.c",
            "third/glfw/src/win32_thread.c",
            "third/glfw/src/win32_window.c",
            "third/glfw/src/wgl_context.c",
            "third/glfw/src/egl_context.c",
            "third/glfw/src/osmesa_context.c"
        }

		defines 
		{ 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
		}
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"