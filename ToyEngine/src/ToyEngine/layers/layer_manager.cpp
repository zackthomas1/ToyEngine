#include "pch.h"
#include "layer_manager.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before `#include "spdlog/spdlog.h"

#include "spdlog/spdlog.h"

namespace ToyEngine
{
	unsigned int LayerManager::layer_count_ = 0;
	Layer* LayerManager::layers_[] = {};

	void LayerManager::PushLayer(Layer* layer)
	{
		// Check that by adding a layer the maximum allow number of layers is not execeeded.
		TY_CORE_ASSERT(layer_count_ < MAX_LAYERS, "Exceeded maximum allowed number of layers");
		
		// Layers are pushed to the back of the layers_ array.
		// This means the currently added layer processes events AFTER previously added layers. 
		// If a previously added layer has already handled the event the current layer will NOT process it
		// even if the layer has a statement for handling that specific event. 
		// The order of layers in the array matters and effects how events are handled.
		layers_[layer_count_] = layer;
		layer_count_++;
		layer->OnAttach();
	}

	void LayerManager::RemoveLayer(Layer* layer)
	{
		TY_CORE_WARN("TODO: Implement RemoveLayer.");
		layer_count_--;
	}

	void LayerManager::DeleteLayers()
	{
		// Deallocates memory for each layer in the layers_ array
		TY_CORE_TRACE("Deleted Layers");
		for (int i = 0; i < layer_count_; i++)
		{
			layers_[i]->OnDetatch();
			delete layers_[i];
			layers_[i] = nullptr;
			layer_count_--;
		}
	}
	
	void LayerManager::UpdateLayers(float time_delta)
	{
		// Iterate through all the layers and call their Update() method.
		for (int i = 0; i < layer_count_; i++)
		{
			layers_[i]->Update(time_delta);
		}
	}
	
	void LayerManager::OnEvent(Event& e)
	{
		for (int i = 0; i < layer_count_; i++)
		{
			// If the event has already been handled by a previous layer return from method.
			// There is no need to continue iterating through the layers_ array looking for a 
			// layer to handle the event.
			if (e.GetEventHandled() == true) { return; }	
			
			// Execute layer OnEvent method to which will attempt to handle the event passed into it.
			// If current layer handles the event's 'isHandled_' varaible is set to true.
			layers_[i]->OnEvent(e);
		}
	}

	void LayerManager::RenderGui()
	{
		for (int i = 0; i < layer_count_; i++)
		{
			layers_[i]->OnImGuiRender();
		}
	}
}

