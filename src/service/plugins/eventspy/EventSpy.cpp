/*
 *   SPDX-FileCopyrightText: 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "EventSpy.h"

#include <QFileInfo>
#include <QString>

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KDesktopFile>
#include <KCoreAddons/KDirWatch>
#include <KIOCore/KRecentDocument>

KAMD_EXPORT_PLUGIN(eventspyplugin, EventSpyPlugin, "kactivitymanagerd-plugin-eventspy.json")

EventSpyPlugin::EventSpyPlugin(QObject *parent, const QVariantList &args)
    : Plugin(parent)
    , m_resources(nullptr)
    , m_dirWatcher(new KDirWatch(this))
    , m_lastUpdate(QDateTime::currentDateTime())
{
    Q_UNUSED(args);

    m_dirWatcher->addDir(KRecentDocument::recentDocumentDirectory());

    connect(m_dirWatcher.get(), &KDirWatch::dirty, this, &EventSpyPlugin::directoryUpdated);
}

void EventSpyPlugin::directoryUpdated(const QString &dir)
{
    Q_UNUSED(dir);
    const auto newDocuments = KRecentDocument::recentDocuments();

    // Processing the new arrivals
    for (const auto &document : newDocuments) {
        QFileInfo fileInfo(document);
        if (fileInfo.lastModified() > m_lastUpdate) {
            addDocument(document);
        }
    }

    m_lastUpdate = QDateTime::currentDateTime();
}

void EventSpyPlugin::addDocument(const QString &document)
{
    const KDesktopFile desktopFile(document);
    const KConfigGroup desktopGroup(&desktopFile, "Desktop Entry");

    const QString url = QUrl(desktopFile.readUrl()).toLocalFile();
    const QString name = desktopFile.readName();
    const QString application = desktopGroup.readEntry("X-KDE-LastOpenedWith", QString());

    Plugin::invoke<Qt::QueuedConnection>(m_resources,
                                         "RegisterResourceEvent",
                                         Q_ARG(QString, application), // Application
                                         Q_ARG(uint, 0), // Window ID
                                         Q_ARG(QString, url), // URI
                                         Q_ARG(uint, 0) // Event Activities::Accessed
    );
}

EventSpyPlugin::~EventSpyPlugin()
{
}

bool EventSpyPlugin::init(QHash<QString, QObject *> &modules)
{
    Plugin::init(modules);

    m_resources = modules["resources"];

    return true;
}

#include "EventSpy.moc"
