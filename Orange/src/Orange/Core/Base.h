#pragma once

#include <memory>

#include "Orange/Core/PlatformDetection.h"

#ifdef OG_DEBUG
	#if defined(OG_PLATFORM_WINDOWS)
		#define OG_DEBUGBREAK() __debugbreak()
	#elif defined(OG_PLATFORM_LINUX)
		#include <signal.h>
		#define OG_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "当前平台暂不支持调试！"
	#endif // defined(OG_PLATFORM_WINDOWS)
	#define OG_ENABLE_ASSERTS
#else
	#define OG_DEBUGBREAK()
#endif // OG_DEBUG


#ifdef OG_ENABLE_ASSERTS
	#define OG_CLIENT_ASSERT(x, ...) { if (!(x)) { OG_CLIENT_ERROR("断言失败: {0}", __VA_ARGS__); OG_DEBUGBREAK(); } } 
	#define OG_CORE_ASSERT(x, ...) { if (!(x)) { OG_CORE_ERROR("断言失败: {0}", __VA_ARGS__); OG_DEBUGBREAK(); } } 
#else 
    #define OG_CLIENT_ASSERT(x, ...)  
    #define OG_CORE_ASSERT(x, ...) 
#endif // OG_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define OG_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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