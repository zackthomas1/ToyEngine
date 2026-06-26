#pragma once
#include "ToyEngine/layers/layer.h"

namespace ToyEngine
{
  /// @brief The LayerStack class manages a stack of layers and overlays in the application.
  class LayerStack
  {
  public:

    /// @brief Constructs a LayerStack instance.
    LayerStack();

    /// @brief Destructor for the LayerStack class. Cleans up all layers and overlays in the stack.
    ~LayerStack();

    /// @brief Adds a new layer to the stack. Layers are inserted below overlays and above previously added layers.
    /// @param layer: The layer to add.
    void PushLayer(Layer *layer);

    /// @brief Removes the layer from the top of the stack.
    void PopLayer();
    
    /// @brief Adds a new overlay to the stack. Overlays are always on top of layers.
    /// @param overlay: The overlay to add.
    void PushOverlay(Layer *overlay);
    
    /// @brief Removes the overlay from the top of the stack.
    void PopOverlay();

    /// @brief Returns an iterator to the beginning of the layer stack.
    std::vector<Layer*>::iterator begin() { return layers_.begin(); }

    /// @brief Returns an iterator to the end of the layer stack.
    std::vector<Layer*>::iterator end() { return layers_.end(); }
  private:
    /* The vector data structure that holds all layers and overlays. */
    std::vector<Layer*> layers_;

    /* The index at which new layers are inserted. */
    unsigned int layerInsertionIndex_ = 0;
  };
}