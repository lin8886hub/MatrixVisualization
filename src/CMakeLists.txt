cmake_minimum_required(VERSION 3.16)

project(martix)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})


find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

#自动收集所有源文件（关键）
file(GLOB SRC_FILES
    *.cpp
    *.h
)

add_executable(martix ${SRC_FILES})

target_link_libraries(martix PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
    opengl32
)