#pragma once

#include "Orange/Core.h"
#include "Layer.h"

#include <vector>

namespace Orange
{
	class ORANGE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLayer);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* overLayer);

		std::vector<Layer*>::iterator begin() { return o_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return o_Layers.end(); }

	private:
		std::vector<Layer*> o_Layers;
		std::vector<Layer*>::iterator o_LayerInsert;
	};
}