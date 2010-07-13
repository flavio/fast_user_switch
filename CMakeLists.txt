project(plasma-fastuserswitch)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

set(fastuserswitch_SRCS
    fastuserswitch.cpp
    sessionwidget.cpp)

kde4_add_plugin(plasma_applet_fastuserswitch ${fastuserswitch_SRCS})
target_link_libraries(plasma_applet_fastuserswitch ${KDE4_PLASMA_LIBS} kworkspace)

install(TARGETS plasma_applet_fastuserswitch DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-fastuserswitch.desktop DESTINATION ${SERVICES_INSTALL_DIR})
