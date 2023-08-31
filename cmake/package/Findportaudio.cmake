# Findportaudio.cmake

# Package Info.
set(PORTAUDIO_NAME "portaudio")
set(PORTAUDIO_DESCRIPTION "Port of OpenAI's portaudio model in C/C++.")
set(EXTERNAL_DIR "external")
# Package option.
option(USE_PORTAUDIO ${PORTAUDIO_DESCRIPTION} TRUE)
if (USE_PORTAUDIO)
    add_definitions(-DUSE_PORTAUDIO)
    # Define the repository URL and tag for portaudio
    set(PORTAUDIO_URL "https://github.com/PortAudio/portaudio")
    set(PORTAUDIO_TAG "v19.7.0")
endif()

# Package data repository.
if (USE_PORTAUDIO)
    set(FETCHCONTENT_QUIET off)

    get_filename_component(portaudio_base "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/${PORTAUDIO_NAME}"
        REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
    set(FETCHCONTENT_BASE_DIR ${portaudio_base})
    # Declare the portaudio library using FetchContent_Declare
    FetchContent_Declare(
        portaudio
        GIT_REPOSITORY ${PORTAUDIO_URL}
        GIT_TAG ${PORTAUDIO_TAG}
        GIT_PROGRESS   TRUE
        USES_TERMINAL_DOWNLOAD TRUE
    )
    # Check if population has already been performed
    FetchContent_GetProperties(portaudio)
    string(TOLOWER "${PORTAUDIO_NAME}" lcName)
    if(NOT ${lcName}_POPULATED)
        FetchContent_Populate(${lcName})
        add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
    FetchContent_MakeAvailable(portaudio)
endif()
