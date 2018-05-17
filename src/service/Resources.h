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

#ifndef RESOURCES_H
#define RESOURCES_H

// Qt
#include <QString>
#include <QStringList>

// Utils
#include <utils/d_ptr.h>

// Local
#include "Module.h"
#include "Event.h"


/**
 * Resources
 */
class Resources : public Module {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.ActivityManager.Resources")

public:
    Resources(QObject *parent = Q_NULLPTR);
    virtual ~Resources();

public Q_SLOTS:
    /**
     * Registers a new event
     * @param application the name of application that sent the event. Ignored
     *                    if the event is not of type Opened
     * @param windowId ID of the window that displays the resource. Ignored if
     *                 the event is of type Accessed
     * @param uri URI of the resource on which the event happened
     * @param event type of the event
     */
    void RegisterResourceEvent(const QString &application, uint windowId,
                               const QString &uri, uint event);

    /**
     * Registers resource's mimetype.
     * Note that this will be forgotten when the resource in question is closed.
     * @param uri URI of the resource
     */
    void RegisterResourceMimetype(const QString &uri, const QString &mimetype);

    /**
     * Registers resource's title. If not manually specified, it will be a
     * shortened version of the uri
     *
     * Note that this will be forgotten when the resource in question is closed.
     * @param uri URI of the resource
     */
    void RegisterResourceTitle(const QString &uri, const QString &title);

Q_SIGNALS:
    void RegisteredResourceEvent(const Event &event);
    void ProcessedResourceEvents(const EventList &events);
    void RegisteredResourceMimetype(const QString &uri, const QString &mimetype);
    void RegisteredResourceTitle(const QString &uri, const QString &title);

private:
    D_PTR;
};

#endif // RESOURCES_H
