
# A library containing the manager and helper tools for testing
add_library(DaemonTestFramework SHARED)
target_link_libraries(DaemonTestFramework PUBLIC kactivitymanagerdlib)
target_sources(DaemonTestFramework PUBLIC testhelper.cpp )
target_include_directories(DaemonTestFramework PUBLIC ${PROJECT_SOURCE_DIR}/src/utils)

# Set up dbus adaptors for DaemonTestFramework
qt_add_dbus_adaptor (
   DaemonTestFramework
   ../common/dbus/org.kde.ActivityManager.Activities.xml
   testhelper.h ActivitiesDBus
   )

function(integrationTest NAME SRC)

    add_executable(${NAME} ${SRC})
    target_link_libraries(${NAME} Qt${QT_MAJOR_VERSION}::Test DaemonTestFramework)

    add_test(NAME ${NAME} COMMAND dbus-run-session ${CMAKE_BINARY_DIR}/bin/${NAME})

endfunction()

# tests
integrationTest(testActivitiesDBus test_activities_dbus.cpp)
