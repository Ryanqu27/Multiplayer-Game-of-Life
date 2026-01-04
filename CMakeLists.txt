cmake_minimum_required(VERSION 3.15)
project(MultiplayerGameOfLife CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put binaries in build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ===== FIND PACKAGES =====
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

# ===== IMGUI SOURCES =====
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui-master/imgui-master)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# ===== PROJECT SOURCES =====
set(PROJECT_SOURCES
    src/main.cpp
    src/Board.cpp
    src/Cell.cpp
    src/GameWindow.cpp
    src/AIPlayer.cpp
)

add_executable(game-of-life
    ${PROJECT_SOURCES}
    ${IMGUI_SOURCES}
)

# ===== INCLUDE DIRECTORIES =====
target_include_directories(game-of-life PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# ===== LINK LIBRARIES =====
target_link_libraries(game-of-life PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    OpenGL::GL
    JsonCpp::JsonCpp
)

# ===== COMPILER FLAGS =====
if(MSVC)
    target_compile_options(game-of-life PRIVATE /W4)
else()
    target_compile_options(game-of-life PRIVATE -Wall -Wextra)
endif()