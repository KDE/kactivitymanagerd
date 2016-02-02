/*
 *   Copyright (C) 2011 - 2016 by Ivan Cukic <ivan.cukic@kde.org>
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

#ifndef PLUGINS_SLC_PLUGIN_H
#define PLUGINS_SLC_PLUGIN_H

#include <Plugin.h>

class SlcPlugin : public Plugin {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.ActivityManager.SLC")
    // Q_PLUGIN_METADATA(IID "org.kde.ActivityManager.plugins.slc")

public:
    explicit SlcPlugin(QObject *parent = Q_NULLPTR, const QVariantList &args = QVariantList());
    ~SlcPlugin();

    bool init(QHash<QString, QObject *> &modules) Q_DECL_OVERRIDE;

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
