set(TARGET_NAME slgui)

set(PUBLIC_HEADER_FILES
    SLApplication.hpp
    SLBase.hpp
    SLConfigModel.hpp
    SLEvent.hpp
    SLGeometry.hpp
    SLView.hpp
    SLViewController.hpp
    SLWindow.hpp)
set(SOURCE_FILES ${PUBLIC_HEADER_FILES})
list(APPEND SOURCE_FILES 
    SLApplication.cpp
    SLConfigModel.cpp
    SLEvent.cpp
    SLView.cpp 
    SLViewController.cpp
    SLWindow.cpp)

add_library(${TARGET_NAME} ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PRIVATE 
    ${SLGUI_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS})

target_link_libraries(Silhouette ${SDL2_LIBRARIES} SDL2_image slgui)
