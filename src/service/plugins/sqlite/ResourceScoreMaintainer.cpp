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

// Self
#include "ResourceScoreMaintainer.h"

// Qt
#include <QList>
#include <QMutex>
#include <QThread>


// System
#include <time.h>

// Utils
#include <utils/for_each_assoc.h>
#include <utils/d_ptr_implementation.h>

// Local
#include "StatsPlugin.h"
#include "ResourceScoreCache.h"


class ResourceScoreMaintainer::Private : public QThread {
public:
    typedef QString ApplicationName;
    typedef QString ActivityID;
    typedef QList<QString> ResourceList;

    typedef QHash<ApplicationName, ResourceList> Applications;
    typedef QHash<ActivityID, Applications> ResourceTree;

    ResourceTree scheduledResources;
    QMutex scheduledResources_mutex;

    void run() Q_DECL_OVERRIDE;
    void processActivity(const ActivityID &activity,
                         const Applications &applications);
};

void ResourceScoreMaintainer::Private::run()
{
    using namespace kamd::utils;

    forever {
        // initial delay before processing the resources
        sleep(1);

        ResourceTree resources;

        {
            QMutexLocker lock(&scheduledResources_mutex);
            std::swap(resources, scheduledResources);
        }

        const auto activity = StatsPlugin::self()->currentActivity();

        // Let us first process the events related to the current
        // activity so that the stats are available quicker

        if (resources.contains(activity)) {
            processActivity(activity, resources[activity]);
            resources.remove(activity);
        }

        for_each_assoc(resources,
            [this](const ActivityID & activity, const Applications & applications) {
                processActivity(activity, applications);
            }
        );
    }
}

void ResourceScoreMaintainer::Private::processActivity(const ActivityID
                                                       &activity,
                                                       const Applications
                                                       &applications)
{
    using namespace kamd::utils;

    for_each_assoc(applications,
        [&](const ApplicationName &application, const ResourceList &resources) {
            for (const auto &resource : resources) {
                ResourceScoreCache(activity, application, resource).update();
            }
        }
    );
}

ResourceScoreMaintainer *ResourceScoreMaintainer::self()
{
    static ResourceScoreMaintainer instance;
    return &instance;
}

ResourceScoreMaintainer::ResourceScoreMaintainer()
    : d()
{
}

ResourceScoreMaintainer::~ResourceScoreMaintainer()
{
}

void ResourceScoreMaintainer::processResource(const QString &resource,
                                              const QString &application)
{
    QMutexLocker lock(&d->scheduledResources_mutex);

    // Checking whether the item is already scheduled for
    // processing

    const auto activity = StatsPlugin::self()->currentActivity();

    Q_ASSERT_X(!application.isEmpty(),
               "ResourceScoreMaintainer::processResource",
               "Agent shoud not be empty");
    Q_ASSERT_X(!resource.isEmpty(),
               "ResourceScoreMaintainer::processResource",
               "Resource shoud not be empty");

    if (d->scheduledResources.contains(activity)
        && d->scheduledResources[activity].contains(application)
        && d->scheduledResources[activity][application].contains(resource)) {

        // Nothing

    } else {
        d->scheduledResources[activity][application] << resource;
    }

    d->start();
}
