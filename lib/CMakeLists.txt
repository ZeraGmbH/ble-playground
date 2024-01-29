file(GLOB SOURCES
    *.cpp
    decoders/*.cpp
    )

file(GLOB HEADER
    *.h
    decoders/*.h
    )

add_library(ble-sensors SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(ble-sensors
    PUBLIC
    Qt5::Core
    Qt5::Bluetooth
    Zera::zera-timers
    )

target_include_directories(ble-sensors
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/decoders>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ble-sensors>
    )

set_target_properties(ble-sensors PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ble-sensors PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS ble-sensors
    EXPORT BlePlaygroundExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

generate_export_header(ble-sensors)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ble-sensors_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ble-sensors
    )

export(TARGETS ble-sensors FILE BlePlaygroundTargets.cmake)
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ble-sensors
    )
