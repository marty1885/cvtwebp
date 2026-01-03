cmake_minimum_required(VERSION 3.15)
project(cvtwebp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Magick++ REQUIRED IMPORTED_TARGET Magick++)

add_executable(cvtwebp main.cpp)
target_link_libraries(cvtwebp PRIVATE PkgConfig::Magick++)
install(TARGETS cvtwebp)

