#include "testhelper.h"

// kactivitymanagerdlib
#include "Application.h"
#include <kcrash.h>

ManagerDaemonTestInstance::ManagerDaemonTestInstance(int argc, char **argv)
    : Application(argc, argv)
{
    // Disable session management for this process
    qunsetenv("SESSION_MANAGER");

    QGuiApplication::setDesktopSettingsAware(false);

    setApplicationName(QStringLiteral("ActivityManager"));
    setOrganizationDomain(QStringLiteral("kde.org"));

    init();

    exec();
}
