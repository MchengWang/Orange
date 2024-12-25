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

#ifdef OG_ENABLE_ASSERTS
	#define OG_CLIENT_ASSERT(x, ...) { if ((!x)) {OG_CLIENT_ERROR("断言失败: {0}", __VA_ARGS__); __debugbreak(); } } 
	#define OG_CORE_ASSERT(x, ...) { if ((!x)) {OG_CORE_ERROR("断言失败: {0}", __VA_ARGS__); __debugbreak(); } } 
#else 
    #define OG_CLIENT_ASSERT(x, ...)  
    #define OG_CORE_ASSERT(x, ...) 
#endif // OG_ENABLE_ASSERTS

#define BIT(x) (1 << x)