/*
 *   SPDX-FileCopyrightText: 2012 Makis Marimpis <makhsm@gmail.com>
 *   SPDX-FileCopyrightText: 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PLUGINS_GLOBAL_SHORTCUTS_PLUGIN_H
#define PLUGINS_GLOBAL_SHORTCUTS_PLUGIN_H

#include <Plugin.h>
class KActionCollection;
class QAction;

class GlobalShortcutsPlugin : public Plugin
{
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID "org.kde.ActivityManager.plugins.globalshortcutsplugin")

public:
    explicit GlobalShortcutsPlugin(QObject *parent = nullptr, const QVariantList &args = QVariantList());
    ~GlobalShortcutsPlugin() override;

    bool init(QHash<QString, QObject *> &modules) override;

private Q_SLOTS:
    void activityAdded(const QString &activity);
    void activityRemoved(const QString &activity);
    void activityChanged(const QString &activity);

Q_SIGNALS:
    void currentActivityChanged(const QString &activity);

private:
    inline QString activityName(const QString &activity) const;
    inline QString activityForAction(QAction *action) const;

    QObject *m_activitiesService;
    QStringList m_activitiesList;
    KActionCollection *m_actionCollection;
};

#endif // PLUGINS_GLOBAL_SHORTCUTS_PLUGIN_H
