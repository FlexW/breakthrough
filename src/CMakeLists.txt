file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${breakthroughgl_SOURCE_DIR}/include" "*.hpp")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "*.cpp")

find_package(Freetype REQUIRED)

add_library(breakthroughgl_library ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(breakthroughgl_library
  glad_library
  stb_library
  glfw openal
  ${FREETYPE_LIBRARIES})
target_include_directories(
  breakthroughgl_library
  PUBLIC
  ../include
  ../external
  ../external/glm
  ../external/glfw/include
  ../external/openal-soft/include
  ${FREETYPE_INCLUDE_DIR_ft2build}
  ${FREETYPE_INCLUDE_DIR_freetype2})
target_compile_features(breakthroughgl_library PUBLIC cxx_std_20)

target_compile_options(breakthroughgl_library PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

# Link resource directory
set (source "${CMAKE_CURRENT_SOURCE_DIR}/../resources")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/../resources")
add_custom_command(
  TARGET breakthroughgl_library POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
  DEPENDS ${destination}
  COMMENT "symbolic link resources folder from ${source} => ${destination}"
)

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
