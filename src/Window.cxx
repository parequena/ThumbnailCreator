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
         CreateMenuWindow();
         CreateLayersWindow();
         CreateConfigWindow();

         // ImGui::SetNextWindowPos(ImVec2{ 0.f, menuSize_.y });
         // ImGui::SetNextWindowSize(canvasSize_);
         // ImGui::Begin(windowName_, nullptr, windowFlags_);
         // ImGui::End();

         ImGui::ShowDemoWindow();

         window_->clear();
         ImGui::SFML::Render(*window_);
         window_->display();
      }
   }

   ~Window() noexcept { ImGui::SFML::Shutdown(); }

private:
   static constexpr ImGuiWindowFlags windowFlags_{ ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar
      | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav };

   static constexpr ImVec2 menuSize_{ 1280.F, 30.F };
   static constexpr ImVec2 canvasSize_{ 1280.F, 720.F };
   static constexpr ImVec2 layersSize_{ 200.F, 500.F };
   static constexpr ImVec2 configSize_{ 200.F, canvasSize_.y - layersSize_.y };

   static constexpr std::uint16_t windowWidth_{ static_cast<std::uint16_t>(canvasSize_.x)
      + static_cast<std::uint16_t>(layersSize_.x) };
   static constexpr std::uint16_t windowHeight_{ static_cast<std::uint16_t>(canvasSize_.y)
      + static_cast<std::uint16_t>(menuSize_.y) };
   static constexpr auto windowName_{ "ThumbnailCreator" };
   std::unique_ptr<sf::RenderWindow> window_{};

   auto CreateMenuWindow() const noexcept -> void
   {
      ImGui::SetNextWindowPos(ImVec2{ 0.f, 0.f });
      ImGui::SetNextWindowSize(menuSize_);

      if (!ImGui::BeginMainMenuBar())
      {
         return;
      }

      if (ImGui::BeginMenu("Menu"))
      {
         ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Examples"))
      {
         ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
   }

   auto CreateLayersWindow() const noexcept -> void
   {
      ImGui::SetNextWindowPos(ImVec2{ 1280.F, 0.f });

      ImGui::SetNextWindowSize(layersSize_);
      ImGui::Begin("Layers", nullptr, windowFlags_);
      ImGui::End();
   }

   auto CreateConfigWindow() const noexcept -> void
   {
      ImGui::SetNextWindowPos(ImVec2{ 1280.F, layersSize_.y });

      ImGui::SetNextWindowSize(layersSize_);
      ImGui::Begin("Config", nullptr, windowFlags_);
      ImGui::End();
   }
};
