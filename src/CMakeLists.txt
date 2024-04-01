# Window module:
# Handles:
#   - ImGui-SFML

find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_library(Window)
target_sources(Window
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Window.cxx
)
target_link_libraries(Window PRIVATE imgui::imgui ImGui-SFML::ImGui-SFML)


add_library(App)
target_sources(App
  PUBLIC
    FILE_SET CXX_MODULES FILES
    App.cxx
)
