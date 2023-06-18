# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (kactivitymanagerd-runapplication)

set (
   runapplication_SRCS
   RunApplicationPlugin.cpp
   ${debug_SRCS}
   ${plugin_implementation_SRCS}
   )

kcoreaddons_add_plugin (
   org.kde.ActivityManager.RunApplication
   SOURCES ${runapplication_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries(org.kde.ActivityManager.RunApplication
   Qt::Core
   Qt::DBus
   Qt::Gui
   KF6::CoreAddons
   KF6::ConfigCore
   KF6::Service
   kactivitymanagerd_plugin
   )

