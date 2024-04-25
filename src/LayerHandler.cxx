module;

import Layer;
import UI;

#include <vector>

export module LayerHander;

export struct LayerHander
{
   explicit LayerHander() noexcept { }
   constexpr auto Play() const noexcept -> bool { return keepAlive_; }
   auto Update() noexcept -> void { CreateLayersWindow(); }

private:
   bool keepAlive_{ true };
   std::vector<Layer> layers_{};
};
