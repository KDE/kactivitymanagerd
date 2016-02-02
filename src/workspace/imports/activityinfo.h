/*
 *   Copyright (C) 2012 - 2016 by Ivan Cukic <ivan.cukic@kde.org>
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

#ifndef KACTIVITIES_IMPORTS_ACTIVITY_INFO_H
#define KACTIVITIES_IMPORTS_ACTIVITY_INFO_H

// Qt
#include <QObject>

// STL
#include <memory>

// Local
#include <KActivities/Controller>
#include <KActivities/Info>

namespace KActivities {
namespace Imports {

/**
 * ActivityInfo
 */

class ActivityInfo : public QObject {
    Q_OBJECT

    /**
     * Unique identifier of the activity
     */
    Q_PROPERTY(QString activityId READ activityId WRITE setActivityId NOTIFY activityIdChanged)

    /**
     * Name of the activity
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    /**
     * Name of the activity
     */
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

    /**
     * Activity icon
     */
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)

    /**
     * Is the activity a valid one - does it exist?
     */
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

public:
    ActivityInfo(QObject *parent = 0);
    virtual ~ActivityInfo();

public Q_SLOTS:
    void setActivityId(const QString &id);
    QString activityId() const;

    void setName(const QString &name);
    QString name() const;

    void setDescription(const QString &description);
    QString description() const;

    void setIcon(const QString &icon);
    QString icon() const;

    bool valid() const;

Q_SIGNALS:
    void activityIdChanged(const QString &id);
    void nameChanged(const QString &name);
    void descriptionChanged(const QString &description);
    void iconChanged(const QString &icon);
    void validChanged(bool valid);

private Q_SLOTS:
    void setCurrentActivity(const QString &id);

private:
    void setIdInternal(const QString &id);

    KActivities::Controller m_service;
    std::unique_ptr<KActivities::Info> m_info;
    bool m_showCurrentActivity;
};

} // namespace Imports
} // namespace KActivities

#endif // KACTIVITIES_IMPORTS_ACTIVITY_INFO_H

