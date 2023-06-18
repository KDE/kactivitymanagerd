# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (kactivitymanagerd-virtualdesktopswitch)

set (
   virtualdesktopswitch_SRCS
   VirtualDesktopSwitchPlugin.cpp
   ${plugin_implementation_SRCS}
   )

kcoreaddons_add_plugin(
   org.kde.ActivityManager.VirtualDesktopSwitch
   SOURCES ${virtualdesktopswitch_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries(org.kde.ActivityManager.VirtualDesktopSwitch
   Qt::Core
   Qt::DBus
   KF6::ConfigCore
   KF6::WindowSystem
   KF6::CoreAddons
   kactivitymanagerd_plugin
   )

