
project(hedge CXX)

add_definitions(-DHEDGE_ENABLE_DEFAULT_KERNEL)

add_library(hedge STATIC include/hedge.hpp src/hedge.cc)
target_include_directories(hedge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(hedge glm easylogging++)

add_executable(hedge_test src/hedge_test.cc)
target_link_libraries(hedge_test hedge catch)
