cmake_minimum_required(VERSION 3.17)

project(breakthroughgl
  VERSION 0.0.1
  DESCRIPTION "A clone in OpenGL of the atari game Breaktrough"
  LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDE's
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

endif()

add_subdirectory(src)
add_subdirectory(app)
