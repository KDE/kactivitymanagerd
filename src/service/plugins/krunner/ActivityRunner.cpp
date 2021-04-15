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

KAMD_EXPORT_PLUGIN(activityrunner, ActivityRunner, "kactivitymanagerd-plugin-activityrunner.json")

ActivityRunner::ActivityRunner(QObject *parent, const QVariantList &args)
    : Plugin(parent)
    , m_activitiesService(nullptr)
    , m_keywordi18n(i18nc("KRunner keyword", "activity"))
    , m_keyword(QStringLiteral("activity"))
{
    Q_UNUSED(args)
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
    const auto currentActivity = Plugin::retrieve<QString>(m_activitiesService, "CurrentActivity", "QString");
    auto activities = Plugin::retrieve<ActivityInfoList>(m_activitiesService, "ListActivitiesWithInformation", "ActivityInfoList");

    const QString term = query.trimmed();
    bool list = false;
    QString name;

    if (term.startsWith(m_keyword, Qt::CaseInsensitive)) {
        name = term.right(term.size() - m_keyword.size()).trimmed();
    } else if (term.startsWith(m_keywordi18n, Qt::CaseInsensitive)) {
        name = term.right(term.size() - m_keywordi18n.size()).trimmed();
    } else {
        list = true;
    }

    QList<RemoteMatch> matches;
    for (const ActivityInfo &activityInfo : qAsConst(activities)) {
        if (currentActivity != activityInfo.id) {
            auto info = Plugin::retrieve<ActivityInfo>(m_activitiesService, "ActivityInformation", "ActivityInfo", Q_ARG(QString, activityInfo.id));
            if (list || info.name.startsWith(name, Qt::CaseInsensitive)) {
                RemoteMatch m;
                m.id = activityInfo.id;
                m.text = i18n("Switch to \"%1\"", info.name);
                m.iconName = info.icon;
                m.type = Type::ExactMatch;
                m.relevance = 0.7 + ((activityInfo.state == Activities::State::Running || activityInfo.state == Activities::State::Starting) ? 0.1 : 0);
                matches.append(m);
            }
        }
    }

    return matches;
}

QVariantMap ActivityRunner::Config()
{
    return {
        {"MinLetterCount", qMin(m_keyword.count(), m_keywordi18n.count())},
        {"MatchRegex", '^' + m_keyword + '|' + m_keywordi18n},
    };
}

RemoteActions ActivityRunner::Actions()
{
    return {};
}

void ActivityRunner::Run(const QString &matchId, const QString &actionId)
{
    Q_UNUSED(actionId)
    QMetaObject::invokeMethod(m_activitiesService, "SetCurrentActivity", Q_ARG(QString, matchId));
}

#include "ActivityRunner.moc"
