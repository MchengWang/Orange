#pragma once

#include "Orange/Layer.h"

namespace Orange
{
	class ORANGE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		float o_Time = 0.0f;
	};
}