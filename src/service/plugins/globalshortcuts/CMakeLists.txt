# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (activitymanager-globalshortcuts)

set (
   globalshortcuts_SRCS
   GlobalShortcutsPlugin.cpp
   )

kcoreaddons_add_plugin(
   kactivitymanagerd_plugin_globalshortcuts
   SOURCES ${globalshortcuts_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_globalshortcuts
   Qt::Core
   Qt::DBus
   Qt::Widgets
   KF6::ConfigCore
   KF6::GlobalAccel
   KF6::I18n
   KF6::XmlGui
   KF6::CoreAddons
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_globalshortcuts
   PROPERTIES PREFIX ""
   )
