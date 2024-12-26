#include "ogpch.h"
#include "LayerStack.h"

namespace Orange
{
	LayerStack::LayerStack()
	{
		o_LayerInsert = o_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : o_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		o_LayerInsert = o_Layers.emplace(o_LayerInsert, layer);
	}

	void LayerStack::PushOverLayer(Layer* overLayer)
	{
		o_Layers.emplace_back(overLayer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(o_Layers.begin(), o_Layers.end(), layer);

		if (it != o_Layers.end())
		{
			o_Layers.erase(it);
			o_LayerInsert--;
		}
	}

	void LayerStack::PopOverLayer(Layer* overLayer)
	{
		auto it = std::find(o_Layers.begin(), o_Layers.end(), overLayer);
		
		if (it != o_Layers.end())
			o_Layers.erase(it);
	}
}