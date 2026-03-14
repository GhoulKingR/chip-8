cmake_minimum_required(VERSION 4.0)
project(chip-8)

if(APPLE)
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRECTORIES})

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra)

if(DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -DDEBUG -g -fno-omit-frame-pointer -fsanitize=address)
    target_link_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti)
endif()
