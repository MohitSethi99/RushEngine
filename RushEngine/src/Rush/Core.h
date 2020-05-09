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

#define BIT(x) (1 << x)