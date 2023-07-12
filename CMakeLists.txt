cmake_minimum_required(VERSION 3.5)
project(Echo)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/Libraries/include ${CMAKE_SOURCE_DIR}/Libraries/include/raylib-cpp)
add_executable(${PROJECT_NAME} GUI.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Libraries/lib/raylib/libraylib.a)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
