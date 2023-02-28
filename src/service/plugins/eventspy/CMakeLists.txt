# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (activitymanager-eventspy)

find_package (KF6KIO ${KF6_MIN_VERSION} CONFIG REQUIRED)

set (
   evenyspy_SRCS
   EventSpy.cpp
   )

kcoreaddons_add_plugin(
   kactivitymanagerd_plugin_eventspy
   SOURCES ${evenyspy_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_eventspy
   Qt::Core
   Qt::DBus
   KF6::ConfigCore
   KF6::CoreAddons
   KF6::KIOCore
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_eventspy
   PROPERTIES PREFIX ""
   )
