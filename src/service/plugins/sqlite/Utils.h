/*
 *   Copyright (C) 2014 - 2016 by Ivan Cukic <ivan.cukic@kde.org>
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

#ifndef PLUGINS_SQLITE_DATABASE_UTILS_H
#define PLUGINS_SQLITE_DATABASE_UTILS_H

#include <QSqlQuery>
#include <common/database/schema/ResourcesDatabaseSchema.h>
#include <memory>

namespace Utils {

    inline bool prepare(Common::Database &database,
                        QSqlQuery &query,
                        const QString &queryString)
    {
        Q_UNUSED(database);

        return query.prepare(queryString);
    }

    inline bool prepare(Common::Database &database,
                        std::unique_ptr<QSqlQuery> &query,
                        const QString &queryString)
    {
        if (query) {
            return true;
        }

        query.reset(new QSqlQuery(database.createQuery()));

        return prepare(database, *query, queryString);
    }

    enum ErrorHandling {
        IgnoreError,
        FailOnError
    };

    inline bool exec(ErrorHandling eh, QSqlQuery &query)
    {
        bool success = query.exec();

        if (eh == FailOnError) {
            if (!success) {
                qDebug() << query.lastQuery();
                qDebug() << query.lastError();
            }
            Q_ASSERT_X(success, "Uils::exec", "Query failed");
        }

        return success;
    }

    template <typename T1, typename T2, typename... Ts>
    inline bool exec(ErrorHandling eh, QSqlQuery &query,
                     const T1 &variable, const T2 &value, Ts... ts)
    {
        query.bindValue(variable, value);

        return exec(eh, query, ts...);
    }

} // namespace Utils


#endif /* !PLUGINS_SQLITE_DATABASE_UTILS_H */
