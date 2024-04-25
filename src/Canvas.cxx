module;

import UI;

export module Canvas;

export struct Canvas
{
   explicit Canvas() noexcept { }
   constexpr auto Play() const noexcept -> bool { return keepAlive_; }
   auto Update() noexcept -> void { CreateCanvasWindow(); }

private:
   bool keepAlive_{ true };
};
