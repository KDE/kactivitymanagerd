/*
 *   SPDX-FileCopyrightText: 2010-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

// Self
#include "Resources.h"
#include "Resources_p.h"

// Qt
#include <QDBusConnection>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>

// Utils
#include <utils/d_ptr_implementation.h>
#include <utils/remove_if.h>

// Local
#include "common/dbus/common.h"
#include "resourcesadaptor.h"

Resources::Private::Private(Resources *parent)
    : QThread(parent)
    , q(parent)
{
}

Resources::Private::~Private()
{
    requestInterruption();
    wait(1500); // Enough time for the sleep(1) + processing in run()
}

namespace
{
EventList events;
QMutex events_mutex;
}

void Resources::Private::run()
{
    while (!isInterruptionRequested()) {
        // initial delay before processing the events
        sleep(1);

        EventList currentEvents;

        {
            QMutexLocker locker(&events_mutex);

            if (events.count() == 0) {
                return;
            }

            std::swap(currentEvents, events);
        }

        Q_EMIT q->ProcessedResourceEvents(currentEvents);
    }
}

void Resources::Private::insertEvent(const Event &newEvent)
{
    {
        QMutexLocker locker(&events_mutex);
        events << newEvent;
    }

    Q_EMIT q->RegisteredResourceEvent(newEvent);
}

void Resources::Private::addEvent(const Event &newEvent)
{
    // And now, for something completely delayed
    {
        QMutexLocker locker(&events_mutex);

        // Deleting previously registered Accessed events if
        // the current one has the same application and uri
        if (newEvent.type != Event::Accessed) {
            kamd::utils::remove_if(events, [&newEvent](const Event &event) -> bool {
                return event.application == newEvent.application && event.uri == newEvent.uri;
            });
        }
    }

    // Process the windowing
    insertEvent(newEvent);

    start();
}

Resources::Resources(QObject *parent)
    : Module(QStringLiteral("resources"), parent)
    , d(this)
{
    qRegisterMetaType<Event>("Event");
    qRegisterMetaType<EventList>("EventList");

    new ResourcesAdaptor(this);
    QDBusConnection::sessionBus().registerObject(KAMD_DBUS_OBJECT_PATH("Resources"), this);
}

Resources::~Resources()
{
}

namespace
{
QString sanitizeUri(const QString &uri)
{
    auto _uri = uri;
    if (uri.startsWith("file://")) {
        _uri = _uri.mid(QStringLiteral("file:/").length());
    }
    if (uri.startsWith("//")) {
        _uri = _uri.mid(QStringLiteral("/").length());
    }
    return _uri;
}
};

void Resources::RegisterResourceEvent(const QString &application, uint _windowId, const QString &uri, uint event)
{
    // TODO KF7 remove _windowId
    Q_UNUSED(_windowId)

    if (event > Event::LastEventType || uri.isEmpty() || application.isEmpty()) {
        return;
    }

    d->addEvent(Event{application, sanitizeUri(uri), (Event::Type)event});
}

void Resources::RegisterResourceMimetype(const QString &uri, const QString &mimetype)
{
    if (!mimetype.isEmpty()) {
        return;
    }

    Q_EMIT RegisteredResourceMimetype(sanitizeUri(uri), mimetype);
}

void Resources::RegisterResourceTitle(const QString &uri, const QString &title)
{
    // A dirty sanity check for the title
    if (title.length() < 3) {
        return;
    }

    Q_EMIT RegisteredResourceTitle(sanitizeUri(uri), title);
}

#include "moc_Resources_p.cpp"

#include "moc_Resources.cpp"
