cmake_minimum_required(VERSION 3.28)
project(tinyEngine VERSION 0.1.0 LANGUAGES CXX)

# Add C++23 as default Standard.
# No extensions.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Merge all warnings in just one Macro.
set(PROJECT_WARNINGS "${SANITIZER_FLAGS};${GENERAL_COMPILER_WARNINGS};${SPECIFIC_COMPILER_WARNINGS}")

# Adds al C++-Modules directories.
add_subdirectory(src)

# Create executable.
add_executable(ThumbnailCreator main.cpp)
target_link_libraries(ThumbnailCreator PRIVATE App ${SANITIZER_FLAGS})
