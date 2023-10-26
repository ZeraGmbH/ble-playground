cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME ble-playground)

project(${PROJECT_NAME}
    LANGUAGES CXX
    VERSION 1.0.0
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(Qt5 COMPONENTS Core Network Bluetooth REQUIRED)
find_package(VeinFramework REQUIRED)
find_package(ZenuxCore REQUIRED)

add_subdirectory(src)
