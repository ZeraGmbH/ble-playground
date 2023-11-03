file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

file(GLOB HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_executable(ble-playground-app
    ${SRCS}
    ${HEADERS}
)

target_link_libraries(ble-playground-app
    PRIVATE
    Qt5Core
    ble-sensors
    Zera::VfEvent
    Zera::VfTcp
    Zera::VfConvenientCode
)

install(TARGETS ble-playground-app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
