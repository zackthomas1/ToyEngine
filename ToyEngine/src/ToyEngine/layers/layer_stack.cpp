#include "pch.h"
#include "layer_stack.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before `#include "spdlog/spdlog.h"

#include "spdlog/spdlog.h"

namespace ToyEngine
{
	LayerStack::LayerStack() {}
	LayerStack::~LayerStack()
	{
		TY_CORE_TRACE("Destory LayerStack");
		for (Layer *layer : layers_)
		{
			delete layer;
			layer = nullptr;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		// Layers are pushed to the back of the layers_ array.
		// This means the currently added layer processes events AFTER previously added layers. 
		// If a previously added layer has already handled the event the current layer will NOT process it
		// even if the layer has a statement for handling that specific event. 
		// The order of layers in the array matters and effects how events are handled.
		layer->OnAttach();
		layers_.push_back(layer);
		layerInsertionIndex_++;
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		layers_[layerInsertionIndex_--]->OnDetatch();
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		TY_CORE_WARN("TODO: Implement PushOverlay.");
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		TY_CORE_WARN("TODO: Implement PopOverlay.");
	}
}