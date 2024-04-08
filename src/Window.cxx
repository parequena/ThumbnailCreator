module;

#include <cstdint>   // std::uint32_t
#include <exception> // std::runtime_error
#include <memory>    // std::unique_ptr

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp> // sf::RenderWindow
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

export module Window;

export struct Window
{
   explicit Window()
       : window_{ std::make_unique<sf::RenderWindow>(sf::VideoMode(windowWidth_, windowHeight_), windowName_) }
   {
      if (!ImGui::SFML::Init(*window_))
      {
         throw std::runtime_error("ImGui SFML could not be loaded.\n");
      }
   }

   auto Update() const noexcept -> void
   {
      sf::Clock deltaClock;
      while (window_->isOpen())
      {
         sf::Event event{};
         while (window_->pollEvent(event))
         {
            ImGui::SFML::ProcessEvent(*window_, event);

            if (event.type == sf::Event::Closed)
            {
               window_->close();
            }
         }

         ImGui::SFML::Update(*window_, deltaClock.restart());

         ImGui::ShowDemoWindow();

         ImGui::Begin("Hello, world!");
         ImGui::Button("Look at this pretty button");
         ImGui::End();

         window_->clear();
         ImGui::SFML::Render(*window_);
         window_->display();
      }
   }

   ~Window() noexcept { ImGui::SFML::Shutdown(); }

private:
   static constexpr std::uint16_t windowWidth_{ 1280 };
   static constexpr std::uint16_t windowHeight_{ 720 };
   static constexpr auto windowName_{ "ThumbnailCreator" };
   std::unique_ptr<sf::RenderWindow> window_{};
};
