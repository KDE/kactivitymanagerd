# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

cmake_minimum_required(VERSION 3.16)

project (KActivityManagerd)

set (QT_MIN_VERSION "5.15.0")
set (KF5_MIN_VERSION "5.86")
set (KDE_COMPILERSETTINGS_LEVEL "5.82")
set (Boost_MINIMUM_VERSION "1.58")

# We don't build in-source
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   message (
      FATAL_ERROR
      "kactivities require an out of source build. Please create a separate build directory and run 'cmake path_to_plasma [options]' there."
   )
endif ()

set (KACTIVITIES_CURRENT_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Extra CMake stuff
include (FeatureSummary)
find_package(ECM ${KF5_MIN_VERSION} NO_MODULE)
set_package_properties(ECM PROPERTIES
    DESCRIPTION "Extra CMake Modules."
    URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules"
    TYPE REQUIRED
)
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include (KDEInstallDirs)
include (KDECMakeSettings)
include (KDECompilerSettings NO_POLICY_SCOPE)
include (GenerateExportHeader)
include (ECMConfiguredInstall)
include (ECMGenerateDBusServiceFile)
include (ECMGenerateHeaders)
include (ECMQtDeclareLoggingCategory)
include (ECMSetupQtPluginMacroNames)
include (KDEClangFormat)
include (KDEGitCommitHooks)

# C++17
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core DBus Widgets)

# KDE Frameworks
find_package (KF5DBusAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package (KF5Crash ${KF5_MIN_VERSION} CONFIG REQUIRED)

# Basic includes
include (CPack)

# Adding local CMake modules
set (
   CMAKE_MODULE_PATH
   ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
   ${CMAKE_MODULE_PATH}
   )

add_definitions(
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_SIGNALS_SLOTS_KEYWORDS
)
add_definitions (-DTRANSLATION_DOMAIN=\"kactivities5\")
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ki18n_install (po)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   set (CMAKE_CXX_VISIBILITY_PRESET default)
   set (CMAKE_VISIBILITY_INLINES_HIDDEN 0)
endif ()

add_subdirectory (src)

# add clang-format target for all our real source files
file (GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

ecm_qt_install_logging_categories(
        EXPORT KACTIVITYMANAGERD
        FILE kactivitymanagerd.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )

# Write out the features
feature_summary (WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
