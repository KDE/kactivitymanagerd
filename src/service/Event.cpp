/*
 *   SPDX-FileCopyrightText: 2011-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

// Self
#include "Event.h"

// Local
#include <QDebug>

Event::Event()
    : type(Accessed)
    , timestamp(QDateTime::currentDateTime())
{
}

Event::Event(const QString &vApplication, const QString &vUri, int vType)
    : application(vApplication)
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
    return application == other.application && uri == other.uri && type == other.type && timestamp == other.timestamp;
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
    default:
        return QStringLiteral("Other");
    }
}

QDebug operator<<(QDebug dbg, const Event &e)
{
#ifndef QT_NO_DEBUG_OUTPUT
    dbg << "Event(" << e.application << e.typeName() << e.uri << ":" << e.timestamp << ")";
#else
    Q_UNUSED(e);
#endif
    return dbg.space();
}
