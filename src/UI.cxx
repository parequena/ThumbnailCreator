module;

#include <imgui.h>

export module UI;

static constexpr float CanvasWidth{ 1280.F };
static constexpr float CanvasHeight{ 720.F };

static constexpr float AditionalWidth{ 200.F }; // Layers + Configs.
static constexpr float LayersHeight{ 500.F };
static constexpr float ConfigHeight{ CanvasHeight - LayersHeight };

using uint = unsigned int;
export auto GetRenderWindowWidth() noexcept -> uint;
export auto GetRenderWindowHeight() noexcept -> uint;

// Canvas Window data.
static constexpr ImVec2 CanvasWindowInit{ 0.F, 0.F };
static constexpr ImVec2 CanvasWindowSize{ CanvasWidth, CanvasHeight };

// Layers Window data.
static constexpr ImVec2 LayersInit{ CanvasWidth, 0.F };
static constexpr ImVec2 LayersSize{ AditionalWidth, LayersHeight };

// Config Window data.
static constexpr ImVec2 ConfigInit{ CanvasWidth, LayersHeight };
static constexpr ImVec2 ConfigSize{ AditionalWidth, ConfigHeight };

static constexpr ImGuiWindowFlags LayersWindowFlags{ ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
   | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav };

static constexpr ImGuiWindowFlags CommonWindowFlags{ LayersWindowFlags | ImGuiWindowFlags_NoScrollbar };

/*
________________________
|  MenuWindow | Layers |
-----------------------|
|  Canvas     | Config |
‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
*/

export auto CreateCanvasWindow() noexcept -> void;
export auto CreateLayersWindow() noexcept -> void;
export auto CreateConfigWindow() noexcept -> void;
export auto ShowImGuiDemo() noexcept -> void;

module :private;

auto GetRenderWindowWidth() noexcept -> uint { return uint(CanvasWidth) + uint(AditionalWidth); }
auto GetRenderWindowHeight() noexcept -> uint { return uint(CanvasHeight); }

void CreateCanvasWindow() noexcept
{
   ImGui::SetNextWindowPos(CanvasWindowInit);
   ImGui::SetNextWindowSize(CanvasWindowSize);

   ImGui::Begin("Canvas", nullptr, CommonWindowFlags);
   ImGui::End();
}

auto CreateLayersWindow() noexcept -> void
{
   ImGui::SetNextWindowPos(LayersInit);
   ImGui::SetNextWindowSize(LayersSize);

   ImGui::Begin("Layers", nullptr, LayersWindowFlags);
   ImGui::End();
}

auto CreateConfigWindow() noexcept -> void
{
   ImGui::SetNextWindowPos(ConfigInit);
   ImGui::SetNextWindowSize(ConfigSize);

   ImGui::Begin("Config", nullptr, CommonWindowFlags);
   ImGui::End();
}

auto ShowImGuiDemo() noexcept -> void { ImGui::ShowDemoWindow(); }
