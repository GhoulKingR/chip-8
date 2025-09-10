cmake_minimum_required(VERSION 4.0)
project(chip-8)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRECTORIES})

file(GLOB SOURCES CONFIGURE_DEPENDS "*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

