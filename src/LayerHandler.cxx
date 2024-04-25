module;

import Layer;
import UI;

#include <vector>

export module LayerHandler;

export struct LayerHandler
{
   explicit LayerHandler() noexcept
   {
      layers_.emplace_back(Layer{}); // Generic Layer to add UI
   }
   constexpr auto Play() const noexcept -> bool { return keepAlive_; }
   auto Update() noexcept -> void
   {
      CreateLayersWindow();
      for (auto& layer : layers_)
      {
         layer.Update();
      }
   }

private:
   bool keepAlive_{ true };
   std::vector<Layer> layers_{};
};
