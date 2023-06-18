# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (activitymanager-globalshortcuts)

kcoreaddons_add_plugin(
   org.kde.ActivityManager.GlobalShortcuts
   SOURCES GlobalShortcutsPlugin.cpp
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   org.kde.ActivityManager.GlobalShortcuts
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

