/*
 *   SPDX-FileCopyrightText: 2010-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

// Qt
#include <QStringList>

// Utils
#include <utils/d_ptr.h>

// Local
#include "Module.h"
#include <common/dbus/org.kde.ActivityManager.Activities.h>

/**
 * Service for tracking the user actions and managing the
 * activities
 */
class Activities : public Module
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.ActivityManager.Activities")
    Q_PROPERTY(QString CurrentActivity READ CurrentActivity WRITE SetCurrentActivity NOTIFY CurrentActivityChanged)

public:
    /**
     * Creates new Activities object
     */
    explicit Activities(QObject *parent = nullptr);

    /**
     * Destroys this interface
     */
    ~Activities() override;

    // workspace activities control
public Q_SLOTS:
    /**
     * @returns the id of the current activity, empty string if none
     */
    QString CurrentActivity() const;

    /**
     * @returns the list of all existing activities
     */
    QStringList ListActivities() const;

    /**
     * @returns the name of the specified activity
     * @param activity id of the activity
     */
    QString ActivityName(const QString &activity) const;

    /**
     * @returns the description of the specified activity
     * @param activity id of the activity
     */
    QString ActivityDescription(const QString &activity) const;

    /**
     * @returns the icon of the specified activity
     * @param activity id of the activity
     */
    QString ActivityIcon(const QString &activity) const;

    /**
     * @returns a list of activities with basic info about them
     */
    ActivityInfoList ListActivitiesWithInformation() const;

    /**
     * @returns the info about an activity
     */
    ActivityInfo ActivityInformation(const QString &activity) const;

    /**
     * Sets the current activity
     * @param activity id of the activity to make current
     */
    bool SetCurrentActivity(const QString &activity);

    /**
     * Switches to the previous activity
     */
    bool PreviousActivity();

    /**
     * Switches to the next activity
     */
    bool NextActivity();

    /**
     * Adds a new activity
     * @param name name of the activity
     * @returns id of the newly created activity
     */
    QString AddActivity(const QString &name);

    /**
     * Removes the specified activity
     * @param activity id of the activity to delete
     */
    void RemoveActivity(const QString &activity);

    /**
     * Sets the name of the specified activity
     * @param activity id of the activity
     * @param name name to be set
     */
    void SetActivityName(const QString &activity, const QString &name);

    /**
     * Sets the description of the specified activity
     * @param activity id of the activity
     * @param description description to be set
     */
    void SetActivityDescription(const QString &activity, const QString &description);

    /**
     * Sets the icon of the specified activity
     * @param activity id of the activity
     * @param icon icon to be set
     */
    void SetActivityIcon(const QString &activity, const QString &icon);

Q_SIGNALS:
    /**
     * This signal is emitted when the global
     * activity is changed
     * @param activity id of the new current activity
     */
    void CurrentActivityChanged(const QString &activity);

    /**
     * This signal is emitted when a new activity is created
     * @param activity id of the activity
     */
    void ActivityAdded(const QString &activity);

    /**
     * This signal is emitted when an activity is deleted
     * @param activity id of the activity
     */
    void ActivityRemoved(const QString &activity);

    /**
     * Emitted when an activity name is changed
     * @param activity id of the changed activity
     * @param name name of the changed activity
     */
    void ActivityNameChanged(const QString &activity, const QString &name);

    /**
     * Emitted when an activity description is changed
     * @param activity id of the changed activity
     * @param description description of the changed activity
     */
    void ActivityDescriptionChanged(const QString &activity, const QString &description);

    /**
     * Emitted when an activity icon is changed
     * @param activity id of the changed activity
     * @param icon name of the changed activity
     */
    void ActivityIconChanged(const QString &activity, const QString &icon);

    /**
     * Emitted when an activity is changed (name, icon, or some other property)
     * @param activity id of the changed activity
     */
    void ActivityChanged(const QString &activity);

private:
    D_PTR;
};
