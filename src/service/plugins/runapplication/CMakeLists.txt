# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

kamd_add_plugin(org.kde.ActivityManager.RunApplication SOURCES RunApplicationPlugin.cpp)

target_link_libraries(org.kde.ActivityManager.RunApplication
   Qt::Core
   Qt::DBus
   Qt::Gui
   KF6::CoreAddons
   KF6::ConfigCore
   KF6::Service
   kactivitymanagerd_plugin
   debug_STATIC
   )

