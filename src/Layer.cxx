module;

import UI;

export module Layer;

export struct Layer
{
   explicit Layer() noexcept { }
   constexpr auto Play() const noexcept -> bool { return keepAlive_; }
   auto Update() noexcept -> void { CreateConfigWindow(); }

private:
   bool keepAlive_{ true };
};
