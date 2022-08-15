
#pragma once

#include <QObject>

#include "Application.h"

/**
 * This object creates an instance of the daemon that can be tested.
 */
class ManagerDaemonTestInstance : public Application
{
    Q_OBJECT
public:
    ManagerDaemonTestInstance(int argc = 0, char **argv = nullptr);
};

/**
 *
 */
class ActivitiesDBus : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.ActivityManager.Activities")
};
