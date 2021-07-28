# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (kactivitymanagerd-plugin-sqlite)

set (
   sqliteplugin_SRCS
   Database.cpp
   StatsPlugin.cpp
   ResourceScoreCache.cpp
   ResourceScoreMaintainer.cpp
   ResourceLinking.cpp

   ${debug_SRCS}
   ${KACTIVITIES_CURRENT_ROOT_SOURCE_DIR}/src/common/database/Database.cpp
   ${KACTIVITIES_CURRENT_ROOT_SOURCE_DIR}/src/common/database/schema/ResourcesDatabaseSchema.cpp

   ${KACTIVITIES_CURRENT_ROOT_SOURCE_DIR}/src/utils/qsqlquery_iterator.cpp
   )

qt_add_dbus_adaptor (
   sqliteplugin_SRCS
   ${KACTIVITIES_CURRENT_ROOT_SOURCE_DIR}/src/common/dbus/org.kde.ActivityManager.ResourcesLinking.xml
   ResourceLinking.h ResourceLinking
   resourcelinkingadaptor
   )

qt_add_dbus_adaptor (
   sqliteplugin_SRCS
   ${KACTIVITIES_CURRENT_ROOT_SOURCE_DIR}/src/common/dbus/org.kde.ActivityManager.ResourcesScoring.xml
   StatsPlugin.h StatsPlugin
   resourcescoringadaptor
   )

kcoreaddons_add_plugin(
   kactivitymanagerd_plugin_sqlite
   SOURCES ${sqliteplugin_SRCS}
   INSTALL_NAMESPACE ${KAMD_PLUGIN_DIR}
   )

target_link_libraries (
   kactivitymanagerd_plugin_sqlite
   Qt::Core
   Qt::Sql
   KF5::ConfigCore
   KF5::KIOCore
   KF5::DBusAddons
   KF5::CoreAddons
   KF5::Crash
   kactivitymanagerd_plugin
   )

set_target_properties (
   kactivitymanagerd_plugin_sqlite
   PROPERTIES PREFIX ""
   )
