cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

include(settings)

make_executable(Editor)

find_package(SDL2       REQUIRED)
find_package(SDL2_gfx   REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_net   REQUIRED)

target_link_libraries(
    Editor PRIVATE
        SDL2::Main
        SDL2::GFX
        SDL2::Image
        SDL2::Mixer
        SDL2::Net
        Core
)
