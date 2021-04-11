/*
 *   SPDX-FileCopyrightText: 2015-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef DEBUG_AND_RETURN_H
#define DEBUG_AND_RETURN_H

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace kamd
{
namespace utils
{
template<typename T>
T debug_and_return(const char *message, T &&value)
{
#ifdef QT_DEBUG
    qCDebug(KAMD_LOG_RESOURCES) << message << " " << value;
#endif

    return std::forward<T>(value);
}

template<typename T>
T debug_and_return(bool debug, const char *message, T &&value)
{
#ifdef QT_DEBUG
    if (debug) {
        qCDebug(KAMD_LOG_RESOURCES) << message << " " << value;
    }
#endif

    return std::forward<T>(value);
}

} // namespace utils
} // namespace kamd

#endif // DEBUG_AND_RETURN_H
