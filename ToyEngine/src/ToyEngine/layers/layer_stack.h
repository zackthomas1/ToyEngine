#pragma once
#include "ToyEngine/layers/layer.h"

namespace ToyEngine
{

  // Consider a refactor to replace the 'layers_' array with std::vector<Layer*>
  class LayerStack
  {
  public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer *layer);
    void PopLayer();
    void PushOverlay(Layer *layer);
    void PopOverlay();

    std::vector<Layer*>::iterator begin() { return layers_.begin(); }
    std::vector<Layer*>::iterator end() { return layers_.end(); }
  private:
    std::vector<Layer*> layers_;
    unsigned int layerInsertionIndex_ = 0;
  };
}