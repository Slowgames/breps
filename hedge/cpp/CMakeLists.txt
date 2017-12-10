
project(hedge CXX)


add_library(hedge STATIC hedge.hpp hedge.cc)
target_include_directories(hedge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hedge glm easylogging++)

add_executable(hedge_test src/hedge_test.cc)
target_link_libraries(hedge_test hedge catch)
