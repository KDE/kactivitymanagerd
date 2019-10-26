/*
 *   Copyright (C) 2019 Méven Car (meven.car@kdemail.net)
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLUGINS_GTK_EVENT_SPY_PLUGIN_H
#define PLUGINS_GTK_EVENT_SPY_PLUGIN_H

#include <memory>
#include <Plugin.h>

#include <QStringList>

class KDirWatch;

class GtkEventSpyPlugin : public Plugin
{
    Q_OBJECT

public:
    explicit GtkEventSpyPlugin(QObject *parent = nullptr,
                               const QVariantList &args = QVariantList());
    ~GtkEventSpyPlugin() override;

    bool init(QHash<QString, QObject *> &modules) override;

private Q_SLOTS:
    void fileUpdated(const QString &file);
    void addDocument(const QUrl &url, const QString &application, const QString &mimetype);

private:
    QObject *m_resources;
    std::unique_ptr<KDirWatch> m_dirWatcher;
    QDateTime m_lastUpdate;
};

#endif // PLUGINS_GTK_EVENT_SPY_PLUGIN_H
