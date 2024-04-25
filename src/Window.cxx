module;

import Canvas;
import LayerHandler;

#include <cstdint>       // std::uint32_t
#include <exception>     // std::runtime_error
#include <memory>        // std::unique_ptr
#include <unordered_map> // std::unordered_map

#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp> // sf::RenderWindow
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

export module Window;

export struct Window
{
   explicit Window(unsigned int const width, unsigned int const height)
       : window_{ std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), windowName_) }
   {
      if (!ImGui::SFML::Init(*window_))
      {
         throw std::runtime_error("ImGui SFML could not be loaded.\n");
      }
   }

   constexpr auto Play() const noexcept -> bool { return keepAlive_; }

   auto Update() noexcept -> void
   {
      sf::Event event{};
      while (window_->pollEvent(event))
      {
         ImGui::SFML::ProcessEvent(*window_, event);

         if (event.type == sf::Event::Closed)
         {
            keepAlive_ = false;
         }
      }

      ImGui::SFML::Update(*window_, deltaClock_.restart());

      canvas_.Update();
      layerHandler_.Update();
   }

   auto Render() const noexcept -> void
   {
      window_->clear();
      ImGui::SFML::Render(*window_);
      window_->display();
   }

   ~Window() noexcept
   {
      if (window_->isOpen())
      {
         window_->close();
      }
      ImGui::SFML::Shutdown();
   }

private:
   std::unique_ptr<sf::RenderWindow> window_{};
   sf::Clock deltaClock_{};
   static constexpr auto windowName_{ "ThumbnailCreator" };
   bool keepAlive_{ true };
   Canvas canvas_{};
   LayerHandler layerHandler_{};

   // Supported by IMGUI:
   // bmp, png, tga, jpg, gif, psd, hdr, pic and pnm
   // static constexpr std::unordered_map<std::string, std::string> Filters{};
};
