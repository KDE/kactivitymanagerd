# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

project (ActivityManager)

# General
find_package (ECM 0.0.8 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

find_package (Qt5 REQUIRED NO_MODULE COMPONENTS Sql Gui Widgets)

find_package (KF5Config ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5CoreAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5WindowSystem ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5GlobalAccel ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5XmlGui ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5Crash ${KF5_MIN_VERSION} CONFIG REQUIRED)


# Standard stuff

add_library(kactivitymanagerd_plugin SHARED Plugin.cpp Module.cpp Event.cpp ${debug_SRCS})
generate_export_header(kactivitymanagerd_plugin)
target_link_libraries(kactivitymanagerd_plugin PUBLIC Qt::Core Qt::DBus KF5::CoreAddons KF5::ConfigCore)

add_subdirectory (plugins)

set (kactivitymanager_SRCS
   Application.cpp
   ${KACTIVITIES_CURRENT_ROOT_SOURCE_DIR}/src/common/dbus/org.kde.ActivityManager.Activities.cpp

   ${debug_SRCS}
   Activities.cpp
   Resources.cpp
   Features.cpp
   Config.cpp

   ksmserver/KSMServer.cpp
   )

qt_add_dbus_adaptor (
   kactivitymanager_SRCS
   ../common/dbus/org.kde.ActivityManager.Activities.xml
   Activities.h Activities
   )

qt_add_dbus_adaptor (
   kactivitymanager_SRCS
   ../common/dbus/org.kde.ActivityManager.Resources.xml
   Resources.h Resources
   )

qt_add_dbus_adaptor (
   kactivitymanager_SRCS
   ../common/dbus/org.kde.ActivityManager.Features.xml
   Features.h Features
   )

add_executable (kactivitymanagerd ${kactivitymanager_SRCS})

target_link_libraries (kactivitymanagerd
   Qt::Core
   Qt::DBus
   Qt::Gui
   Qt::Widgets
   KF5::DBusAddons
   KF5::CoreAddons
   KF5::ConfigCore
   KF5::I18n
   KF5::WindowSystem
   KF5::Crash
   kactivitymanagerd_plugin
   )

########### install application ###############

install (FILES
   files/kactivitymanagerd.desktop
   DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
   )

install (TARGETS
    kactivitymanagerd
    DESTINATION ${KDE_INSTALL_LIBEXECDIR}
   )

install (TARGETS
   kactivitymanagerd_plugin ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
   )

install (FILES
   files/kactivitymanagerd-plugin.desktop
   DESTINATION ${KDE_INSTALL_KSERVICETYPES5DIR}
   )

