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

#ifndef KAMD_ACTIVITIES_DBUS_H
#define KAMD_ACTIVITIES_DBUS_H

#include <QString>
#include <QList>
#include <QDBusArgument>
#include <QDebug>

struct ActivityInfo {
    QString id;
    QString name;
    QString description;
    QString icon;
    int state;

    ActivityInfo(const QString &id = QString(),
                 const QString &name = QString(),
                 const QString &description = QString(),
                 const QString &icon = QString(),
                 int state = 0)
        : id(id)
        , name(name)
        , description(description)
        , icon(icon)
        , state(state)
    {
    }

    bool operator<(const ActivityInfo &other) const
    {
        return id < other.id;
    }

    bool operator==(const ActivityInfo &other) const
    {
        return id == other.id;
    }
};

typedef QList<ActivityInfo> ActivityInfoList;

Q_DECLARE_METATYPE(ActivityInfo)
Q_DECLARE_METATYPE(ActivityInfoList)

QDBusArgument &operator<<(QDBusArgument &arg, const ActivityInfo);
const QDBusArgument &operator>>(const QDBusArgument &arg, ActivityInfo &rec);

QDebug operator<<(QDebug dbg, const ActivityInfo &r);

#endif // KAMD_ACTIVITIES_DBUS_H
