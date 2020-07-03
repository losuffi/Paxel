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

#define PX_ENSURE_RET_VOID(condi,...)\
if(!(condi))\
{\
	PX_CORE_ERROR("Assertion Failded: {0}",__VA_ARGS__);\
	return;\
}
#define PX_ENSURE_RET_VAL(condi,Ret,...)\
if (!(condi))\
{\
PX_CORE_ERROR("Assertion Failded: {0}", __VA_ARGS__); \
return Ret; \
}

#define PAXEL_LOW_LEVEL_RUNTIME_LOG_ENABLE 0