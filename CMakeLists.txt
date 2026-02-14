cmake_minimum_required(VERSION 3.13.0)
project(slgui VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 CONFIG REQUIRED)

set(slgui_ROOT_PROJET OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(slgui_ROOT_PROJET ON)
    set(SLGUI_INCLUDE_DIRS 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${SDL2_INCLUDE_DIRS})
else()
	set(SLGUI_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${SDL2_INCLUDE_DIRS}
        PARENT_SCOPE)
endif()

add_subdirectory(src)

if(slgui_ROOT_PROJET)
    add_subdirectory(demo)
endif()