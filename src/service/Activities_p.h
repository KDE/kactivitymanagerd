/*
 *   SPDX-FileCopyrightText: 2010-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef ACTIVITIES_P_H
#define ACTIVITIES_P_H

// Self
#include "Activities.h"

// Qt
#include <QReadWriteLock>
#include <QString>
#include <QTimer>

// KDE
#include <kconfig.h>
#include <kconfiggroup.h>

class KSMServer;

class Activities::Private : public QObject
{
    Q_OBJECT

public:
    Private(Activities *parent);
    ~Private() override;

    // Loads the last activity
    // the user has used
    void loadLastActivity();

    // If the current activity is not running,
    // make some other activity current
    void ensureCurrentActivityIsRunning();

public Q_SLOTS:
    bool setCurrentActivity(const QString &activity);
    bool previousActivity();
    bool nextActivity();
    void updateSortedActivityList();

public:
    void setActivityState(const QString &activity, Activities::State state);

    // Configuration
    class KDE4ConfigurationTransitionChecker
    {
    public:
        KDE4ConfigurationTransitionChecker();
    } kde4ConfigurationTransitionChecker;
    QTimer configSyncTimer;
    KConfig config;

    // Interface to the session management
    KSMServer *ksmserver;

    QHash<QString, Activities::State> activities;
    QVector<ActivityInfo> sortedActivities;
    QReadWriteLock activitiesLock;
    QString currentActivity;

public:
    inline KConfigGroup activityNameConfig()
    {
        return KConfigGroup(&config, "activities");
    }

    inline KConfigGroup activityDescriptionConfig()
    {
        return KConfigGroup(&config, "activities-descriptions");
    }

    inline KConfigGroup activityIconConfig()
    {
        return KConfigGroup(&config, "activities-icons");
    }

    inline KConfigGroup mainConfig()
    {
        return KConfigGroup(&config, "main");
    }

    inline QString activityName(const QString &activity)
    {
        return activityNameConfig().readEntry(activity, QString());
    }

    inline QString activityDescription(const QString &activity)
    {
        return activityDescriptionConfig().readEntry(activity, QString());
    }

    inline QString activityIcon(const QString &activity)
    {
        return activityIconConfig().readEntry(activity, QString());
    }

public Q_SLOTS:
    // Schedules config syncing to be done after
    // a predefined time interval
    void scheduleConfigSync();

    // Immediately syncs the configuration file
    void configSync();

    QString addActivity(const QString &name);
    void removeActivity(const QString &activity);
    void activitySessionStateChanged(const QString &activity, int state);

private:
    Activities *const q;
};

#endif // ACTIVITIES_P_H
