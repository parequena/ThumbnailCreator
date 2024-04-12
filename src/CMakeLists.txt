# Window module:
# Handles:
#   - imgui

add_library(FileManager)
target_sources(FileManager
  PUBLIC
    FILE_SET CXX_MODULES FILES
    FileManager.cxx
)
target_compile_options(FileManager PRIVATE ${PROJECT_WARNINGS})
if(CMAKE_HOST_WIN32)
message("As building on Windows, we need comdlg32 to link FileManager")
target_link_libraries(FileManager PRIVATE comdlg32)
endif()

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
