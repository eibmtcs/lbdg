project (render)

add_library(${PROJECT_NAME}
    render.cpp
)
add_library(sub::render ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    sub::tools
    sub::models
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
)
