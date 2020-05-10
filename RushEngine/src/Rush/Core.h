#pragma once

#ifdef RS_PLATFORM_WINDOWS
	#ifdef RS_BUILD_DLL
		#define RUSH_API __declspec(dllexport)
	#else
		#define RUSH_API __declspec(dllimport)
	#endif // RS_BUILD_DLL
#else
	#error Rush only supports Windows!
#endif // RS_PLATFORM_WINDOWS

#ifdef RS_ENABLE_ASSERTS
#define RS_ASSERT(x, ...) { if(!(x)) { RS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define RS_CORE_ASSERT(x, ...) { if(!(x)) { RS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define RS_ASSERT(x, ...)
#define RS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)