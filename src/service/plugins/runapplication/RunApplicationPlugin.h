/*
 *   SPDX-FileCopyrightText: 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PLUGINS_RUN_APPLICATION_PLUGIN_H
#define PLUGINS_RUN_APPLICATION_PLUGIN_H

#include <Plugin.h>

class RunApplicationPlugin : public Plugin
{
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID "org.kde.ActivityManager.plugins.virtualdesktopswitch")

public:
    explicit RunApplicationPlugin(QObject *parent = nullptr, const QVariantList &args = QVariantList());
    ~RunApplicationPlugin() override;

    bool init(QHash<QString, QObject *> &modules) override;

private Q_SLOTS:
    void currentActivityChanged(const QString &activity);
    void activityStateChanged(const QString &activity, int state);

private:
    QString activityDirectory(const QString &activity) const;
    void executeIn(const QString &directory) const;

    QString m_currentActivity;
    QStringList m_previousActivities;
    QObject *m_activitiesService;
};

#endif // PLUGINS_RUN_APPLICATION_PLUGIN_H
