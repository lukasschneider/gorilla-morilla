cmake_minimum_required(VERSION 3.12)
project(gorillagame)

set(CMAKE_CXX_STANDARD 17)
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4100")
    # -Wno-unknown-attributes gibt es auf MSVC nicht.
    # #pragma warning(disable: warning_number) könnte jeweils in den Sourcefiles benutzt werden
    # TODO: Windows Optimisierungs-Flags fixen
    #set(CMAKE_CXX_FLAGS_DEBUG   /Od)
    #set(CMAKE_CXX_FLAGS_RELEASE /O2)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter") # baseclass functions have plenty unused parameters
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-attributes")
    set(CMAKE_CXX_FLAGS_DEBUG   "-Og")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif ()

add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")

if(APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/CMAKE/FINDSDL2.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/CMAKE/FINDSDL2_IMAGE.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/CMAKE/FINDSDL2Mixer.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/CMAKE/FINDSDL2TTF.cmake)
endif()

add_executable(${PROJECT_NAME}
        src/lib/global.h
        src/gamebase.h
        src/gamebase.cpp
        src/lib/recthelper.h
        src/main.cpp
        src/gorillagame.cpp
        src/gorillagame.h
        src/GameStates/MainState.cpp
        src/GameObjects/Player.cpp
        src/GameObjects/Player.h
        src/GameObjects/Enemy.cpp
        src/GameObjects/Enemy.h
        src/Map/Room.cpp
        src/Map/Room.h
        src/GameObjects/Gun.cpp src/GameObjects/Gun.h src/GameObjects/Bullet.cpp src/GameObjects/Bullet.h src/Map/RoomManager.cpp src/Map/RoomManager.h)
if(NOT WIN32)
    include(FindPkgConfig)
endif ()

if(WIN32)
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
            $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
            )

    find_package(SDL2_image CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

    find_package(SDL2_mixer CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)

    find_package(SDL2_ttf CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
else ()
    pkg_search_module(SDL2      REQUIRED sdl2>=2.0.0)
    pkg_search_module(SDL2TTF   REQUIRED SDL2_ttf>=2.0.0)
    pkg_search_module(SDL2NET   REQUIRED SDL2_net>=2.0.0)
    pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
    pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    #pkg_search_module(SDL2GFX   REQUIRED SDL2_gfx>=1.0.0) # this 1.x.x is correct

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
endif ()