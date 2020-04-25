add_executable(breaktroughgl main.cpp)
target_compile_features(breaktroughgl PRIVATE cxx_std_11)
target_include_directories(
  breaktroughgl
  PUBLIC
  ../include
  ../external)

target_compile_options(breaktroughgl PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

target_link_libraries(breaktroughgl PRIVATE breakthroughgl_library)

install(TARGETS breaktroughgl)
