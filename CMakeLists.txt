cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0048 NEW)
project(energetik VERSION 0.0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5DBus)
find_package(KF5Config)
find_library(LIBPROC_LIBRARY procps)

aux_source_directory(./src/service SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::DBus KF5::ConfigCore ${LIBPROC_LIBRARY})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
