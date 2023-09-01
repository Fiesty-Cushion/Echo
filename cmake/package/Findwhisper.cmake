# Findwhisper.cmake

# Package Info.
set(WHISPER_NAME "whisper")
set(WHISPER_DESCRIPTION "Port of OpenAI's Whisper model in C/C++.")
set(EXTERNAL_DIR "external")
# Package option.
option(USE_WHISPER ${WHISPER_DESCRIPTION} TRUE)
if (USE_WHISPER)
    add_definitions(-DUSE_WHISPER)
    # Define the repository URL and tag for Whisper
    set(WHISPER_URL "https://github.com/ggerganov/whisper.cpp")
    set(WHISPER_TAG "v1.4.0")
endif()

# Package data repository.
if (USE_WHISPER)
    set(FETCHCONTENT_QUIET off)

    get_filename_component(whisper_base "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/${WHISPER_NAME}"
        REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
    set(FETCHCONTENT_BASE_DIR ${whisper_base})
    # Declare the whisper library using FetchContent_Declare
    FetchContent_Declare(
        whisper
        GIT_REPOSITORY ${WHISPER_URL}
        GIT_TAG ${WHISPER_TAG}
        GIT_PROGRESS   TRUE
        USES_TERMINAL_DOWNLOAD TRUE
    )
    # Check if population has already been performed
    FetchContent_GetProperties(whisper)
    string(TOLOWER "${WHISPER_NAME}" lcName)
    if(NOT ${lcName}_POPULATED)
        FetchContent_Populate(${lcName})
        add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
    FetchContent_MakeAvailable(whisper)
endif()
