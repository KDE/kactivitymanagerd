/*
 *   Copyright (C) 2010 - 2016 by Ivan Cukic <ivan.cukic@kde.org>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License or (at your option) version 3 or any later version
 *   accepted by the membership of KDE e.V. (or its successor approved
 *   by the membership of KDE e.V.), which shall act as a proxy
 *   defined in Section 14 of version 3 of the license.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ACTIVITIES_P_H
#define ACTIVITIES_P_H

// Self
#include "Activities.h"

// Qt
#include <QString>
#include <QTimer>
#include <QReadWriteLock>

// KDE
#include <kconfig.h>
#include <kconfiggroup.h>


class KSMServer;


class Activities::Private : public QObject {
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

public:
    void setActivityState(const QString &activity, Activities::State state);

    // Configuration
    class KDE4ConfigurationTransitionChecker {
    public:
        KDE4ConfigurationTransitionChecker();
    } kde4ConfigurationTransitionChecker;
    QTimer configSyncTimer;
    KConfig config;

    // Interface to the session management
    KSMServer *ksmserver;

    QHash<QString, Activities::State> activities;
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
