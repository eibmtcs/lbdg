project(tools)

find_package(glfw3 REQUIRED)
find_package(PNGwriter 0.7.0)

add_library(${PROJECT_NAME}
    shader.cpp
    function.cpp
    fileget.cpp
    color.cpp
    camera.cpp
    point.cpp
    glad.c
)
add_library(sub::tools ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PNGwriter::PNGwriter
    -lglfw3
    -lGL
    -lm
    -lXrandr
    -lXi
    -lX11
    -lpthread
    -ldl
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/Models/include
        ${CMAKE_SOURCE_DIR}/src/Render/include
)
