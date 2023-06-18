# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

set(activitytemplates_SRCS TemplatesPlugin.cpp)
qt_add_dbus_adaptor (
   activitytemplates_SRCS org.kde.ActivityManager.Templates.xml
   TemplatesPlugin.h TemplatesPlugin
   )

kamd_add_plugin(org.kde.ActivityManager.ActivityTemplates SOURCES ${activitytemplates_SRCS})

target_link_libraries(org.kde.ActivityManager.ActivityTemplates
   Qt::Core
   Qt::DBus
   KF6::ConfigCore
   KF6::DBusAddons
   KF6::CoreAddons
   kactivitymanagerd_plugin
   )

