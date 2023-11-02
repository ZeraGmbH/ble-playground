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

find_package(Qt5 COMPONENTS Core Network Bluetooth Test REQUIRED)
find_package(VeinFramework REQUIRED)
find_package(ZenuxCore REQUIRED)

enable_testing()

# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt
function(SETUP_QTESTS)
    foreach(_testname ${ARGN})
        add_test(NAME ${_testname} COMMAND ${_testname})
        add_executable(${_testname} ${_testname}.h ${_testname}.cpp)
        target_include_directories(${_testname}
            PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            ${PROJECT_SOURCE_DIR}/src
            )
        target_link_libraries(${_testname}
            PUBLIC
            Qt5::Test
            ble-sensors
            )
    endforeach()
endfunction()


add_subdirectory(app)
add_subdirectory(lib)
add_subdirectory(tests)
