/*
 *   Copyright (C) 2012 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

// Self
#include "KSMServer.h"
#include "KSMServer_p.h"

// Qt
#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>

// KDE
#include <kdbusconnectionpool.h>

// Utils
#include <utils/d_ptr_implementation.h>

// Local
#include <Debug.h>

#define KWIN_SERVICE QStringLiteral("org.kde.kwin")
#define KSMSERVER_SERVICE QStringLiteral("org.kde.ksmserver")

KSMServer::Private::Private(KSMServer *parent)
    : ksmServerFunctioning(false)
    , serviceWatcher(Q_NULLPTR)
    , kwin(Q_NULLPTR)
    , ksmserver(Q_NULLPTR)
    , processing(false)
    , q(parent)
{
    serviceWatcher = new QDBusServiceWatcher(this);

    serviceWatcher->setConnection(KDBusConnectionPool::threadConnection());
    serviceWatcher->addWatchedService(KWIN_SERVICE);
    serviceWatcher->addWatchedService(KSMSERVER_SERVICE);

    connect(serviceWatcher, SIGNAL(serviceOwnerChanged(QString, QString, QString)),
            this, SLOT(serviceOwnerChanged(QString, QString, QString)));

    serviceOwnerChanged(KWIN_SERVICE, QString(), QString());
    serviceOwnerChanged(KSMSERVER_SERVICE, QString(), QString());
}

template <typename Func>
static void initializeInterface(QDBusInterface *&service,
                                const QString &servicePath, const QString &path, const QString &object, Func init)
{
    // Delete the old object, just in case
    delete service;

    // Creating the new dbus interface
    service = new QDBusInterface(servicePath, path, object);

    // If the service is valid, initialize it
    // otherwise delete the object
    if (service->isValid()) {
        init(service);

    } else {
        delete service;
        service = Q_NULLPTR;
    }
}

void KSMServer::Private::serviceOwnerChanged(const QString &service,
                                             const QString &oldOwner, const QString &newOwner)
{
    Q_UNUSED(oldOwner);
    Q_UNUSED(newOwner);

    if (service == KSMSERVER_SERVICE) {
        qDebug() << "KSM owned by ksmserver";

        initializeInterface(
            ksmserver,
            KSMSERVER_SERVICE,
            QStringLiteral("/KSMServer"),
            QStringLiteral("org.kde.KSMServerInterface"),
            [this](QObject * service) {
                service->setParent(this);

                ksmServerFunctioning =
                    connect(service, SIGNAL(subSessionOpened()),
                        this, SLOT(subSessionOpened()))
                &&
                    connect(service, SIGNAL(subSessionClosed()),
                        this, SLOT(subSessionClosed()))
                &&
                    connect(service, SIGNAL(subSessionCloseCanceled()),
                        this, SLOT(subSessionCloseCanceled()));
            });

    } else if (service == KWIN_SERVICE) {
        qDebug() << "KSM owned by kwin";

        initializeInterface(
            kwin,
            KWIN_SERVICE,
            QStringLiteral("/KWin"),
            QStringLiteral("org.kde.KWin"),
            [this](QObject * service) {
                service->setParent(this);
                ksmServerFunctioning = true;
            });
    }
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
    if (!d->ksmServerFunctioning) {
        emit activitySessionStateChanged(activity, Started);
        return;
    }

    d->processLater(activity, true);
}

void KSMServer::stopActivitySession(const QString &activity)
{
    if (!d->ksmServerFunctioning) {
        emit activitySessionStateChanged(activity, Stopped);
        return;
    }

    d->processLater(activity, false);
}

void KSMServer::Private::processLater(const QString &activity, bool start)
{
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
    if (!kwin) {
        subSessionSendEvent(value ? KSMServer::Started : KSMServer::Stopped);
        return;
    }

    const auto call = kwin->asyncCall(
        value ? QLatin1String("startActivity") : QLatin1String("stopActivity"),
        processingActivity);

    const auto watcher = new QDBusPendingCallWatcher(call, this);

    QObject::connect(
        watcher, SIGNAL(finished(QDBusPendingCallWatcher *)),
        this,
        value
            ? SLOT(startCallFinished(QDBusPendingCallWatcher *))
            : SLOT(stopCallFinished(QDBusPendingCallWatcher *)));
}

void KSMServer::Private::startCallFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<bool> reply = *call;

    if (reply.isError()) {
        emit q->activitySessionStateChanged(processingActivity, KSMServer::Started);

    } else {
        // If we got false, it means something is going on with ksmserver
        // and it didn't start our activity
        const auto retval = reply.argumentAt<0>();

        if (!retval) {
            subSessionSendEvent(KSMServer::Stopped);
        }
    }

    call->deleteLater();
}

void KSMServer::Private::stopCallFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<bool> reply = *call;

    if (reply.isError()) {
        emit q->activitySessionStateChanged(processingActivity, KSMServer::Stopped);

    } else {
        // If we got false, it means something is going on with ksmserver
        // and it didn't stop our activity
        const auto retval = reply.argumentAt<0>();

        if (!retval) {
            subSessionSendEvent(KSMServer::FailedToStop);
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

void KSMServer::Private::subSessionOpened()
{
    subSessionSendEvent(KSMServer::Started);
}

void KSMServer::Private::subSessionClosed()
{
    subSessionSendEvent(KSMServer::Stopped);
}

void KSMServer::Private::subSessionCloseCanceled()
{
    subSessionSendEvent(KSMServer::FailedToStop);
}