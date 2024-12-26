#pragma once

#include "Orange/Core.h"
#include "Orange/Events/Event.h"

namespace Orange
{
	class ORANGE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& evnet) {}

		inline const std::string& GetName() { return o_DebugName; }

	protected:
		std::string o_DebugName;
	};
}