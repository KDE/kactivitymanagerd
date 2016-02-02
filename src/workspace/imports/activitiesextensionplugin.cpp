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

#include "activitiesextensionplugin.h"

#include <QtQml>

#include "activitymodel.h"
#include "activityinfo.h"
#include "resourcemodel.h"
#include "resourceinstance.h"

#include <QDebug>

ActivitiesExtensionPlugin::ActivitiesExtensionPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{
}

void ActivitiesExtensionPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("org.kde.activities"));

    qmlRegisterType<KActivities::Imports::ActivityModel>(uri, 0, 1, "ActivityModel");
    qmlRegisterType<KActivities::Imports::ActivityInfo>(uri, 0, 1, "ActivityInfo");
    qmlRegisterType<KActivities::Imports::ResourceModel>(uri, 0, 1, "ResourceModel");
    qmlRegisterType<KActivities::Imports::ResourceInstance>(uri, 0, 1, "ResourceInstance");
}

