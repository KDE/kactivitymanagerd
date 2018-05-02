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

// Self
#include "Event.h"

// Local
#include <QDebug>


Event::Event()
    : wid(0)
    , type(Accessed)
    , timestamp(QDateTime::currentDateTime())
{
}

Event::Event(const QString &vApplication, quintptr vWid, const QString &vUri, int vType)
    : application(vApplication)
    , wid(vWid)
    , uri(vUri)
    , type(vType)
    , timestamp(QDateTime::currentDateTime())
{
    Q_ASSERT(!vApplication.isEmpty());
    Q_ASSERT(!vUri.isEmpty());
}

Event Event::deriveWithType(Type type) const
{
    Event result(*this);
    result.type = type;
    return result;
}

bool Event::operator==(const Event &other) const
{
    return application == other.application && wid == other.wid && uri == other.uri && type == other.type && timestamp == other.timestamp;
}

QString Event::typeName() const
{
    switch (type) {
        case Accessed:
            return QStringLiteral("Accessed");
        case Opened:
            return QStringLiteral("Opened");
        case Modified:
            return QStringLiteral("Modified");
        case Closed:
            return QStringLiteral("Closed");
        case FocussedIn:
            return QStringLiteral("FocussedIn");
        case FocussedOut:
            return QStringLiteral("FocussedOut");
        default:
            return QStringLiteral("Other");
    }
}

QDebug operator<<(QDebug dbg, const Event &e)
{
#ifndef QT_NO_DEBUG_OUTPUT
    dbg << "Event(" << e.application << e.wid << e.typeName() << e.uri << ":" << e.timestamp << ")";
#else
    Q_UNUSED(e);
#endif
    return dbg.space();
}
