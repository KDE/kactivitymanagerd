/*
 *   Copyright (C) 2015 - 2016 by Ivan Cukic <ivan.cukic@kde.org>
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

#include <Config.h>

#include <functional>

#include <QStandardPaths>
#include <QDebug>

#include <KDirWatch>

#include <utils/d_ptr_implementation.h>

class Config::Private {
public:
    Private(Config *parent)
        : q(parent)
        , mainConfigFile(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
              + QStringLiteral("/kactivitymanagerdrc"))
        , pluginConfigFile(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
              + QStringLiteral("/kactivitymanagerd-pluginsrc"))
    {
        using namespace std::placeholders;

        watcher.addFile(mainConfigFile);
        watcher.addFile(pluginConfigFile);

        QObject::connect(
            &watcher, &KDirWatch::created,
            q, std::bind(&Private::configFileChanged, this, _1));
        QObject::connect(
            &watcher, &KDirWatch::dirty,
            q, std::bind(&Private::configFileChanged, this, _1));
    }

    void configFileChanged(const QString &file)
    {
        if (file == pluginConfigFile) {
            emit q->pluginConfigChanged();
        } else {
            emit q->mainConfigChanged();
        }
    }

    KDirWatch watcher;

private:
    Config * const q;

    const QString mainConfigFile;
    const QString pluginConfigFile;
};

Config::Config(QObject *parent)
    : Module(QStringLiteral("config"), parent)
    , d(this)
{
}

Config::~Config()
{
}


