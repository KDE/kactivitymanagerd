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

#include "GtkEventSpy.h"

#include <QStringList>
#include <QString>
#include <QUrl>
#include <QFileInfo>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QStandardPaths>

#include <KCoreAddons/KDirWatch>
#include <KServiceTypeTrader>

#include "DebugPluginGtkEventSpy.h"

KAMD_EXPORT_PLUGIN(GtkEventSpyPlugin, GtkEventSpyPlugin,
                   "kactivitymanagerd-plugin-gtk-eventspy.json")

GtkEventSpyPlugin::GtkEventSpyPlugin(QObject *parent, const QVariantList &args)
    : Plugin(parent)
    , m_resources(nullptr)
    , m_dirWatcher(new KDirWatch(this))
    , m_lastUpdate(QDateTime::currentDateTime())
{
    Q_UNUSED(args);

    // gtk xml history file
    // usually $HOME/.local/share/recently-used.xbel
    QString filename = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                       + QLatin1String("/recently-used.xbel");

    m_dirWatcher->addFile(filename);
    connect(m_dirWatcher.get(), &KDirWatch::dirty,
            this, &GtkEventSpyPlugin::fileUpdated);
    connect(m_dirWatcher.get(), &KDirWatch::created,
            this, &GtkEventSpyPlugin::fileUpdated);
}

struct Application {
    QString name;
    QDateTime modified;
};

class Bookmark
{
public:
    QUrl href;
    QDateTime added;
    QDateTime modified;
    QDateTime visited;
    QList<Application> applications;

    QString latestApplication() const;
};

QString Bookmark::latestApplication() const
{
    Application current = applications.first();
    for (const Application &app : applications) {
        if (app.modified > current.modified) {
            current = app;
        }
    }
    return current.name;
}

class BookmarkHandler : public QXmlDefaultHandler
{
public:

    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName,
                      const QXmlAttributes &attributes) override;
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName) override;

    QList<Bookmark> bookmarks() const;
private:
    QList<Bookmark> marks;
    Bookmark current;
};

QList<Bookmark> BookmarkHandler::bookmarks() const
{
    return marks;
}

bool BookmarkHandler::startElement(const QString & /*namespaceURI*/, const QString & /*localName*/,
                                   const QString &qName, const QXmlAttributes &attributes)
{
    // new bookmark
    if (qName == QStringLiteral("bookmark")) {
        current = Bookmark();
        current.href = QUrl(attributes.value("href"));
        QString added = attributes.value("added");
        QString modified = attributes.value("modified");
        QString visited = attributes.value("visited");
        current.added = QDateTime::fromString(added, Qt::ISODate);
        current.modified = QDateTime::fromString(modified, Qt::ISODate);
        current.visited = QDateTime::fromString(visited, Qt::ISODate);

        // application for the current bookmark
    } else if (qName == QStringLiteral("bookmark:application")) {
        Application app;

        QString exec = attributes.value("exec");

        if (exec.startsWith(QLatin1Char('\'')) && exec.endsWith(QLatin1Char('\''))) {
            // remove "'" caracters wrapping the command
            exec = exec.mid(1, exec.size() -2);
        }

        // Search for applications which are executable and case-insensitively match the search term
        // See https://techbase.kde.org/Development/Tutorials/Services/Traders#The_KTrader_Query_Language
        const auto query = QString("exist Exec and Exec ~~ '%1'").arg(exec);
        const KService::List services
            = KServiceTypeTrader::self()->query(QStringLiteral("Application"), query);

        if (!services.isEmpty()) {
            // use the first item matching
            const auto service = services.first();
            app.name = service->desktopEntryName();
        } else {
            // when no services are found, sanitize a little the exec
            // remove space and any caracter after
            const int spaceIndex = exec.indexOf(" ");
            if (spaceIndex != -1) {
                exec = exec.mid(0, spaceIndex);
            }
            app.name = exec;
        }

        app.modified = QDateTime::fromString(attributes.value("modified"), Qt::ISODate);

        current.applications.append(app);
    }
    return true;
}

bool BookmarkHandler::endElement(const QString &namespaceURI, const QString &localName,
                                 const QString &qName)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);

    if (qName == QStringLiteral("bookmark")) {
        // keep track of the finished parsed bookmark
        marks << current;
    }

    return true;
}

void GtkEventSpyPlugin::fileUpdated(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qCWarning(KAMD_LOG_PLUGIN_GTK_EVENTSPY) << "Could not read" << filename;
        return;
    }

    // must parse the xbel xml file
    BookmarkHandler bookmarkHandler;

    QXmlSimpleReader reader;
    reader.setContentHandler(&bookmarkHandler);
    reader.setErrorHandler(&bookmarkHandler);
    QXmlInputSource source(&file);

    if (!reader.parse(source)) {
        qCWarning(KAMD_LOG_PLUGIN_GTK_EVENTSPY) << "could not parse" << file << "error was "
                                            << bookmarkHandler.errorString();
        return;
    }

    // then find the files that were accessed since last run
    const QList<Bookmark> bookmarks = bookmarkHandler.bookmarks();
    for (const Bookmark &mark : bookmarks) {
        if (mark.added > m_lastUpdate || mark.modified > m_lastUpdate
            || mark.visited > m_lastUpdate) {
            addDocument(mark.href, mark.latestApplication());
        }
    }

    m_lastUpdate = QDateTime::currentDateTime();
}

void GtkEventSpyPlugin::addDocument(const QUrl &url, const QString &application)
{
    const QString name = url.fileName();

    Plugin::invoke<Qt::QueuedConnection>(
        m_resources, "RegisterResourceEvent",
        Q_ARG(QString, application),             // Application
        Q_ARG(uint, 0),                          // Window ID
        Q_ARG(QString, url.toString()),          // URI
        Q_ARG(uint, 0)                           // Event Activities::Accessed
        );
}

GtkEventSpyPlugin::~GtkEventSpyPlugin()
{
}

bool GtkEventSpyPlugin::init(QHash<QString, QObject *> &modules)
{
    Plugin::init(modules);

    m_resources = modules["resources"];

    return true;
}

#include "GtkEventSpy.moc"
