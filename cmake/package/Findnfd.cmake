# FindRaylib.cmake
include(FetchContent)

# Package Info.
set(NFD_NAME "nfd")
set(NFD_DESCRIPTION "A simple and easy-to-use library to enjoy videogames programming.")
set(EXTERNAL_DIR "external")
# Package option.
option(USE_NFD ${NFD_DESCRIPTION} TRUE)
if (USE_NFD)
    add_definitions(-DUSE_NFD)
    # Define the repository URL and tag for Raylib
    set(NFD_URL "https://github.com/btzy/nativefiledialog-extended")
    set(NFD_TAG "v1.1.0")
endif()

# Package data repository.
if (USE_NFD)
    set(FETCHCONTENT_QUIET off)

    get_filename_component(nfd_base "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/${NFD_NAME}"
        REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
    set(FETCHCONTENT_BASE_DIR ${nfd_base})
    # Declare the Raylib library using FetchContent_Declare
    FetchContent_Declare(
        nfd
        GIT_REPOSITORY ${NFD_URL}
        GIT_TAG ${NFD_TAG}
        GIT_PROGRESS   TRUE
        USES_TERMINAL_DOWNLOAD TRUE
    )
    # Check if population has already been performed
    FetchContent_GetProperties(nfd)
    string(TOLOWER "${NFD_NAME}" lcName)
    if(NOT ${lcName}_POPULATED)
        FetchContent_Populate(${lcName})
        add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
    FetchContent_MakeAvailable(nfd)
endif()
