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

#ifndef KSMSERVER_P_H
#define KSMSERVER_P_H

// Self
#include "KSMServer.h"

// Qt
#include <QPair>

// STD
#include <memory>

class QDBusServiceWatcher;
class QDBusInterface;
class QDBusPendingCallWatcher;

class KSMServer::Private : public QObject {
    Q_OBJECT

public:
    Private(KSMServer *parent);

    void processLater(const QString &activity, bool start);

private Q_SLOTS:
    void serviceOwnerChanged(const QString &service, const QString &oldOwner, const QString &newOwner);

    void process();
    void makeRunning(bool value);

    void startCallFinished(QDBusPendingCallWatcher *watcher);
    void stopCallFinished(QDBusPendingCallWatcher *watcher);

    void subSessionSendEvent(int event);

private:
    std::unique_ptr<QDBusServiceWatcher> serviceWatcher;
    QDBusInterface *kwin;

    bool processing;
    QString processingActivity;
    QList<QPair<QString, bool>> queue;

    KSMServer *const q;
};

#endif // KSMSERVER_P_H
