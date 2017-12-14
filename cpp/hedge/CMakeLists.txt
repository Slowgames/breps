
project(hedge CXX)


add_library(hedge STATIC hedge.hpp hedge.cpp)
target_include_directories(hedge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hedge mathfu easylogging++)

add_executable(hedge_test hedge_test.cpp)
target_link_libraries(hedge_test hedge catch)
