set(CMAKE_TOOLCHAIN_FILE $ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake)
cmake_minimum_required(VERSION 3.0.0)
project(NBodyParticleSim VERSION 0.1.0)

set(DCMAKE_PREFIX_PATH $ENV{HOME}/NGL)
find_package(NGL CONFIG REQUIRED)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS OpenGL Widgets REQUIRED )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")
include_directories("$ENV{HOME}/NGL/include")
include_directories("${Qt6Widgets_INCLUDE_DIRS}")
add_subdirectory(nbodysim)

add_executable(NBodyUI src/main.cpp src/appwindow.cpp include/appwindow.h)


target_link_libraries(NBodyUI PRIVATE NBodyParticleSim)

target_link_libraries(NBodyUI PRIVATE NGL Qt::Widgets Qt::OpenGL)