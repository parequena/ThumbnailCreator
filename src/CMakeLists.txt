# Window module:
# Handles:
#   - imgui

find_package(imgui CONFIG REQUIRED)
add_library(UI)
target_sources(UI
  PUBLIC
    FILE_SET CXX_MODULES FILES
    UI.cxx
)
target_compile_options(UI PRIVATE ${PROJECT_WARNINGS})
target_link_libraries(UI PRIVATE imgui::imgui)

# Window module:
# Handles:
#   - ImGui-SFML
find_package(ImGui-SFML CONFIG REQUIRED)
add_library(Window)
target_sources(Window
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Window.cxx
)
target_compile_options(Window PRIVATE ${PROJECT_WARNINGS})
target_link_libraries(Window PRIVATE UI ImGui-SFML::ImGui-SFML)
