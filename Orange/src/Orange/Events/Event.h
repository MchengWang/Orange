#pragma once
#include "ogpch.h"

#include "Orange/Core.h"

namespace Orange
{
	// Orange �е��¼���ǰ��������״̬������ζ�ŵ��¼�����ʱ���������������ȣ���������������
	// ���ڽ��������õĲ��Կ��������¼��л����¼����ߣ����ڸ��½׶ε� ��event�� ���ִ������ǡ�

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication            = BIT(0),
		EventCategoryInput                  = BIT(1),
		EventCategoryKeyboard               = BIT(2),
		EventCategoryMouse                  = BIT(3),
		EventCategoryMouseButton            = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class ORANGE_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	private:
		bool o_Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			:o_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (o_Event.GetEventType() == T::GetStaticType())
			{
				o_Event.o_Handled = func(*(T*)&o_Event);
				return true;
			}

			return false;
		}

	private:
		Event& o_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}