# Window module:
# Handles:
#   - imgui

find_package(ImGui-SFML CONFIG REQUIRED)

add_library(Window)
target_sources(Window
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Window.cxx
)
target_compile_options(Window PRIVATE ${PROJECT_WARNINGS})
target_link_libraries(Window PRIVATE ImGui-SFML::ImGui-SFML)
