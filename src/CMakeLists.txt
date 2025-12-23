cmake_minimum_required(VERSION 3.24)

include(../cmake/raylib.cmake)

project(game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
    
FetchContent_Declare(
            enet
            GIT_REPOSITORY https://github.com/lsalzman/enet
            GIT_TAG master
)

FetchContent_MakeAvailable(enet)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.4
)

FetchContent_Declare(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
)

FetchContent_MakeAvailable(imgui rlimgui)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library")

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${LIB_DIR}/rlImGui.cpp  # L'integrazione raylib
)

add_executable(${PROJECT_NAME} main.cpp ${IMGUI_SOURCES})

# Add config.h.in file
configure_file(config.h.in config.h)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${enet_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}
    ${LIB_DIR}
    ${rlimgui_SOURCE_DIR}
    "${CMAKE_CURRENT_BINARY_DIR}"
)


target_link_libraries(${PROJECT_NAME} 
    enet
    raylib
)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "IsTextureValid(tex)=(tex.id > 0)")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE "IsTextureValid(tex)=(tex.id > 0)")
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE "IMGUI_IMPL_OPENGL_USE_VERTEX_ARRAY")

# Link to Raylib
# Static linking may be interesting, especially under Windows with MinGW,
# so that the MinGW DLLs do not have to be supplied.
# (Unfortunately, there is no easy way to link the C++ standard libraries
# statically and other libraries dynamically.)
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    target_link_libraries(${PROJECT_NAME} raylib)
else()
    target_link_libraries(${PROJECT_NAME} raylib -static)
endif()

# Copy assets to assets path
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/../cmake/copy-assets.cmake"
        )
add_dependencies(${PROJECT_NAME} copy_assets)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()

# CPack instructions
# call e.g. cpack -G ZIP -C Debug to create a binary distribution
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_NAME}_Bundle")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets" DESTINATION "${PROJECT_NAME}_Bundle")