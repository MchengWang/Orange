#include "ogpch.h"
#include "Orange/Core/Input.h"

#ifdef OG_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif // OG_PLATFORM_WINDOWS

namespace Orange
{

	Scope<Input> Input::o_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef OG_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		OG_CORE_ASSERT(false, "未知的平台类型！");
		return nullptr;
#endif // OG_PLATFORM_WINDOWS

	}

}