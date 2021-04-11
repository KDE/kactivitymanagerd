/*
 *   SPDX-FileCopyrightText: 2011, 2012 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PLUGINS_SQLITE_RESOURCESDATABASE_H
#define PLUGINS_SQLITE_RESOURCESDATABASE_H

// Qt
#include <QDateTime>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>

// Utils
#include <utils/d_ptr.h>

// Local
#include <common/database/Database.h>

class QDateTime;
class QSqlError;

namespace Common
{
class Database;
} // namespace Common

class ResourcesDatabaseInitializer
{
public:
    // static Database *self();

private:
    ResourcesDatabaseInitializer();
    ~ResourcesDatabaseInitializer();

    void initDatabase(bool retryOnFail = true);

    D_PTR;

    friend Common::Database::Ptr resourcesDatabase();
};

Common::Database::Ptr resourcesDatabase();

#endif // PLUGINS_SQLITE_RESOURCESDATABASE_H
