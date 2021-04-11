/*
 *   SPDX-FileCopyrightText: 2011, 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PLUGINS_SLC_PLUGIN_H
#define PLUGINS_SLC_PLUGIN_H

#include <Plugin.h>

class SlcPlugin : public Plugin
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.ActivityManager.SLC")
    // Q_PLUGIN_METADATA(IID "org.kde.ActivityManager.plugins.slc")

public:
    explicit SlcPlugin(QObject *parent = nullptr, const QVariantList &args = QVariantList());
    ~SlcPlugin() override;

    bool init(QHash<QString, QObject *> &modules) override;

private Q_SLOTS:
    void registeredResourceEvent(const Event &event);
    void registeredResourceMimetype(const QString &uri, const QString &mimetype);
    void registeredResourceTitle(const QString &uri, const QString &title);

public Q_SLOTS:
    QString focussedResourceURI() const;
    QString focussedResourceMimetype() const;
    QString focussedResourceTitle() const;

Q_SIGNALS:
    void focusChanged(const QString &uri, const QString &mimetype, const QString &title);

private:
    struct ResourceInfo {
        QString title;
        QString mimetype;
    };

    QHash<QString, ResourceInfo> m_resources;
    QString m_focussedResource;
};

#endif // PLUGINS_SLC_PLUGIN_H
