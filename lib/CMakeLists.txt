file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_library(ble-sensors SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(ble-sensors
    PUBLIC
    Qt5::Core
    Qt5::Bluetooth
    )

target_include_directories(ble-sensors
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ble-sensors>
    )

set_target_properties(ble-sensors PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ble-sensors PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

write_basic_package_version_file(
    ble-sensorsConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )
configure_file(ble-sensorsConfig.cmake.in ble-sensorsConfig.cmake @ONLY)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ble-sensorsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ble-sensorsConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ble-sensors
    )

export(TARGETS ble-sensors NAMESPACE Zera:: FILE ble-sensorsTargets.cmake)
install(TARGETS ble-sensors
    EXPORT ble-sensorsExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ble-sensors
    )
