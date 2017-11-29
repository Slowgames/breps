
project(mesh CXX)


add_library(mesh STATIC include/mesh.h src/mesh.cc)
target_include_directories(mesh INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
