cmake_minimum_required(VERSION 4.0)
project(chip-8)

set(SDL2 /Library/Frameworks/SDL2.framework)
set(SDL2_image /Library/Frameworks/SDL2_image.framework)

add_compile_options(-F/Library/Frameworks)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${SDL2}/Versions/A/SDL2
    ${SDL2_image}/Versions/A/SDL2_image
)

