/*
 *   SPDX-FileCopyrightText: 2015-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef UTILS_SLIDE_H
#define UTILS_SLIDE_H

#include <algorithm>

// Inspired by C++ Seasoning talk by Sean Parent

namespace kamd
{
namespace utils
{
template<typename Iterator>
void slide(Iterator f, Iterator l, Iterator p)
{
    if (p < f) {
        std::rotate(p, f, l);
    } else if (l < p) {
        std::rotate(f, l, p);
    }
}

template<typename Iterator>
void slide_one(Iterator f, Iterator p)
{
    slide(f, f + 1, p);
}

} // namespace utils
} // namespace kamd

#endif // UTILS_SLIDE_H
