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

#ifndef PLUGINS_SQLITE_RESOURCE_SCORE_CACHE_H
#define PLUGINS_SQLITE_RESOURCE_SCORE_CACHE_H

// Qt
#include <QString>

// Utils
#include <utils/d_ptr.h>

/**
 * ResourceScoreCache handles the persistence of the usage ratings for
 * the resources.
 *
 * It contains the logic to update the score of a single resource.
 */
class ResourceScoreCache {
public:
    ResourceScoreCache(const QString &activity, const QString &application,
                       const QString &resource);
    virtual ~ResourceScoreCache();

    void update();

private:
    D_PTR;
    class Queries;
};

#endif // PLUGINS_SQLITE_RESOURCE_SCORE_CACHE_H
