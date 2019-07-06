cmake_minimum_required(VERSION 3.14)
project(rawgl)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(rawgl
        aifcplayer.cpp
        aifcplayer.h
        bitmap.cpp
        bitmap.h
        engine.cpp
        engine.h
        file.cpp
        file.h
        graphics.h
        #graphics_gl.cpp
        graphics_soft.cpp
        intern.h
        main.cpp
        # mixer.cpp
        mixer_sdl1.cpp
        mixer.h
        pak.cpp
        pak.h
        resource.cpp
        resource.h
        resource_3do.cpp
        resource_3do.h
        resource_mac.cpp
        resource_mac.h
        resource_nth.cpp
        resource_nth.h
        resource_win31.cpp
        resource_win31.h
        scaler.cpp
        scaler.h
        screenshot.cpp
        screenshot.h
        script.cpp
        script.h
        sfxplayer.cpp
        sfxplayer.h
        staticres.cpp
        systemstub.h
        # systemstub_sdl.cpp
        systemstub_sdl1.cpp
        unpack.cpp
        unpack.h
        util.cpp
        util.h
        video.cpp
        video.h)

target_link_libraries(rawgl SDL SDL_mixer z)
