#pragma once

#include "Orange/Core/Core.h"
#include "Orange/Core/Timestep.h"
#include "Orange/Events/Event.h"

namespace Orange {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return o_DebugName; }
	protected:
		std::string o_DebugName;
	};

}