/*
 *   Copyright (C) 2011, 2012 Ivan Cukic <ivan.cukic(at)kde.org>
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

// Self
#include <kactivities-features.h>
#include "Database.h"

// Qt
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QVariant>

// KDE
#include <kdelibs4migration.h>

// Utils
#include <utils/d_ptr_implementation.h>
#include <utils/qsqlquery_iterator.h>

// System
#include <cmath>
#include <memory>

// Local
#include "Debug.h"
#include "Utils.h"

#include <common/database/Database.h>
#include <common/database/schema/ResourcesDatabaseSchema.h>

class ResourcesDatabaseMigrator::Private {
public:
    Common::Database::Ptr database;

};

Common::Database::Ptr resourcesDatabase()
{
    static ResourcesDatabaseMigrator instance;
    return instance.d->database;
}

ResourcesDatabaseMigrator::ResourcesDatabaseMigrator()
{
    const QString databaseDir
        = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
          + QStringLiteral("/kactivitymanagerd/resources/");

    qDebug() << "Creating directory: " << databaseDir;
    auto created = QDir().mkpath(databaseDir);

    if (!created || !QDir(databaseDir).exists()) {
        qWarning() << "Database folder can not be created!";
    }

    d->database = Common::Database::instance(
            Common::Database::ResourcesDatabase,
            Common::Database::ReadWrite);

    if (d->database) {
        Common::ResourcesDatabaseSchema::initSchema(*d->database);
    }
}

ResourcesDatabaseMigrator::~ResourcesDatabaseMigrator()
{
}

