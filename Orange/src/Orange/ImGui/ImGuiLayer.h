#pragma once

#include "Orange/Layer.h"

#include "Orange/Events/ApplicationEvent.h"
#include "Orange/Events/KeyEvent.h"
#include "Orange/Events/MouseEvent.h"

namespace Orange {

	class ORANGE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}