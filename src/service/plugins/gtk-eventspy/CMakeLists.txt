# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (activitymanager-gtk-eventspy)

find_package (KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)

set (
   gtkevenyspy_SRCS
   GtkEventSpy.cpp
   )

ecm_qt_declare_logging_category(gtkevenyspy_SRCS
   HEADER DebugPluginGtkEventSpy.h
   IDENTIFIER KAMD_LOG_PLUGIN_GTK_EVENTSPY
   CATEGORY_NAME org.kde.kactivities.plugin.gtk-eventspy
   DEFAULT_SEVERITY Warning
   DESCRIPTION "KActivities Gtk Event Spy Resources" EXPORT KACTIVITYMANAGERD)

kcoreaddons_add_plugin(
   kactivitymanagerd_plugin_gtk_eventspy
   SOURCES ${gtkevenyspy_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_gtk_eventspy
   Qt::Core
   Qt::Xml
   KF5::CoreAddons
   KF5::Service
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_gtk_eventspy
   PROPERTIES PREFIX ""
   )
