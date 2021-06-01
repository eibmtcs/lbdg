project (models)

add_library(${PROJECT_NAME}
    model.cpp
    command.cpp
    line.cpp
    optionalLine.cpp
    quadrilateral.cpp
    subFile.cpp
    triangle.cpp
)
add_library(sub::models ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    sub::tools
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
)
