#include "pch.h"
#include "layer_stack.h"

namespace ToyEngine
{
  LayerStack::LayerStack() {}
  LayerStack::~LayerStack()
  {
    TY_CORE_INFO("Destroy LayerStack");
    for (Layer *layer : layers_)
    {
      layer->OnDetatch();
      delete layer;
      layer = nullptr;
    }
  }

  void LayerStack::PushLayer(Layer* layer)
  {
    // Layers are inserted at layerInsertionIndex_ so overlays remain at the end.
    // Newer layers receive events before older ones. 
    // Layers in the foreground(newer) handle or consume events before they reach background layers.

    // Layers are inserted at the position of layerInsertionIndex_.
    // This ensures that overlays (added after all layers) remain at the end of the array.
    layer->OnAttach();
    layers_.insert(layers_.begin() + layerInsertionIndex_, layer);
    layerInsertionIndex_++;
  }

  void LayerStack::PopLayer()
  {
    if (layerInsertionIndex_ == 0)
      return; // No regular layers to pop

    // The layer to remove is at (layerInsertionIndex_ - 1)
    auto it = layers_.begin() + (layerInsertionIndex_ - 1);
    Layer* layer = *it;
    layer->OnDetatch();
    delete layer; // Prevent memory leak
    layers_.erase(it);
    layerInsertionIndex_--;
}

  void LayerStack::PushOverlay(Layer* layer)
  {
    // Overlays are pushed to the end of the layers_ array, after all regular layers.
    // They are always rendered and updated BEFORE all regular layers.
    layer->OnAttach();
    layers_.push_back(layer);
  }

  void LayerStack::PopOverlay()
  {
    Layer* layer = layers_.back();
    layer->OnDetatch();
    delete layer; // Prevent memory leak
    layers_.pop_back();
  }
}