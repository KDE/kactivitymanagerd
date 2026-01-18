/*
 *   SPDX-FileCopyrightText: 2010-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

// Self
#include "Activities.h"

// Qt
#include <QReadWriteLock>
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

public Q_SLOTS:
    bool setCurrentActivity(const QString &activity);
    bool previousActivity();
    bool nextActivity();
    void updateSortedActivityList();

public:
    QList<ActivityInfo *> createSortedActivitiesByOrder();
    void writeActivitiesSortOrder(const QList<ActivityInfo *> sortedActivitiesByOrder);
    void moveActivityInSortOrder(const QString &activity, int newPosition);
    void reorderActivitiesByName();

    QTimer configSyncTimer;
    KConfig config;
    KConfig stateConfig;

    // Interface to the session management
    KSMServer *ksmserver;

    QSet<QString> activities;
    QList<ActivityInfo> sortedActivities;
    QReadWriteLock activitiesLock;
    QString currentActivity;

public:
    inline KConfigGroup activityNameConfig()
    {
        return KConfigGroup(&config, QStringLiteral("activities"));
    }

    inline KConfigGroup activityDescriptionConfig()
    {
        return KConfigGroup(&config, QStringLiteral("activities-descriptions"));
    }

    inline KConfigGroup activityIconConfig()
    {
        return KConfigGroup(&config, QStringLiteral("activities-icons"));
    }

    inline KConfigGroup activitySortOrderConfig()
    {
        return KConfigGroup(&config, QStringLiteral("activities-order"));
    }

    inline KConfigGroup mainConfig()
    {
        return KConfigGroup(&config, QStringLiteral("main"));
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

    inline int activitySortOrder(const QString &activity)
    {
        return activitySortOrderConfig().readEntry(activity, -1);
    }

public Q_SLOTS:
    // Schedules config syncing to be done after
    // a predefined time interval
    void scheduleConfigSync();

    // Immediately syncs the configuration file
    void configSync();

    QString addActivity(const QString &name);
    void removeActivity(const QString &activity);

private:
    Activities *const q;
};
