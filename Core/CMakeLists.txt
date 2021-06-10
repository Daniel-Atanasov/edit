cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

include(settings)

make_static_library(Core)

find_package(Freetype   REQUIRED)
find_package(harfbuzz   REQUIRED)
# find_package(re2        REQUIRED)

target_link_libraries(
    Core PUBLIC
        Freetype::Freetype
        harfbuzz
        # re2::re2
)
