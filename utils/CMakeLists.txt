
project(utils CXX)


add_library(utils STATIC
  include/utils.h
  include/types.h
  include/sdl2_util.h src/sdl2_util.cc
  include/bgfx_util.h src/bgfx_util.cc
  )
target_include_directories(utils PUBLIC include)
target_link_libraries(utils bgfx SDL2-static)
