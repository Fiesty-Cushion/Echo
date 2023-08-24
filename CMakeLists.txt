cmake_minimum_required(VERSION 3.5)
project(Echo)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/Libraries/include ${CMAKE_SOURCE_DIR}/Libraries/include/raylib-cpp)
add_executable(${PROJECT_NAME} Audio.cpp GUI.h LoadingWindowGUI.cpp MainWindowGUI.h TextBox.h TextBox.cpp main.cpp utils.h Audio.h Globals.cpp LoadingWindowGUI.h Modes.h Transcriber.cpp pcm_stream.cpp Button.h Button.cpp Globals.h MainWindowGUI.cpp StartGUI.cpp Transcriber.h test.cpp DropdownList.h DropdownList.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Libraries/lib/raylib/libraylib.a ${CMAKE_SOURCE_DIR}/Libraries/lib/libportaudio.a ${CMAKE_SOURCE_DIR}/Libraries/lib/libwhisper.dylib )

if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework CoreServices")
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} "-framework AudioUnit")
    target_link_libraries(${PROJECT_NAME} "-framework AudioToolbox")
    target_link_libraries(${PROJECT_NAME} "-framework CoreAudio")

endif()


