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

#ifndef KSMSERVER_H
#define KSMSERVER_H

// Qt
#include <QObject>

// Utils
#include <utils/d_ptr.h>

/**
 * KSMServer
 */
class KSMServer : public QObject {
    Q_OBJECT
public:

    enum ReturnStatus {
        Started = 0,
        Stopped = 1,
        FailedToStop = 2
    };

    explicit KSMServer(QObject *parent = nullptr);
    virtual ~KSMServer();

public Q_SLOTS:
    void startActivitySession(const QString &activity);
    void stopActivitySession(const QString &activity);

Q_SIGNALS:
    void activitySessionStateChanged(const QString &activity, int status);

private:
    D_PTR;
};

#endif // KSMSERVER_H
