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

#ifndef DEBUG_AND_RETURN_H
#define DEBUG_AND_RETURN_H

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace kamd {
namespace utils {

template<typename T>
T debug_and_return(const char * message, T && value) {
    #ifdef QT_DEBUG
    qDebug() << message << " " << value;
    #endif

    return std::forward<T>(value);
}

template<typename T>
T debug_and_return(bool debug, const char * message, T && value) {
    #ifdef QT_DEBUG
    if (debug) {
        qDebug() << message << " " << value;
    }
    #endif

    return std::forward<T>(value);
}

} // namespace utils
} // namespace kamd

#endif // DEBUG_AND_RETURN_H

