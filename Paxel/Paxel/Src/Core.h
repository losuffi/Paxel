#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#ifdef PX_BUILD_DLL
		#define PAXEL_API __declspec(dllexport)
	#else
		#define PAXEL_API __declspec(dllimport)
	#endif
#else
	#error Only support windows now!
#endif
