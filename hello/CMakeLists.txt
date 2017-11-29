

add_executable(hello main.cpp)
set_target_properties(hello
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  )
target_link_libraries(hello SDL2main SDL2-static bgfx easylogging++)
