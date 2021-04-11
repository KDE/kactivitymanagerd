/*
 *   SPDX-FileCopyrightText: 2019 Méven Car (meven.car@kdemail.net)
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PLUGINS_GTK_EVENT_SPY_PLUGIN_H
#define PLUGINS_GTK_EVENT_SPY_PLUGIN_H

#include <Plugin.h>
#include <memory>

#include <QStringList>

class KDirWatch;

class GtkEventSpyPlugin : public Plugin
{
    Q_OBJECT

public:
    explicit GtkEventSpyPlugin(QObject *parent = nullptr, const QVariantList &args = QVariantList());
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
