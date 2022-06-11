cmake_minimum_required(VERSION 3.13)
project(
	pacreset
	VERSION 1.0.0
	DESCRIPTION "A recovery program to reset Arch Linux package file permissions"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(LibArchive REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LibAlpm REQUIRED libalpm)

add_definitions(-D_XOPEN_SOURCE=700)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

add_executable(pacreset main.cpp)
target_link_libraries(pacreset ${LibAlpm_LIBRARIES} ${LibArchive_LIBRARIES})
