project (print)

add_library(${PROJECT_NAME}
    print.cpp
)
add_library(sub::print ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    sub::tools
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
)
