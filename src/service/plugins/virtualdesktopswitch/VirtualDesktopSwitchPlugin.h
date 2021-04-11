/*
 *   SPDX-FileCopyrightText: 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PLUGINS_VIRTUAL_DESKTOP_SWITCH_PLUGIN_H
#define PLUGINS_VIRTUAL_DESKTOP_SWITCH_PLUGIN_H

#include <Plugin.h>

class VirtualDesktopSwitchPlugin : public Plugin
{
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID "org.kde.ActivityManager.plugins.virtualdesktopswitch")

public:
    explicit VirtualDesktopSwitchPlugin(QObject *parent = nullptr, const QVariantList &args = QVariantList());
    ~VirtualDesktopSwitchPlugin() override;

    bool init(QHash<QString, QObject *> &modules) override;

private Q_SLOTS:
    void currentActivityChanged(const QString &activity);
    void activityRemoved(const QString &activity);

private:
    QString m_currentActivity;
    QObject *m_activitiesService;
};

#endif // PLUGINS_VIRTUAL_DESKTOP_SWITCH_PLUGIN_H
