cmake_minimum_required(VERSION 3.28)
project(tinyEngine VERSION 0.1.0 LANGUAGES CXX)

# Add C++23 as default Standard.
# No extensions.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set all warnings as errors!
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Merge all warnings in just one Macro.
set(PROJECT_WARNINGS "${SANITIZER_FLAGS};${GENERAL_COMPILER_WARNINGS};${SPECIFIC_COMPILER_WARNINGS}")

# Adds al C++-Modules directories.
add_subdirectory(src)

# Create executable.
add_executable(ThumbnailCreator main.cpp)
target_compile_options(ThumbnailCreator PRIVATE ${PROJECT_WARNINGS})
target_link_libraries(ThumbnailCreator PRIVATE ${SANITIZER_FLAGS} Window FileManager)

#
# Compiler Warnings
# Unix  :
# -pedantic -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough -Weffc++
# Gcc   :
# -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast -Wlifetime
# Clang : 
# -Weverything -Wno-c++98-compat
# Windows :
# /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928
