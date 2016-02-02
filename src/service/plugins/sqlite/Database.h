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

#ifndef PLUGINS_SQLITE_RESOURCESDATABASE_H
#define PLUGINS_SQLITE_RESOURCESDATABASE_H

// Qt
#include <QObject>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>

// Utils
#include <utils/d_ptr.h>

// Local
#include <Debug.h>

class QDateTime;
class QSqlDatabase;
class QSqlError;

namespace Common {
    class Database;
} // namespace Common

class ResourcesDatabaseMigrator : public QObject {
    Q_OBJECT

public:
    // static Database *self();

private:
    ResourcesDatabaseMigrator();
    ~ResourcesDatabaseMigrator();

    void migrateDatabase(const QString &newDatabaseFile) const;

    D_PTR;

    friend Common::Database &resourcesDatabase();
};

Common::Database &resourcesDatabase();

#endif // PLUGINS_SQLITE_RESOURCESDATABASE_H
