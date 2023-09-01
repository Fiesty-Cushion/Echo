cmake_minimum_required(VERSION 3.22)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/package/")

# ------ PROJECT INFO ------
# Options
set(PROJECT_NAME "Echo" CACHE STRING "Echo")
set(PROJECT_TARGET ${PROJECT_NAME} CACHE STRING "Echo")
set(PROJECT_REAL_NAME "Echo" CACHE STRING "Echo")

project("Echo")

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS ON)
endif()

find_package(raylib REQUIRED)
find_package(whisper REQUIRED)
find_package(portaudio REQUIRED)
find_package(nfd REQUIRED)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/") # Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_EXTERNAL_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/external/include/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE} ${PROJECT_EXTERNAL_INCLUDE} ${raylib_INCLUDE} ${whisper_INCLUDE} ${portaudio_INCLUDE} ${nfd_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib whisper portaudio nfd)

# Copying the portaudio dll to the build directory
add_custom_command(TARGET Echo POST_BUILD       
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${PROJECT_SOURCE_DIR}/external/portaudio/portaudio-build/libportaudio.dll"     
        $<TARGET_FILE_DIR:Echo>)                