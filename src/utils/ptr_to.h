/*
 *   SPDX-FileCopyrightText: 2015-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef PTR_TO_H
#define PTR_TO_H

namespace kamd
{
namespace utils
{
enum {
    Const = 0,
    Mutable = 1,
};

template<typename T, int Policy = Const>
struct ptr_to {
    typedef const T *const type;
};

template<typename T>
struct ptr_to<T, Mutable> {
    typedef T *const type;
};

} // namespace utils
} // namespace kamd

#endif // PTR_TO_H
