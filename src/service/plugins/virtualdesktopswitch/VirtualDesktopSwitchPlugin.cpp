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

#include "VirtualDesktopSwitchPlugin.h"

#include <QString>
#include <Debug.h>

#include <kwindowsystem.h>

KAMD_EXPORT_PLUGIN(virtualdesktopswitchplugin, VirtualDesktopSwitchPlugin, "kactivitymanagerd-plugin-virtualdesktopswitch.json")

const auto configPattern = QStringLiteral("desktop-for-%1");

VirtualDesktopSwitchPlugin::VirtualDesktopSwitchPlugin(QObject *parent, const QVariantList &args)
    : Plugin(parent)
    , m_activitiesService(Q_NULLPTR)
{
    Q_UNUSED(args);

    setName("org.kde.ActivityManager.VirtualDesktopSwitch");
}

VirtualDesktopSwitchPlugin::~VirtualDesktopSwitchPlugin()
{
}

bool VirtualDesktopSwitchPlugin::init(QHash<QString, QObject *> &modules)
{
    Plugin::init(modules);

    m_activitiesService = modules["activities"];

    m_currentActivity = Plugin::retrieve<QString>(
        m_activitiesService, "CurrentActivity", "QString");

    connect(m_activitiesService, SIGNAL(CurrentActivityChanged(QString)),
            this, SLOT(currentActivityChanged(QString)));
    connect(m_activitiesService, SIGNAL(ActivityRemoved(QString)),
            this, SLOT(activityRemoved(QString)));

    return true;
}

void VirtualDesktopSwitchPlugin::currentActivityChanged(const QString &activity)
{
    if (m_currentActivity == activity) {
        return;
    }

    config().writeEntry(
        configPattern.arg(m_currentActivity),
        QString::number(KWindowSystem::currentDesktop()));

    m_currentActivity = activity;

    const auto desktopId = config().readEntry(configPattern.arg(m_currentActivity), -1);

    if (desktopId <= KWindowSystem::numberOfDesktops() && desktopId >= 0) {
        KWindowSystem::setCurrentDesktop(desktopId);
    }
}

void VirtualDesktopSwitchPlugin::activityRemoved(const QString &activity)
{
    config().deleteEntry(configPattern.arg(activity));
    config().sync();
}

#include "VirtualDesktopSwitchPlugin.moc"

