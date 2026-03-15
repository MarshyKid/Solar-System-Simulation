cmake_minimum_required(VERSION 3.16)

project(solarSystem)

set(CMAKE_C_STANDARD 11)

add_executable(solarSystem
    src/main.c
    src/shader.c
    src/stb_image.c
    src/mesh.c
    src/shape.c
    src/camera.c
    src/render.c
    src/physics.c
    external/glad.c
)

target_include_directories(solarSystem PRIVATE
    include
)

target_link_libraries(solarSystem
    ${CMAKE_SOURCE_DIR}/external/libglfw3.a
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework IOKit"
)
