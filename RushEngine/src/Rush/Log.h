#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Rush 
{
	class RUSH_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define RS_CORE_ERROR(...)    ::Rush::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RS_CORE_WARN(...)     ::Rush::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RS_CORE_INFO(...)     ::Rush::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RS_CORE_TRACE(...)    ::Rush::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RS_CORE_FATAL(...)    ::Rush::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log errors
#define RS_ERROR(...)         ::Rush::Log::GetClientLogger()->error(__VA_ARGS__)
#define RS_WARN(...)          ::Rush::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RS_INFO(...)          ::Rush::Log::GetClientLogger()->info(__VA_ARGS__)
#define RS_TRACE(...)	      ::Rush::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RS_FATAL(...)         ::Rush::Log::GetClientLogger()->fatal(__VA_ARGS__)