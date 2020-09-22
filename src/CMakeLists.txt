# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

# Boosting us a bit

find_package (Boost 1.49 REQUIRED)
include_directories (${Boost_INCLUDE_DIR})

string (REGEX MATCH "1053.." BOOST_VERSION_BLACKLISTED ${Boost_VERSION})

if (BOOST_VERSION_BLACKLISTED AND NOT KACTIVITIES_ENABLE_EXCEPTIONS)
   message (
      WARNING
      "Boost.Container 1.53 has issues when exceptions are disabled. "
      "We will set the KACTIVITIES_ENABLE_EXCEPTIONS option."
      )
   set (KACTIVITIES_ENABLE_EXCEPTIONS ON)
endif ()

if (KACTIVITIES_ENABLE_EXCEPTIONS)
   string (REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   add_definitions (-fexceptions)
endif ()

# =======================================================
# Starting the actual project definition

# Config file
set (KAMD_PLUGIN_VERSION 1)
set (KAMD_PLUGIN_DIR "kactivitymanagerd/${KAMD_PLUGIN_VERSION}")
set (KAMD_FULL_PLUGIN_DIR "${KDE_INSTALL_FULL_PLUGINDIR}/${KAMD_PLUGIN_DIR}/")
configure_file (kactivities-features.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/kactivities-features.h)

include_directories (
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}
   )

# Is the compiler modern enough to build the ActivityManager service
# and accompanying workspace addons?

string (COMPARE EQUAL "${CXX_FEATURES_UNSUPPORTED}" "" CXX_COMPILER_IS_MODERN)

# The compiler is good enough
if (CXX_COMPILER_IS_MODERN)
   message (STATUS
      "C++11 enabled compiler: Your compiler is state-of-the-art"
      )
else ()
   message (STATUS
      "C++11 enabled compiler:"
      "Your compiler doesn't support the following features: ${CXX_FEATURES_UNSUPPORTED} but
      the list of the supported ones is sufficient for the build: ${CXX_FEATURES_SUPPORTED}"
      )
endif ()


# D-Bus activation service file, and associated systemd service
ecm_generate_dbus_service_file(
   NAME org.kde.ActivityManager
   EXECUTABLE "${KDE_INSTALL_FULL_LIBEXECDIR}/kactivitymanagerd"
   SYSTEMD_SERVICE plasma-kactivitymanagerd.service
   DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR}
)
ecm_install_configured_files(
   INPUT plasma-kactivitymanagerd.service.in
   DESTINATION ${KDE_INSTALL_SYSTEMDUNITDIR}
)

ecm_qt_declare_logging_category(debug_SRCS
   HEADER DebugActivities.h
   IDENTIFIER KAMD_LOG_ACTIVITIES
   CATEGORY_NAME org.kde.kactivities.activities
   DEFAULT_SEVERITY Warning)
ecm_qt_declare_logging_category(debug_SRCS
   HEADER DebugResources.h
   IDENTIFIER KAMD_LOG_RESOURCES
   CATEGORY_NAME org.kde.kactivities.resources
   DEFAULT_SEVERITY Warning)
ecm_qt_declare_logging_category(debug_SRCS
   HEADER DebugApplication.h
   IDENTIFIER KAMD_LOG_APPLICATION
   CATEGORY_NAME org.kde.kactivities.application
   DEFAULT_SEVERITY Warning)

ecm_setup_qtplugin_macro_names(JSON_ARG3 "KAMD_EXPORT_PLUGIN")

add_subdirectory (service)

