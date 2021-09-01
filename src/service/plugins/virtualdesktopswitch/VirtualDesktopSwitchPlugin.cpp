/*
 *   SPDX-FileCopyrightText: 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "VirtualDesktopSwitchPlugin.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusReply>
#include <QString>

#include <kwindowsystem.h>

KAMD_EXPORT_PLUGIN(virtualdesktopswitchplugin, VirtualDesktopSwitchPlugin, "kactivitymanagerd-plugin-virtualdesktopswitch.json")

const auto configPattern = QStringLiteral("desktop-for-%1");
const auto kwinName = QStringLiteral("org.kde.KWin");
const auto kwinPath = QStringLiteral("/KWin");

VirtualDesktopSwitchPlugin::VirtualDesktopSwitchPlugin(QObject *parent, const QVariantList &args)
    : Plugin(parent)
    , m_activitiesService(nullptr)
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

    m_currentActivity = Plugin::retrieve<QString>(m_activitiesService, "CurrentActivity", "QString");

    connect(m_activitiesService, SIGNAL(CurrentActivityChanged(QString)), this, SLOT(currentActivityChanged(QString)));
    connect(m_activitiesService, SIGNAL(ActivityRemoved(QString)), this, SLOT(activityRemoved(QString)));

    return true;
}

void VirtualDesktopSwitchPlugin::currentActivityChanged(const QString &activity)
{
    if (m_currentActivity == activity) {
        return;
    }

    const auto desktopId = config().readEntry(configPattern.arg(activity), -1);

    if (KWindowSystem::isPlatformX11()) {
        config().writeEntry(configPattern.arg(m_currentActivity), QString::number(KWindowSystem::currentDesktop()));
        if (desktopId <= KWindowSystem::numberOfDesktops() && desktopId >= 0) {
            KWindowSystem::setCurrentDesktop(desktopId);
        }
    } else {
        auto currentMessage = QDBusMessage::createMethodCall(kwinName, kwinPath, kwinName, QStringLiteral("currentDesktop"));
        auto currentDesktopCall = QDBusConnection::sessionBus().asyncCall(currentMessage);
        if (desktopId >= 0) {
            auto setDesktopMessage = QDBusMessage::createMethodCall(kwinName, kwinPath, kwinName, QStringLiteral("setCurrentDesktop"));
            setDesktopMessage.setArguments({desktopId});
            QDBusConnection::sessionBus().send(setDesktopMessage);
        }
        const auto watcher = new QDBusPendingCallWatcher(currentDesktopCall);
        connect(watcher, &QDBusPendingCallWatcher::finished, this, [oldActivity = m_currentActivity, watcher, this] {
            watcher->deleteLater();
            auto reply = QDBusReply<int>(*watcher);
            config().writeEntry(configPattern.arg(oldActivity), reply.isValid() ? reply.value() : 0);
        });
    }
    m_currentActivity = activity;
}

void VirtualDesktopSwitchPlugin::activityRemoved(const QString &activity)
{
    config().deleteEntry(configPattern.arg(activity));
    config().sync();
}

#include "VirtualDesktopSwitchPlugin.moc"
