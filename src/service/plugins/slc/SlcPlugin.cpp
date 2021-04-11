/*
 *   SPDX-FileCopyrightText: 2011, 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

// Self
#include "SlcPlugin.h"

// Qt
#include <QDBusConnection>

// Local
#include "slcadaptor.h"

KAMD_EXPORT_PLUGIN(slcplugin, SlcPlugin, "kactivitymanagerd-plugin-slc.json")

SlcPlugin::SlcPlugin(QObject *parent, const QVariantList &args)
    : Plugin(parent)
{
    Q_UNUSED(args);

    new SLCAdaptor(this);
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/SLC"), this);
}

SlcPlugin::~SlcPlugin()
{
}

QString SlcPlugin::focussedResourceURI() const
{
    return m_focussedResource;
}

QString SlcPlugin::focussedResourceMimetype() const
{
    return m_resources[m_focussedResource].mimetype;
}

QString SlcPlugin::focussedResourceTitle() const
{
    return m_resources[m_focussedResource].title;
}

void SlcPlugin::registeredResourceEvent(const Event &event)
{
    switch (event.type) {
    case Event::FocussedIn:

        if (!event.uri.startsWith(QLatin1String("about"))) {
            if (m_focussedResource != event.uri) {
                m_focussedResource = event.uri;
                const auto &info = m_resources[m_focussedResource];
                emit focusChanged(event.uri, info.mimetype, info.title);
            }
        } else {
            m_focussedResource.clear();
            emit focusChanged(QString(), QString(), QString());
        }

        break;

    case Event::FocussedOut:

        if (m_focussedResource == event.uri) {
            m_focussedResource.clear();
            emit focusChanged(QString(), QString(), QString());
        }

        break;

    case Event::Closed:
        m_resources.remove(event.uri);

        break;

    default:
        break;
    }
}

void SlcPlugin::registeredResourceMimetype(const QString &uri, const QString &mimetype)
{
    m_resources[uri].mimetype = mimetype;
}

void SlcPlugin::registeredResourceTitle(const QString &uri, const QString &title)
{
    m_resources[uri].title = title;
}

bool SlcPlugin::init(QHash<QString, QObject *> &modules)
{
    Plugin::init(modules);

    connect(modules[QStringLiteral("resources")], SIGNAL(RegisteredResourceEvent(Event)), this, SLOT(registeredResourceEvent(Event)), Qt::QueuedConnection);
    connect(modules[QStringLiteral("resources")],
            SIGNAL(RegisteredResourceMimetype(QString, QString)),
            this,
            SLOT(registeredResourceMimetype(QString, QString)),
            Qt::QueuedConnection);
    connect(modules[QStringLiteral("resources")],
            SIGNAL(RegisteredResourceTitle(QString, QString)),
            this,
            SLOT(registeredResourceTitle(QString, QString)),
            Qt::QueuedConnection);

    return true;
}

#include "SlcPlugin.moc"
