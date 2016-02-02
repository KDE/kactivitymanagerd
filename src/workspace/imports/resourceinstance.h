/*
 *   Copyright (C) 2011 - 2016 by Marco Martin <mart@kde.org>
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

#ifndef RESOURCEINSTANCE_H
#define RESOURCEINSTANCE_H

//Qt
#include <QQuickItem>
#include <QUrl>

// STL
#include <memory>

namespace KActivities {
class ResourceInstance;
}

class QTimer;


namespace KActivities {
namespace Imports {

class ResourceInstance : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QUrl uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString mimetype READ mimetype WRITE setMimetype NOTIFY mimetypeChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    ResourceInstance(QQuickItem *parent = 0);
    ~ResourceInstance();

    QUrl uri() const;
    void setUri(const QUrl &uri);

    QString mimetype() const;
    void setMimetype(const QString &mimetype);

    QString title() const;
    void setTitle(const QString &title);

protected Q_SLOTS:
    void syncWid();

Q_SIGNALS:
    void uriChanged();
    void mimetypeChanged();
    void titleChanged();

public Q_SLOTS:
    /**
     * Call this method to notify the system that you modified
     * (the contents of) the resource
     */
    void notifyModified();

    /**
     * Call this method to notify the system that the resource
     * has the focus in your application
     * @note You only need to call this in MDI applications
     */
    void notifyFocusedIn();

    /**
     * Call this method to notify the system that the resource
     * lost the focus in your application
     * @note You only need to call this in MDI applications
     */
    void notifyFocusedOut();

private:
    std::unique_ptr<KActivities::ResourceInstance> m_resourceInstance;
    QUrl m_uri;
    QString m_mimetype;
    QString m_title;
    QTimer *m_syncTimer;
};

}
}

#endif
