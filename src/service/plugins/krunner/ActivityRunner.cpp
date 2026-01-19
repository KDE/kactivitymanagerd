/*
 *   SPDX-FileCopyrightText: 2021 Alexander Lohnau <alexander.lohnau@gmx.de>
 *   SPDX-FileCopyrightText: 2011 Aaron Seigo <aseigo@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "ActivityRunner.h"

#include <Activities.h>
#include <KLocalizedString>
#include <krunner1adaptor.h>

K_PLUGIN_CLASS(ActivityRunner)

ActivityRunner::ActivityRunner(QObject *parent)
    : Plugin(parent)
    , m_activitiesService(nullptr)
    , m_keywordi18n(i18nc("KRunner keyword", "activity"))
    , m_keyword(QStringLiteral("activity"))
{
    setName(QStringLiteral("org.kde.ActivityManager.ActivityRunner"));
    new Krunner1Adaptor(this);
    qDBusRegisterMetaType<RemoteMatch>();
    qDBusRegisterMetaType<RemoteMatches>();
    qDBusRegisterMetaType<RemoteAction>();
    qDBusRegisterMetaType<RemoteActions>();
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/runner"), this);
    QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.runners.activities"));
}

ActivityRunner::~ActivityRunner() = default;

bool ActivityRunner::init(QHash<QString, QObject *> &modules)
{
    Plugin::init(modules);

    m_activitiesService = modules[QStringLiteral("activities")];
    return true;
}

RemoteMatches ActivityRunner::Match(const QString &query)
{
    Q_ASSERT(m_activitiesService);
    const auto currentActivity = Plugin::retrieve<QString>(m_activitiesService, "CurrentActivity");
    auto activities = Plugin::retrieve<ActivityInfoList>(m_activitiesService, "ListActivitiesWithInformation");

    const QString term = query.trimmed();
    QString searchTerm = term;

    // Allow optional 'activity' keyword prefix
    if (term.startsWith(m_keyword, Qt::CaseInsensitive)) {
        searchTerm = term.right(term.size() - m_keyword.size()).trimmed();
    } else if (term.startsWith(m_keywordi18n, Qt::CaseInsensitive)) {
        searchTerm = term.right(term.size() - m_keywordi18n.size()).trimmed();
    }

    // If search term is empty, don't return any results
    if (searchTerm.isEmpty()) {
        return {};
    }

    QList<RemoteMatch> matches;
    for (const ActivityInfo &activityInfo : std::as_const(activities)) {
        if (currentActivity != activityInfo.id) {
            auto info = Plugin::retrieve<ActivityInfo>(m_activitiesService, "ActivityInformation", Q_ARG(QString, activityInfo.id));

            // Calculate relevance based on match quality
            qreal relevance = 0.0;
            const QString activityName = info.name;

            if (activityName.compare(searchTerm, Qt::CaseInsensitive) == 0) {
                // Exact match
                relevance = 1.0;
            } else if (activityName.startsWith(searchTerm, Qt::CaseInsensitive)) {
                // Starts with search term
                relevance = 0.9;
            } else if (activityName.contains(searchTerm, Qt::CaseInsensitive)) {
                // Contains search term
                relevance = 0.7;
            }

            // Only add matches that actually match the search term
            if (relevance > 0.0) {
                RemoteMatch m;
                m.id = activityInfo.id;
                m.text = i18n("Switch to \"%1\"", info.name);
                m.iconName = info.icon;
                m.type = Type::ExactMatch;
                m.relevance = relevance;
                matches.append(m);
            }
        }
    }

    return matches;
}

QVariantMap ActivityRunner::Config()
{
    return {
        {"MinLetterCount", 3},
    };
}

RemoteActions ActivityRunner::Actions()
{
    return {};
}

void ActivityRunner::Run(const QString &matchId, const QString & /*actionId*/)
{
    QMetaObject::invokeMethod(m_activitiesService, "SetCurrentActivity", Q_ARG(QString, matchId));
}

void ActivityRunner::Teardown()
{
    // Nothing to clean up
}

void ActivityRunner::SetActivationToken(const QString &token)
{
    m_activationToken = token;
}

#include "ActivityRunner.moc"

#include "moc_ActivityRunner.cpp"
