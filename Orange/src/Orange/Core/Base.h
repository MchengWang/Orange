#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	// Windows x64 / x86
	#ifdef _WIN64
		// Windows x64
		#define OG_PLATFORM_WINDOWS
	#else
		// Windows x86
		#error "暂不支持构建 x86！"
	#endif // _WIN64

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	* so we must check all of them (in this order)
	* to ensure that we're running on MAC
	* and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "暂不支持 ios 模拟器!"
	#elif TRAGET_OS_IPHONE == 1
		#define OG_PLATFORM_IOS
		#error "暂不支持 ios 系统！"
	#elif TRAGET_OS_MAX == 1
		#define OG_PLATFORM_MACOS
		#error "暂不支持 MacOS 系统！"
	#else
		#error "位置的苹果平台！"
	#endif
#endif // _WIN32

//#ifdef OG_PLATFORM_WINDOWS
//	#if OG_DYNAMIC_LINK
//		#ifdef OG_BUILD_DLL
//			#define ORANGE_API __declspec(dllexport)
//		#else
//			#define ORANGE_API __declspec(dllimport)
//		#endif // OG_BUILD_DLL
//	#else
//		#define ORANGE_API
//	#endif // OG_DYNAMIC_LINK
//#else 
//	#error orange仅支持Windows系统！
//#endif // OG_PLATFORM_WINDOWS

#ifdef OG_DEBUG
	#if defined(OG_PLATFORM_WINDOWS)
		#define OG_DEBUGBREAK() __debugbreak()
	#elif defined(OG_PLATFORM_LINUX)
		#include <signal.h>
		#define OG_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "当前平台暂不支持调试！"
	#endif // defined(OG_PLATFORM_WINDOWS)
	#define OG_DEBUGBREAK()
#else
	#define OG_ENABLE_ASSERTS
#endif // OG_DEBUG


#ifdef OG_ENABLE_ASSERTS
	#define OG_CLIENT_ASSERT(x, ...) { if (!(x)) { OG_CLIENT_ERROR("断言失败: {0}", __VA_ARGS__); OG_DEBUGBREAK(); } } 
	#define OG_CORE_ASSERT(x, ...) { if (!(x)) { OG_CORE_ERROR("断言失败: {0}", __VA_ARGS__); OG_DEBUGBREAK(); } } 
#else 
    #define OG_CLIENT_ASSERT(x, ...)  
    #define OG_CORE_ASSERT(x, ...) 
#endif // OG_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define OG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Orange
{
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}