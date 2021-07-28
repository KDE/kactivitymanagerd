# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (kactivitymanagerd-plugin-activitytemplates)

set (
   activitytemplates_SRCS
   TemplatesPlugin.cpp
   )

qt_add_dbus_adaptor (
   activitytemplates_SRCS org.kde.ActivityManager.Templates.xml
   TemplatesPlugin.h TemplatesPlugin
   )

kcoreaddons_add_plugin(
   kactivitymanagerd_plugin_activitytemplates
   SOURCES ${activitytemplates_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_activitytemplates
   Qt::Core
   Qt::DBus
   KF5::ConfigCore
   KF5::DBusAddons
   KF5::CoreAddons
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_activitytemplates
   PROPERTIES PREFIX ""
   )
