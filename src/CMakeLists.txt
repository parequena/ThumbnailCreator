###############################################################################################
# Module    | FileManager
# Handles:
#   - File opener (Windows)
add_library(FileManager)
target_sources(FileManager
  PUBLIC
    FILE_SET CXX_MODULES FILES
    FileManager.cxx
)
target_compile_options(FileManager PRIVATE ${PROJECT_WARNINGS})

###############################################################################################
# Module    | UI
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

###############################################################################################
# Module    | Canvas
# Handles:
#   - Canvas Window
add_library(Canvas)
target_sources(Canvas
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Canvas.cxx
)
target_compile_options(Canvas PRIVATE ${PROJECT_WARNINGS})
target_link_libraries(Canvas PRIVATE UI)

###############################################################################################
# Module    | LayerHandler
# Handles:
#   - Layer's config
#   - Layers
add_library(LayerHandler)
target_sources(LayerHandler
  PUBLIC
    FILE_SET CXX_MODULES FILES
    Layer.cxx
    LayerHandler.cxx
)
target_compile_options(LayerHandler PRIVATE ${PROJECT_WARNINGS})
target_link_libraries(LayerHandler PRIVATE UI)

###############################################################################################
# Module    | Window
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
target_link_libraries(Window PRIVATE LayerHandler Canvas ImGui-SFML::ImGui-SFML)
