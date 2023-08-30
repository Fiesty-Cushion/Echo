list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/packages")

message("Hello There")

set(THIRD_PARTY "third-party" CACHE STRING "3rdparty folder for project dependencies. [Don't change this variable.]")

find_package(Git)
if(Git_FOUND)
  message("Git found: ${GIT_EXECUTABLE}")
endif()

find_package(raylib REQUIRED)
find_package(whisper REQUIRED)
find_package(portaudio REQUIRED)

if (USE_CUSTOM_ENGINE)
  find_package(${ENGINE_CODE_NAME}  REQUIRED)
endif()