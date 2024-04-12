
# ThumbnailCreator

[Youtube List (Spanish)](https://www.youtube.com/playlist?list=PLBEviA0cKSihsX5KPePVxA7B0QaV33-z0)

A tiny clone of [PhotoShop](https://www.adobe.com/products/photoshop.html), just to create [Youtbe Thumbnails](https://www.youtube.com/) on a fast way, this repo won't create a fully Photoshop clone and that's not intended.
I'll just wanna create fast Thumbnails to my own channel.
This repo is WIP.

## Features
- [ImGui](https://github.com/ocornut/imgui) : As UI creator/manager.
- [SFML](https://www.sfml-dev.org/) : As rendering API.
- [vcpkg](https://vcpkg.io/en/) : As a dependency manager.
- [C++ 20 Modules](https://en.cppreference.com/w/cpp/language/modules)
- [CMake](https://cmake.org/) : As porject generator.
- [Ninja](https://ninja-build.org/) : As building tool (C++20 Modules require Ninja at this time [April 2024])
 
## Installation and compilation
### Dependencies
* CMake 3.28 or later
* A build tool like Ninja
* A C++23-compliant compiler, such as:
  * Clang 17 or later (C++20 Modules require Clang17 at this time [April 2024])

#### Using CMake (preferred):
##### CMake presets:
* clang:
    * clang-debug

##### Linux (Tested on Arch WSL2.0)
Let's see an example with `clang-debug` as preset, but you can choose another from this list.
```
$ git clone --recurse-submodules git@github.com:parequena/ThumbnailCreator.git
#               git submodule update --init # if fails
$ mkdir build
$ cd build
$ cmake .. --preset clang-debug
$ cd clang-debug
$ cmake --build . -j
```

##### Windows
ImGUI-SFML fails building on Windows, so we require extra steps (for the purpose of this project, I won't handle all problems)
Tested on: Windows Powershell 7.4.1 (Windows 11)
```
$ git clone --recurse-submodules git@github.com:parequena/ThumbnailCreator.git
#               git submodule update --init # if fails
$ mkdir build
$ cd build
$ cmake .. --preset clang-debug # **This stop should fail**
$ cp -r .\clang-debug\vcpkg_installed\x64-windows\include\ .\clang-debug\vcpkg_installed\x64-windows\debug\
$ cmake .. --preset clang-debug # **This step will generate the project**
$ cd clang-debug
$ cmake --build . -j
```

## TODO
- All (KEK)

## Social Media
If you wanna get in touch:

[LinkedIn](https://www.linkedin.com/in/parequena/) <br/>
[Twitter (Spanish)](https://twitter.com/conPdePABLO) <br/>
[Twitch (Spanish)](https://www.twitch.tv/theApoCa): Not always streaming C++. <br/>
[Yotube (Spanish)](https://www.youtube.com/channel/UCnrucENG097xgewxhVe5toA): C++ programming courses (?) <br/>
