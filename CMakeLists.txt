cmake_minimum_required(VERSION 3.25)
project(BlackShades LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

option(BLACKSHADES_SANITIZE "Build with AddressSanitizer and UBSan" OFF)

# Homebrew's openal-soft is keg-only, and CMake's stock FindOpenAL would pick
# Apple's deprecated framework, so point the config-mode search at it.
if(APPLE)
  find_program(BREW brew)
  if(BREW)
    execute_process(COMMAND ${BREW} --prefix openal-soft
      OUTPUT_VARIABLE OPENAL_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    list(APPEND CMAKE_PREFIX_PATH ${OPENAL_PREFIX})
  endif()
  find_package(OpenAL REQUIRED CONFIG)
else()
  find_package(OpenAL REQUIRED)
endif()

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(SDL3 3.4 REQUIRED CONFIG)
find_package(PkgConfig REQUIRED)
pkg_check_modules(VORBISFILE REQUIRED IMPORTED_TARGET vorbisfile)

add_executable(blackshades
  Source/Camera.cpp
  Source/Decals.cpp
  Source/Fog.cpp
  Source/Frustum.cpp
  Source/GameDraw.cpp
  Source/Gamepad.cpp
  Source/GameInitDispose.cpp
  Source/GameLoop.cpp
  Source/GameTick.cpp
  Source/Globals.cpp
  Source/MacInput.cpp
  Source/Main.cpp
  Source/Maths.cpp
  Source/Models.cpp
  Source/Person.cpp
  Source/Quaternions.cpp
  Source/Serialize.cpp
  Source/Skeleton.cpp
  Source/Sprites.cpp
  Source/Support.cpp
  Source/Text.cpp
  Source/Textures.cpp
  Source/Timer.cpp
)

target_include_directories(blackshades PRIVATE Source)
# The renderer builds GLfloat arrays from double expressions throughout.
target_compile_options(blackshades PRIVATE -Wno-narrowing)
target_link_libraries(blackshades PRIVATE
  SDL3::SDL3 OpenAL::OpenAL OpenGL::GL OpenGL::GLU PkgConfig::VORBISFILE)

if(BLACKSHADES_SANITIZE)
  target_compile_options(blackshades PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(blackshades PRIVATE -fsanitize=address,undefined)
endif()
