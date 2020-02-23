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

// Self
#include "KSMServer.h"
#include "KSMServer_p.h"

// Qt
#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QDBusInterface>
#include <QDBusConnectionInterface>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>

// Utils
#include <utils/d_ptr_implementation.h>

// Local
#include "DebugActivities.h"

#define KWIN_SERVICE QStringLiteral("org.kde.KWin")

KSMServer::Private::Private(KSMServer *parent)
    : kwin(new QDBusInterface(KWIN_SERVICE, QStringLiteral("/KWin"), QStringLiteral("org.kde.KWin"), QDBusConnection::sessionBus(), this))
    , processing(false)
    , q(parent)
{
}

KSMServer::KSMServer(QObject *parent)
    : QObject(parent)
    , d(this)
{
}

KSMServer::~KSMServer()
{
}

void KSMServer::startActivitySession(const QString &activity)
{
    d->processLater(activity, true);
}

void KSMServer::stopActivitySession(const QString &activity)
{
    d->processLater(activity, false);
}

void KSMServer::Private::processLater(const QString &activity, bool start)
{
    if (kwin->isValid()) {
        for (const auto &item: queue) {
            if (item.first == activity) {
                return;
            }
        }

        queue << qMakePair(activity, start);

        if (!processing) {
            processing = true;
            QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection);
        }
    } else {
        // We don't have kwin. No way to invoke the session stuff
        subSessionSendEvent(start ? KSMServer::Started
                                  : KSMServer::Stopped);
    }
}

void KSMServer::Private::process()
{
    // If the queue is empty, we have nothing more to do
    if (queue.isEmpty()) {
        processing = false;
        return;
    }

    const auto item = queue.takeFirst();
    processingActivity = item.first;

    makeRunning(item.second);

    // Calling process again for the rest of the list
    QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection);
}

void KSMServer::Private::makeRunning(bool value)
{
    if (!kwin->isValid()) {
        qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: No kwin, marking activity as: " << value;
        subSessionSendEvent(value ? KSMServer::Started : KSMServer::Stopped);
        return;
    }

    const auto call = kwin->asyncCall(
        value ? QLatin1String("startActivity") : QLatin1String("stopActivity"),
        processingActivity);

    const auto watcher = new QDBusPendingCallWatcher(call, this);

    qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: Telling kwin to start/stop activity : " << processingActivity << value;
    QObject::connect(
        watcher, SIGNAL(finished(QDBusPendingCallWatcher *)),
        this,
        value
            ? SLOT(startCallFinished(QDBusPendingCallWatcher *))
            : SLOT(stopCallFinished(QDBusPendingCallWatcher *)));
}

void KSMServer::Private::startCallFinished(QDBusPendingCallWatcher *call)
{
    qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: Start call is finished";
    QDBusPendingReply<bool> reply = *call;

    if (reply.isError()) {
        qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: Error in getting a reply for start, marking as started";
        subSessionSendEvent(KSMServer::Started);

    } else {
        // If we got false, it means something is going on with ksmserver
        // and it didn't start our activity
        const auto retval = reply.argumentAt<0>();

        if (!retval) {
            qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: Error starting, marking as stopped";
            subSessionSendEvent(KSMServer::Stopped);
        } else {
            qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: All OK starting, marking as starting";
            subSessionSendEvent(KSMServer::Started);
        }
    }

    call->deleteLater();
}

void KSMServer::Private::stopCallFinished(QDBusPendingCallWatcher *call)
{
    qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: Stop call is finished";
    QDBusPendingReply<bool> reply = *call;

    if (reply.isError()) {
        qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: Error in getting a reply for stop, marking as stopped";
        subSessionSendEvent(KSMServer::Stopped);

    } else {
        // If we got false, it means something is going on with ksmserver
        // and it didn't stop our activity
        const auto retval = reply.argumentAt<0>();

        if (!retval) {
            qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: Error stopping, marking as started";
            subSessionSendEvent(KSMServer::FailedToStop);
        } else {
            qCDebug(KAMD_LOG_ACTIVITIES) << "Activities KSM: All OK stopping, marking as stopped";
            subSessionSendEvent(KSMServer::Stopped);
        }
    }

    call->deleteLater();
}

void KSMServer::Private::subSessionSendEvent(int event)
{
    if (processingActivity.isEmpty()) {
        return;
    }

    emit q->activitySessionStateChanged(processingActivity, event);

    processingActivity.clear();
}
