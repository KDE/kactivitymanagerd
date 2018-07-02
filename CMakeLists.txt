# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

cmake_minimum_required (VERSION 3.0)

project (KActivities)

option (KACTIVITIES_LIBRARY_ONLY "If true, compiles only the KActivities library, without the service and other modules." OFF)
option (KACTIVITIES_ENABLE_EXCEPTIONS "If you have Boost 1.53, you need to build KActivities with exceptions enabled. This is UNTESTED and EXPERIMENTAL!" OFF)

set(QT_MIN_VERSION "5.9.0")
set(KF5_MIN_VERSION "5.42.0")

# We don't build in-source
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   message (
      FATAL_ERROR
      "kactivities require an out of source build. Please create a separate build directory and run 'cmake path_to_plasma [options]' there."
   )
endif ()

set (KACTIVITIES_CURRENT_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Extra CMake stuff
include(FeatureSummary)
find_package(ECM ${KF5_MIN_VERSION} NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include (KDEInstallDirs)
include (KDECMakeSettings)
include (KDECompilerSettings NO_POLICY_SCOPE)
include (GenerateExportHeader)
include (ECMGenerateHeaders)
include (ECMQtDeclareLoggingCategory)

# Qt
set (CMAKE_AUTOMOC ON)
find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core DBus Widgets)

# KDE Frameworks
find_package (KF5DBusAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)

# Basic includes
include (CPack)

# Adding local CMake modules
set (
   CMAKE_MODULE_PATH
   ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
   ${CMAKE_MODULE_PATH}
   )

add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions (-DTRANSLATION_DOMAIN=\"kactivities5\")
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ki18n_install (po)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   set(CMAKE_CXX_VISIBILITY_PRESET default)
   set(CMAKE_VISIBILITY_INLINES_HIDDEN 0)
endif ()

add_subdirectory (src)

# Write out the features
feature_summary (WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

