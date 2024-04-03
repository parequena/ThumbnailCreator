# Window module:
# Handles:
#   - ImGui-SFML

find_package(imgui CONFIG REQUIRED)

add_library(Window)
target_sources(Window
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Window.cxx
)
target_compile_options(Window PRIVATE ${PROJECT_WARNINGS})
target_link_libraries(Window PRIVATE imgui::imgui)


add_library(App)
target_sources(App
  PUBLIC
    FILE_SET CXX_MODULES FILES
    App.cxx
)
target_compile_options(App PRIVATE ${PROJECT_WARNINGS})
