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

#ifndef RESOURCES_P_H
#define RESOURCES_P_H

// Self
#include "Resources.h"

// Qt
#include <QString>
#include <QList>
#include <QWindow> // for WId

// Local
#include "resourcesadaptor.h"


class Resources::Private : public QThread {
    Q_OBJECT

public:
    Private(Resources *parent);
    ~Private() override;

    void run() override;

    // Inserts the event directly into the queue
    void insertEvent(const Event &newEvent);

    // Processes the event and inserts it into the queue
    void addEvent(const QString &application, WId wid, const QString &uri,
                  int type);

    // Processes the event and inserts it into the queue
    void addEvent(const Event &newEvent);

    QStringList resourcesLinkedToActivity(const QString &activity) const;

public Q_SLOTS:
    // Reacting to window manager signals
    void windowClosed(WId windowId);

    void activeWindowChanged(WId windowId);

private:
    struct WindowData {
        QSet<QString> resources;
        QString focussedResource;
        QString application;
    };

    Event lastEvent;

    QHash<WId, WindowData> windows;
    WId focussedWindow;

    Resources *const q;
};

#endif // RESOURCES_P_H
