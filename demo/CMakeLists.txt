set(TARGET_NAME slgui_demo)

set(IMPLEMENTATION_FILES 
    slgui_demo_main.cpp)
set(SOURCE_FILES ${IMPLEMENTATION_FILES})

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PRIVATE 
    ${SLGUI_INCLUDE_DIRS})

target_link_libraries(${TARGET_NAME} slgui)
