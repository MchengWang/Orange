#pragma once

#include "Orange/Core.h"
#include "Layer.h"

#include <vector>

namespace Orange {

	class ORANGE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return o_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return o_Layers.end(); }
	private:
		std::vector<Layer*> o_Layers;
		unsigned int o_LayerInsertIndex = 0;
	};

}