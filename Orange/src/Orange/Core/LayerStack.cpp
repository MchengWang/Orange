#include "ogpch.h"
#include "LayerStack.h"

namespace Orange {

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : o_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		o_Layers.emplace(o_Layers.begin() + o_LayerInsertIndex, layer);
		o_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		o_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(o_Layers.begin(), o_Layers.end(), layer);
		if (it != o_Layers.end())
		{
			o_Layers.erase(it);
			o_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(o_Layers.begin(), o_Layers.end(), overlay);
		if (it != o_Layers.end())
			o_Layers.erase(it);
	}

}