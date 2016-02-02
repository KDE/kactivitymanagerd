/*
 *   Copyright (C) 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef FILE_ITEM_LINKING_PLUGIN_P_H
#define FILE_ITEM_LINKING_PLUGIN_P_H

#include "FileItemLinkingPlugin.h"

#include <QThread>

#include <KFileItemListProperties>

#include <KActivities/Consumer>
#include <KActivities/Info>

struct Action {
    QString title;
    QString icon;
    QString activity;
    bool link;
};
typedef QList<Action> ActionList;

class FileItemLinkingPlugin::Private : public QObject {
    Q_OBJECT

public:
    Private();

    QAction *root;
    QMenu *rootMenu;
    KFileItemListProperties items;

    QAction *basicAction(QWidget *parentWidget);

    KActivities::Consumer activities;

public Q_SLOTS:
    void activitiesServiceStatusChanged(KActivities::Consumer::ServiceStatus status);
    void rootActionHovered();
    void setActions(const ActionList &actions);

    void actionTriggered();
    void loadAllActions();

private:
    bool shouldLoad : 1;
    bool loaded : 1;
};

class FileItemLinkingPluginActionStaticInit {
public:
    FileItemLinkingPluginActionStaticInit();
};

#endif // FILE_ITEM_LINKING_PLUGIN_P_H
