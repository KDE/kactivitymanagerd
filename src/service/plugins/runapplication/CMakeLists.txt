# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (kactivitymanagerd-runapplication)

set (
   runapplication_SRCS
   RunApplicationPlugin.cpp
   ${debug_SRCS}
   ${plugin_implementation_SRCS}
   )

kcoreaddons_add_plugin (
   kactivitymanagerd_plugin_runapplication
   SOURCES ${runapplication_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_runapplication
   Qt::Core
   Qt::DBus
   Qt::Gui
   KF5::CoreAddons
   KF5::ConfigCore
   KF5::Service
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_runapplication
   PROPERTIES PREFIX ""
   )

