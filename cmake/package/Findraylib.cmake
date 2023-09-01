# FindRaylib.cmake
include(FetchContent)

# Package Info.
set(RAYLIB_NAME "raylib")
set(RAYLIB_DESCRIPTION "A simple and easy-to-use library to enjoy videogames programming.")
set(EXTERNAL_DIR "external")
# Package option.
option(USE_RAYLIB ${RAYLIB_DESCRIPTION} TRUE)
if (USE_RAYLIB)
    add_definitions(-DUSE_RAYLIB)
    # Define the repository URL and tag for Raylib
    set(RAYLIB_URL "https://github.com/raysan5/raylib")
    set(RAYLIB_TAG "4.5.0")
endif()

# Package data repository.
if (USE_RAYLIB)
    set(FETCHCONTENT_QUIET off)

    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

    get_filename_component(raylib_base "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/${RAYLIB_NAME}"
        REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
    set(FETCHCONTENT_BASE_DIR ${raylib_base})
    # Declare the Raylib library using FetchContent_Declare
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY ${RAYLIB_URL}
        GIT_TAG ${RAYLIB_TAG}
        GIT_PROGRESS   TRUE
        USES_TERMINAL_DOWNLOAD TRUE
    )
    # Check if population has already been performed
    FetchContent_GetProperties(raylib)
    string(TOLOWER "${RAYLIB_NAME}" lcName)
    if(NOT ${lcName}_POPULATED)
        FetchContent_Populate(${lcName})
        add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
    FetchContent_MakeAvailable(raylib)
endif()
