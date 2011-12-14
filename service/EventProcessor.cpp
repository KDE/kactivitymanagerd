/*
 *   Copyright (C) 2010 Ivan Cukic <ivan.cukic(at)kde.org>
 *   Copyright (c) 2010 Sebastian Trueg <trueg@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "EventProcessor.h"
#include "Plugin.h"

#include "config-features.h"

#include <KDebug>

#include <QDateTime>
#include <QList>
#include <QMutex>

#include <KDebug>
#include <KServiceTypeTrader>

#include <time.h>

class EventProcessorPrivate: public QThread {
public:
    QList < Plugin * > lazyBackends;
    QList < Plugin * > syncBackends;

    QList < Event > events;
    QMutex events_mutex;

    void run();

    static EventProcessor * s_instance;

};

EventProcessor * EventProcessorPrivate::s_instance = NULL;

void EventProcessorPrivate::run()
{
    kDebug() << "starting event processing...";

    forever {
        // initial delay before processing the events
        sleep(5); // do we need it?

        EventProcessorPrivate::events_mutex.lock();

        if (events.count() == 0) {
            // kDebug() << "No more events to process, exiting.";

            EventProcessorPrivate::events_mutex.unlock();
            return;
        }

        QList < Event > currentEvents = events;
        events.clear();

        EventProcessorPrivate::events_mutex.unlock();

        // kDebug() << "Passing" << currentEvents.size() << "events to" << lazyBackends.size() << "lazy plugins";
        foreach (Plugin * backend, lazyBackends) {
            backend->addEvents(currentEvents);
        }
    }
}

EventProcessor * EventProcessor::self()
{
    if (!EventProcessorPrivate::s_instance) {
        EventProcessorPrivate::s_instance = new EventProcessor();
    }

    return EventProcessorPrivate::s_instance;
}

// #define FAKE_EVENTS_FEED
#ifdef FAKE_EVENTS_FEED

#include <QTextStream>
#include <QDateTime>
#include <QFile>

void EventProcessor_FakeEventsFeed(EventProcessor * ep)
{
    // kDebug() << "Fake data loading";
    QFile infile("/tmp/fakedata");

    if (!infile.open(QFile::ReadOnly)) return;

    QTextStream in(&infile);

    while(!in.atEnd()) {
        QStringList args = in.readLine().split(' ');

        if (args.size() > 2) {
            // kDebug() << "processing" << args;
            // kDebug() << QDateTime::fromString(args[0] + ' ' + args[1], "yyyy.MM.dd hh:mm");

            Event newEvent(
                    args[2],      // application
                    0,            // window id
                    args[4],      // document
                    (args[3][0] == 'o') ? Event::Opened : Event::Closed, // type
                    Event::User   // reason
                    );
            newEvent.timestamp = QDateTime::fromString(args[0] + ' ' + args[1], "yyyy.MM.dd hh:mm");

            ep->addEvent(newEvent);

        }
    }
}

#endif // FAKE_EVENTS_FEED

EventProcessor::EventProcessor()
    : d(new EventProcessorPrivate())
{
    // Initializing SharedInfo
    SharedInfo * shared = SharedInfo::self();

    connect(shared, SIGNAL(scoreUpdateRequested(const QString &, const QString &)),
            this, SLOT(updateScore(const QString &, const QString &)));

    // Plugin loading

    KService::List offers = KServiceTypeTrader::self()->query("ActivityManager/Plugin");

    QStringList disabledPlugins = shared->pluginConfig("Global").readEntry("disabledPlugins", QStringList());
    // kDebug() << disabledPlugins << "disabled due to the configuration in activitymanager-pluginsrc";

    foreach(const KService::Ptr & service, offers) {
        if (!disabledPlugins.contains(service->library())) {
            disabledPlugins.append(
                    service->property("X-ActivityManager-PluginOverrides", QVariant::StringList).toStringList()
                );
            // kDebug() << service->name() << "disables" <<
            //        service->property("X-ActivityManager-PluginOverrides", QVariant::StringList);

        }
    }

    foreach(const KService::Ptr & service, offers) {
        if (disabledPlugins.contains(service->library())) {
            continue;
        }

        kDebug() << "Loading plugin:"
            << service->name() << service->storageId() << service->library()
            << service->property("X-ActivityManager-PluginType", QVariant::String);

        KPluginFactory * factory = KPluginLoader(service->library()).factory();

        if (!factory) {
            // kDebug() << "Failed to load plugin:" << service->name();
            continue;
        }

        Plugin * plugin = factory->create < Plugin > (this);

        if (plugin) {
            plugin->setSharedInfo(shared);
            plugin->init();

            const QString & type = service->property("X-ActivityManager-PluginType", QVariant::String).toString();

            if (type == "lazyeventhandler") {
                d->lazyBackends << plugin;
                // kDebug() << "Added to lazy plugins";

            } else if (type == "synceventhandler"){
                d->syncBackends << plugin;
                // kDebug() << "Added to sync plugins";

            }

        } else {
            // kDebug() << "Failed to load plugin:" << service->name();
        }

    }

#ifdef FAKE_EVENTS_FEED
    EventProcessor_FakeEventsFeed(this);
#endif // FAKE_EVENTS_FEED
}

EventProcessor::~EventProcessor()
{
    qDeleteAll(d->lazyBackends);
    qDeleteAll(d->syncBackends);
    delete d;
}

void EventProcessor::addEvent(const QString & application, WId wid, const QString & uri,
            int type, int reason)
{
    Event newEvent(application, wid, uri, type, reason);

    addEvent(newEvent);
}

void EventProcessor::addEvent(const Event & newEvent)
{
    // Passing the events to the plugins that want them immediately
    foreach (Plugin * backend, d->syncBackends) {
        backend->addEvents(QList < Event > () << newEvent);
    }

    // And now, for something completely delayed
    d->events_mutex.lock();

    // Deleting the accessed events if we have other types of events
    // with the same parameters
    if (newEvent.type != Event::Accessed) {
        foreach (const Event & event, d->events) {
            if (event.type == Event::Accessed && event.uri == newEvent.uri
                    && event.application == newEvent.application) {
                // Accessed events are of a lower priority
                // then the other ones
                if (newEvent.type == Event::Accessed) {
                    d->events.removeAll(newEvent);
                }
            }
        }
    }

    d->events.append(newEvent);

    d->events_mutex.unlock();

    d->start();
}

void EventProcessor::updateScore(const QString & application, const QString & uri)
{
    // This is not a real event, it just notifies the system to recalculate
    // the score for the specified uri
    EventProcessor::self()->addEvent(application, 0, uri,
            Event::UpdateScore, Event::UserEventReason);

    // kDebug() << "Score updating requested for" << application << uri;
}


