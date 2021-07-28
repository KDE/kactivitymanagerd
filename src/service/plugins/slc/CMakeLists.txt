# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (activitymanager-plugin-slc)

# kservice_desktop_to_json (activitymanager-plugin-slc.desktop)

set (
   slc_SRCS
   SlcPlugin.cpp
   )

qt_add_dbus_adaptor (
   slc_SRCS org.kde.ActivityManager.SLC.xml
   SlcPlugin.h SlcPlugin
   )

kcoreaddons_add_plugin(
   kactivitymanagerd_plugin_slc
   SOURCES ${slc_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_slc
   Qt::Core
   Qt::DBus
   KF5::ConfigCore
   KF5::DBusAddons
   KF5::CoreAddons
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_slc
   PROPERTIES PREFIX ""
   )
