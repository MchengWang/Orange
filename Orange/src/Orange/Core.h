#pragma once

#ifdef OG_PLATFORM_WINDOWS
	#ifdef OG_BUILD_DLL
		#define ORANGE_API __declspec(dllexport)
	#else
		#define ORANGE_API __declspec(dllimport)
	#endif // OG_BUILD_DLL
#else 
	#error Orange ��֧�� windows ϵͳ��
#endif // OG_PLATFORM_WINDOWS