/*
 *   SPDX-FileCopyrightText: 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PLUGINS_EVENT_SPY_PLUGIN_H
#define PLUGINS_EVENT_SPY_PLUGIN_H

#include <Plugin.h>
#include <memory>

#include <QStringList>

class KDirWatch;

class EventSpyPlugin : public Plugin
{
    Q_OBJECT

public:
    explicit EventSpyPlugin(QObject *parent = nullptr, const QVariantList &args = QVariantList());
    ~EventSpyPlugin() override;

    bool init(QHash<QString, QObject *> &modules) override;

private Q_SLOTS:
    void directoryUpdated(const QString &dir);
    void addDocument(const QString &document);

private:
    QObject *m_resources;
    std::unique_ptr<KDirWatch> m_dirWatcher;
    QDateTime m_lastUpdate;
};

#endif // PLUGINS_EVENT_SPY_PLUGIN_H
