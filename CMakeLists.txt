cmake_minimum_required(VERSION 3.20)
project(Basic_Ray_Tracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)

add_executable(Basic_Ray_Tracer
        include/Angel.h
        include/CheckError.h
        include/InitShader.cpp
        include/mat.h
        include/vec.h
        global.h
        object.cpp
        object.h
        raycast.cpp
        scene.cpp
        scene.h
        sphere.cpp
        sphere.h
        trace.cpp
        trace.h
        vector.cpp
        vector.h)
