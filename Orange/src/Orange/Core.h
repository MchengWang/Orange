#pragma once

#ifdef OG_PLATFORM_WINDOWS
	#ifdef OG_BUILD_DLL
		#define ORANGE_API __declspec(dllexport)
	#else
		#define ORANGE_API __declspec(dllimport)
	#endif // OG_BUILD_DLL
#else 
	#error Orange 仅支持 windows 系统！
#endif // OG_PLATFORM_WINDOWS


#define BIT(x) (1 << x)