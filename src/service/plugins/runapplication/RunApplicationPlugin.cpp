/*
 *   SPDX-FileCopyrightText: 2012, 2013, 2014 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "RunApplicationPlugin.h"

#include <QDesktopServices>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <QUrl>

#include "DebugApplication.h"

#include <KService>

K_PLUGIN_CLASS(RunApplicationPlugin)

RunApplicationPlugin::RunApplicationPlugin(QObject *parent)
    : Plugin(parent)
    , m_activitiesService(nullptr)
{
    setName(QStringLiteral("org.kde.ActivityManager.RunApplication"));
}

RunApplicationPlugin::~RunApplicationPlugin()
{
}

bool RunApplicationPlugin::init(QHash<QString, QObject *> &modules)
{
    Plugin::init(modules);

    m_activitiesService = modules[QStringLiteral("activities")];

    connect(m_activitiesService, SIGNAL(CurrentActivityChanged(QString)), this, SLOT(currentActivityChanged(QString)));

    const auto currentActivity = Plugin::retrieve<QString>(m_activitiesService, "CurrentActivity");

    currentActivityChanged(currentActivity);

    return true;
}

QString RunApplicationPlugin::activityDirectory(const QString &activity) const
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/kactivitymanagerd/activities/") + activity
        + QLatin1Char('/');
}

void RunApplicationPlugin::currentActivityChanged(const QString &activity)
{
    if (m_currentActivity == activity) {
        return;
    }

    if (!m_currentActivity.isEmpty()) {
        executeIn(activityDirectory(m_currentActivity) + QStringLiteral("deactivated"));
    }

    m_currentActivity = activity;

    executeIn(activityDirectory(activity) + QStringLiteral("activated"));

    if (!m_previousActivities.contains(activity)) {
        m_previousActivities << activity;
    }
}

void RunApplicationPlugin::executeIn(const QString &path) const
{
    QDir directory(path);
    const auto fileInfos = directory.entryInfoList(QDir::Files);
    for (const auto &fileInfo : fileInfos) {
        QString filePath = fileInfo.filePath();
        if (fileInfo.suffix() == "sh" && fileInfo.isExecutable()) {
            qCDebug(KAMD_LOG_APPLICATION) << "Starting a shell script: " << filePath;
            QProcess::startDetached(filePath, QStringList());

        } else if (KService service(filePath); service.isValid() && service.isApplication()) {
            qCDebug(KAMD_LOG_APPLICATION) << "Starting application: " << service.exec();
            QProcess::startDetached(service.exec(), QStringList());

        } else {
            qCDebug(KAMD_LOG_APPLICATION) << "Opening file: " << QUrl::fromLocalFile(filePath);
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        }
    }
}

// void RunApplicationPlugin::activityRemoved(const QString &activity)
// {
//     // TODO: Clean up the directory
// }

#include "RunApplicationPlugin.moc"

#include "moc_RunApplicationPlugin.cpp"
