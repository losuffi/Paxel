#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
class PAXEL_API Log
{
public:
	static void Init();
	static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

#define PX_CORE_TRACE(...)	Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PX_CORE_INFO(...)	Log::GetCoreLogger()->info(__VA_ARGS__)
#define PX_CORE_WARN(...)	Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PX_CORE_ERROR(...)	Log::GetCoreLogger()->error(__VA_ARGS__)
#define PX_CORE_FATAL(...)	Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define PX_TRACE(...)	Log::GetClientLogger()->trace(__VA_ARGS__)
#define PX_INFO(...)	Log::GetClientLogger()->info(__VA_ARGS__)
#define PX_WARN(...)	Log::GetClientLogger()->warn(__VA_ARGS__)
#define PX_ERROR(...)	Log::GetClientLogger()->error(__VA_ARGS__)
#define PX_FATAL(...)	Log::GetClientLogger()->fatal(__VA_ARGS__)