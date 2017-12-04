
project(mesh CXX)


add_library(mesh STATIC include/mesh.h src/mesh.cc)
target_include_directories(mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
