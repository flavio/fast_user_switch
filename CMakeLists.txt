project(plasma-fastuserswitch)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})

set(fastuserswitch_SRCS
    common.cpp
    fastuserswitch.cpp
    sessionwidget.cpp)
kde4_add_ui_files(fastuserswitch_SRCS fastuserswitchConfig.ui)

kde4_add_plugin(plasma_applet_fastuserswitch ${fastuserswitch_SRCS})
target_link_libraries(plasma_applet_fastuserswitch ${KDE4_PLASMA_LIBS} kworkspace)

install(TARGETS plasma_applet_fastuserswitch DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-fastuserswitch.desktop DESTINATION ${SERVICES_INSTALL_DIR})
