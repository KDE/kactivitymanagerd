# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (kactivitymanagerd-activityrunner)

set (
   activityrunner_SRCS
   ActivityRunner.cpp
   ${debug_SRCS}
   ${plugin_implementation_SRCS}
   )
qt_add_dbus_adaptor(activityrunner_SRCS "org.kde.krunner1.xml" ActivityRunner.h ActivityRunner)

kcoreaddons_add_plugin (
   kactivitymanagerd_plugin_activityrunner
   SOURCES ${activityrunner_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_activityrunner
   Qt::Core
   Qt::DBus
   KF5::CoreAddons
   KF5::I18n
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_activityrunner
   PROPERTIES PREFIX ""
   )
install(
        FILES plasma-runnners-activities.desktop
        DESTINATION "${KDE_INSTALL_DATAROOTDIR}/krunner/dbusplugins"
)


