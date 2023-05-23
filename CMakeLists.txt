cmake_minimum_required(VERSION 3.12)
project(sdl_basegame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")     # baseclass functions have plenty unused parameters
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-attributes")   # clang does not know [[(un)likely]]
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

set(CMAKE_CXX_FLAGS_DEBUG   "-Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

if(APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/CMAKE/FINDSDL2.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/CMAKE/FINDSDL2_IMAGE.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/CMAKE/FINDSDL2Mixer.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/CMAKE/FINDSDL2TTF.cmake)
endif()

add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")

add_executable(sdl_basegame
        src/lib/global.h
        src/gamebase.h
        src/gamebase.cpp
        src/lib/recthelper.h

        src/main.cpp
        src/gorillagame.cpp
        src/gorillagame.h

        src/GameStates/introstate.cpp src/GameStates/MainState.cpp src/GameObjects/Player.cpp src/GameObjects/Player.h src/GameObjects/Enemy.cpp src/GameObjects/Enemy.h src/GameObjects/Teleport.cpp src/GameObjects/Teleport.h src/GameObjects/Bullet.cpp src/GameObjects/Bullet.h src/GameObjects/Items.cpp src/GameObjects/Items.h src/Map/Room.cpp src/Map/Room.h)

include(FindPkgConfig)

if (MSVC)
    # still does not work in my old VS 2017, but at least it does not complain
	# if someone finds out how to easily use the nuget packages here let me know
else()
    pkg_search_module(SDL2      REQUIRED sdl2>=2.0.0)
    pkg_search_module(SDL2TTF   REQUIRED SDL2_ttf>=2.0.0)
    pkg_search_module(SDL2NET   REQUIRED SDL2_net>=2.0.0)
    pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
    pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    #pkg_search_module(SDL2GFX   REQUIRED SDL2_gfx>=1.0.0) # this 1.x.x is correct
endif()

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIRS}
        ${SDL2NET_INCLUDE_DIRS}
        ${SDL2MIXER_INCLUDE_DIRS}
        ${SDL2IMAGE_INCLUDE_DIRS}
        #${SDL2GFX_INCLUDE_DIRS}
)

target_link_libraries(
        ${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2TTF_LIBRARIES}
        ${SDL2NET_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES}
        #${SDL2GFX_LIBRARIES}
)
