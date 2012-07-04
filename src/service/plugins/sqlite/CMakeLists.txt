project(activitymanager-plugin-sqlite)

# Checking for Nepomuk
# find_package(Nepomuk REQUIRED)

set(
   sqliteplugin_SRCS
   DatabaseConnection.cpp
   StatsPlugin.cpp
   ResourceScoreCache.cpp
   ResourceScoreMaintainer.cpp
   Rankings.cpp
   NepomukCommon.cpp
   ../../Plugin.cpp
   )

qt4_add_dbus_adaptor(
   sqliteplugin_SRCS
   org.kde.ActivityManager.Rankings.xml
   Rankings.h Rankings
   )

if(Nepomuk_FOUND)
    include_directories(${NEPOMUK_INCLUDE_DIR} ${SOPRANO_INCLUDE_DIR})

    soprano_add_ontology(sqliteplugin_SRCS
        ${CMAKE_SOURCE_DIR}/src/ontologies/kao.trig
        "KAO" "Nepomuk::Vocabulary" "trig"
        )
endif(Nepomuk_FOUND)

include_directories(
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_BINARY_DIR}/src
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../..
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}/../..
   ${KDE4_INCLUDES}
   )

kde4_add_plugin(
   activitymanager_plugin_sqlite
   ${sqliteplugin_SRCS}
   )

target_link_libraries(
   activitymanager_plugin_sqlite
   ${QT_QTSQL_LIBRARIES}
   ${KDE4_KDECORE_LIBS}
   ${KDE4_KDEUI_LIBS}
   ${NEPOMUK_LIBRARIES}
   ${NEPOMUK_QUERY_LIBRARIES}
   ${SOPRANO_LIBRARIES}
   )

install(TARGETS activitymanager_plugin_sqlite DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES activitymanager-plugin-sqlite.desktop DESTINATION ${SERVICES_INSTALL_DIR})
