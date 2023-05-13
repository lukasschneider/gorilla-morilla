if(SDL2Mixer_FIND_QUIETLY)
    set(_FIND_SDL2_ARG QUIET)
endif()

find_package(SDL2 ${_FIND_SDL2_ARG})
find_path(SDL2Mixer_INCLUDE_DIR SDL_mixer.h PATH_SUFFIXES SDL2 HINTS ENV SDL2DIR)
mark_as_advanced(SDL2Mixer_INCLUDE_DIR)
find_library(SDL2Mixer_LIBRARY SDL2_mixer HINTS ENV SDL2DIR)
mark_as_advanced(SDL2Mixer_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2Mixer REQUIRED_VARS SDL2Mixer_LIBRARY SDL2Mixer_INCLUDE_DIR
        SDL2_LIBRARY SDL2_INCLUDE_DIR)

if (SDL2Mixer_FOUND)
    set(SDL2Mixer_LIBRARIES ${SDL2Mixer_LIBRARY} ${SDL2_LIBRARIES})
    set(SDL2Mixer_INCLUDE_DIRS ${SDL2Mixer_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})
endif()