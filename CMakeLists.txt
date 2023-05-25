cmake_minimum_required(VERSION 3.12)
project(sdl_basegame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4100")     # baseclass functions have plenty unused parameters

add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")

add_executable(sdl_basegame
        src/lib/global.h
        src/gamebase.h
        src/gamebase.cpp
        src/lib/recthelper.h

        src/main.cpp
        src/gorillagame.cpp
        src/gorillagame.h

        src/GameStates/introstate.cpp src/GameStates/MainState.cpp src/GameObjects/Player.cpp src/GameObjects/Player.h src/GameObjects/Enemy.cpp src/GameObjects/Enemy.h src/GameObjects/Teleport.cpp src/GameObjects/Teleport.h src/GameObjects/Bullet.cpp src/GameObjects/Bullet.h src/GameObjects/Items.cpp src/GameObjects/Items.h src/Map/Room.cpp src/Map/Room.h src/lib/Camera.cpp src/lib/Camera.h)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(sdl_basegame
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        )

    find_package(SDL2_image CONFIG REQUIRED)
    target_link_libraries(sdl_basegame PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

    find_package(SDL2_mixer CONFIG REQUIRED)
    target_link_libraries(sdl_basegame PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)

    find_package(SDL2_ttf CONFIG REQUIRED)
    target_link_libraries(sdl_basegame PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
elseif ()
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