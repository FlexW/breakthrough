file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${breakthroughgl_SOURCE_DIR}/include" "*.hpp")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "*.cpp")

add_library(breakthroughgl_library ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(breakthroughgl_library glad_library stb_library glfw)
target_include_directories(
  breakthroughgl_library
  PUBLIC
  ../include
  ../external
  ../external/glm
  ../external/glfw/include)
target_compile_features(breakthroughgl_library PUBLIC cxx_std_17)

target_compile_options(breakthroughgl_library PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
