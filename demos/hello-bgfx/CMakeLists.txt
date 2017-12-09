

add_executable(hello_bgfx main.cpp)
set_target_properties(hello_bgfx
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  )
target_link_libraries(hello_bgfx SDL2main SDL2-static bgfx easylogging++)
