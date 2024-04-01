module;

#include <memory>    // std::unique_ptr
#include <exception> // std::runtime_error
#include <cstdint>   // std::uint32_t

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp> // sf::RenderWindow
#include <SFML/Graphics/CircleShape.hpp>  // sf::CircleShape
#include <SFML/Window/Event.hpp>          // sf::Event
#include <SFML/System/Clock.hpp>          // sf::Clock

export module Window;

export struct Window
{
   explicit Window()
       : window_{ std::make_unique<sf::RenderWindow>(sf::VideoMode(windowWidth, windowHeight), "ImGui") }
   {
      if (!ImGui::SFML::Init(*window_))
      {
         throw std::runtime_error("SFML Could not load ImGui :(\n");
      }

      window_->setFramerateLimit(windowFrameRate);
   }

   void Update()
   {
      sf::CircleShape shape(100.f);
      shape.setFillColor(sf::Color::Green);

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
         window_->draw(shape);
         ImGui::SFML::Render(*window_);
         window_->display();
      }
   }

   ~Window() noexcept { ImGui::SFML::Shutdown(); }

private:
   static constexpr std::uint32_t windowWidth{ 1280 };
   static constexpr std::uint32_t windowHeight{ 720 };
   static constexpr std::uint32_t windowFrameRate{ 60 };

   std::unique_ptr<sf::RenderWindow> window_{};
   sf::Clock deltaClock_{};
};
