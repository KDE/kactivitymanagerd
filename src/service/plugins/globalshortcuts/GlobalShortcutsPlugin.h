/*
 *   Copyright (C) 2012 Makis Marimpis <makhsm@gmail.com>
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

#ifndef PLUGINS_GLOBAL_SHORTCUTS_PLUGIN_H
#define PLUGINS_GLOBAL_SHORTCUTS_PLUGIN_H

#include <Plugin.h>

class QSignalMapper;
class KActionCollection;
class QAction;

class GlobalShortcutsPlugin : public Plugin {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID "org.kde.ActivityManager.plugins.globalshortcutsplugin")

public:
    GlobalShortcutsPlugin(QObject *parent = Q_NULLPTR, const QVariantList &args = QVariantList());
    virtual ~GlobalShortcutsPlugin();

    bool init(QHash<QString, QObject *> &modules) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void activityAdded(const QString &activity);
    void activityRemoved(const QString &activity = QString());
    void activityChanged(const QString &activity);

private:
    inline QString activityName(const QString &activity) const;
    inline QString activityForAction(QAction *action) const;

    QObject *m_activitiesService;
    QSignalMapper *m_signalMapper;
    QStringList m_activitiesList;
    KActionCollection *m_actionCollection;
};

#endif // PLUGINS_GLOBAL_SHORTCUTS_PLUGIN_H
