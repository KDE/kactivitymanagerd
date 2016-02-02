/*
 *   Copyright (C) 2011 - 2016 by Ivan Cukic <ivan.cukic@kde.org>
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

#ifndef EVENT_H
#define EVENT_H

#include "kactivitymanagerd_plugin_export.h"

// Qt
#include <QString>
#include <QDateTime>
#include <QMetaType>


/**
 *
 */
class KACTIVITYMANAGERD_PLUGIN_EXPORT Event {
public:
    enum Type {
        Accessed = 0, ///< resource was accessed, but we don't know for how long it will be open/used

        Opened = 1, ///< resource was opened
        Modified = 2, ///< previously opened resource was modified
        Closed = 3, ///< previously opened resource was closed

        FocussedIn = 4, ///< resource get the keyboard focus
        FocussedOut = 5, ///< resource lost the focus

        LastEventType = 5,
        UserEventType = 32
    };

    // These events can't come outside of the activity manager daemon,
    // they are intended to provide some additional functionality
    // to the daemon plugins
    enum UserType {
        UpdateScore = UserEventType + 1
    };

    Event();

    explicit Event(const QString &application, quintptr wid, const QString &uri,
                   int type = Accessed);

    Event deriveWithType(Type type) const;

    bool operator==(const Event &other) const;

public:
    QString application;
    quintptr wid;
    QString uri;
    int type;
    QDateTime timestamp;

    QString typeName() const;
};

QDebug operator<<(QDebug dbg, const Event &e);

typedef QList<Event> EventList;

Q_DECLARE_METATYPE(Event)
Q_DECLARE_METATYPE(EventList)

#endif // EVENT_H
