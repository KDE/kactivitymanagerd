/*
 *   SPDX-FileCopyrightText: 2012-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef UTILS_MERGE_INTO_H
#define UTILS_MERGE_INTO_H

namespace kamd
{
namespace utils
{
template<typename Container>
inline void merge_into(Container &into, const Container &from)
{
    typename Container::iterator into_begin = into.begin();
    typename Container::iterator into_end = into.end();
    typename Container::const_iterator from_begin = from.begin();
    typename Container::const_iterator from_end = from.end();

    while (from_begin != from_end) {
        while (into_begin != into_end && *from_begin >= *into_begin)
            into_begin++;

        into_begin = into.insert(into_begin, *from_begin);
        into_begin++;
        from_begin++;
    }
}

} // namespace utils
} // namespace kamd

#endif // UTILS_MERGE_INTO_H
