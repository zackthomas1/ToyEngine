#pragma once
#include "ToyEngine/layers/layer.h"

namespace ToyEngine
{

	// Consider a refactor to replace the 'layers_' array with a linked-list. 
	// Since layers_ is an array of pointers it a linked-list would be a more memory efficient method to store a list of layers. 
	// With an array memory is allocated to store a list of pointers for each layer. 
	// With a linked-list LayerManager would only need to store a single pointer to head of the list. 
	// The layers_ array is used for Update() and OnEvent() methods which walk the list and call the Update() and OnEvent() methods for each layer. 
	// The LayerManager does not utilize indexing or constant time random access offered by arrays. 

	class LayerManager
	{
	public:
		static void PushLayer(Layer* layer);
		static void RemoveLayer(Layer* layer);
		static void DeleteLayers();
		static void UpdateLayers(float time_delta);
		static void OnEvent(Event& e);
		static void RenderGui();
	private:
		static const int MAX_LAYERS = 64;
		static Layer* layers_[MAX_LAYERS];
		static unsigned int layer_count_;
	};
}