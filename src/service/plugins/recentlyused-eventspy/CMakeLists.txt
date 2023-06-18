# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (activitymanager-recentlyused-eventspy)

find_package (KF6KIO ${KF6_MIN_VERSION} CONFIG REQUIRED)

set (
   recentlyusedeventspy_SRCS
   RecentlyUsedEventSpy.cpp
   )

ecm_qt_declare_logging_category(recentlyusedeventspy_SRCS
   HEADER DebugPluginRecentlyUsedEventSpy.h
   IDENTIFIER KAMD_LOG_PLUGIN_RECENTLYUSED_EVENTSPY
   CATEGORY_NAME org.kde.kactivities.plugin.recentlyused-eventspy
   DEFAULT_SEVERITY Warning
   DESCRIPTION "KActivities RecentlyUsed Event Spy Resources" EXPORT KACTIVITYMANAGERD)

kcoreaddons_add_plugin(
   org.kde.ActivityManager.GtkEventSpy
   SOURCES ${recentlyusedeventspy_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries(org.kde.ActivityManager.GtkEventSpy
   Qt::Core
   Qt::Xml
   KF6::CoreAddons
   KF6::Service
   kactivitymanagerd_plugin
   )

