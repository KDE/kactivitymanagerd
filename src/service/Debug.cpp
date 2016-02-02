/*
 *   Copyright (C) 2013 - 2016 by Ivan Cukic <ivan.cukic@kde.org>
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

#include "Debug.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
// logging category for this framework, default: log stuff >= warning
Q_LOGGING_CATEGORY(KAMD_LOG_ACTIVITIES,  "org.kde.kactivities.activities", QtWarningMsg)
Q_LOGGING_CATEGORY(KAMD_LOG_RESOURCES,   "org.kde.kactivities.resources", QtWarningMsg)
Q_LOGGING_CATEGORY(KAMD_LOG_APPLICATION, "org.kde.kactivities.application", QtWarningMsg)
#else
Q_LOGGING_CATEGORY(KAMD_LOG_ACTIVITIES,  "org.kde.kactivities.activities")
Q_LOGGING_CATEGORY(KAMD_LOG_RESOURCES,   "org.kde.kactivities.resources")
Q_LOGGING_CATEGORY(KAMD_LOG_APPLICATION, "org.kde.kactivities.application")
#endif
