# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (kactivitymanagerd-virtualdesktopswitch)

set (
   virtualdesktopswitch_SRCS
   VirtualDesktopSwitchPlugin.cpp
   ${plugin_implementation_SRCS}
   )

kcoreaddons_add_plugin(
   kactivitymanagerd_plugin_virtualdesktopswitch
   JSON kactivitymanagerd-plugin-virtualdesktopswitch.json
   SOURCES ${virtualdesktopswitch_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_virtualdesktopswitch
   Qt5::Core
   Qt5::DBus
   KF5::ConfigCore
   KF5::WindowSystem
   KF5::CoreAddons
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_virtualdesktopswitch
   PROPERTIES PREFIX ""
   )
