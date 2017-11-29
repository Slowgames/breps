
project(easylogging CXX)


add_library(easylogging++ easylogging++.h easylogging++.cc)
target_include_directories(easylogging++ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
