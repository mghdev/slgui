cmake_minimum_required(VERSION 3.13.0)
project(slgui VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_ttf EXCLUDE_FROM_ALL)

set(slgui_ROOT_PROJET OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(slgui_ROOT_PROJET ON)
    set(SLGUI_INCLUDE_DIRS 
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
else()
	set(SLGUI_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PARENT_SCOPE)
endif()

# Creates C resources file from files in given directory
function(create_resources dir output)
    file(WRITE ${output} "")
    file(GLOB bins ${dir}/*)
    foreach(bin ${bins})
        string(REGEX MATCH "([^/]+)$" filename ${bin})
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        file(READ ${bin} filedata HEX)
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        file(APPEND ${output} "const unsigned char ${filename}[] = {${filedata}};\nconst unsigned ${filename}_size = sizeof(${filename});\n")
    endforeach()
endfunction()
create_resources("assets/fonts" ${CMAKE_BINARY_DIR}/assets/fonts.c)

set(TARGET_NAME slgui)
add_subdirectory(src)
add_library(${TARGET_NAME} ${SOURCE_FILES} ${CMAKE_BINARY_DIR}/assets/fonts.c)

# find brew-installed standard libs
if(APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L/usr/local/opt/llvm/lib/c++")
endif()


target_include_directories( ${TARGET_NAME} PRIVATE ${SLGUI_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} SDL3_ttf::SDL3_ttf SDL3::SDL3)

if(slgui_ROOT_PROJET)
    add_subdirectory(demo)
endif()