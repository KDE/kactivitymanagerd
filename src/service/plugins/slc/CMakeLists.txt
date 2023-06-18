# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

qt_add_dbus_adaptor(slc_DBUS_SRCS org.kde.ActivityManager.SLC.xml SlcPlugin.h SlcPlugin)

kamd_add_plugin(org.kde.ActivityManager.SLC SOURCES SlcPlugin.cpp ${slc_DBUS_SRCS})

target_link_libraries(org.kde.ActivityManager.SLC
   Qt::Core
   Qt::DBus
   KF6::ConfigCore
   KF6::DBusAddons
   KF6::CoreAddons
   kactivitymanagerd_plugin
)

