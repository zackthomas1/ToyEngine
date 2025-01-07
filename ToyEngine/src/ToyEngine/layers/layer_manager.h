#pragma once
#include "ToyEngine/layers/layer.h"

namespace ToyEngine
{
	class LayerManager
	{
	public:
		static void AddLayer(Layer* layer);
		static void RemoveLayer(Layer* layer);
		static void DeleteLayers();
		static void UpdateLayers(float time_delta);
		static void OnEvent(Event& e);
	private:
		static const int MAX_LAYERS = 64; 
		static Layer* layers_[MAX_LAYERS];
		static unsigned int layer_count_;
	};
}