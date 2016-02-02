/*
 *   Copyright (C) 2012 - 2016 by Ivan Cukic <ivan.cukic@kde.org>
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

#ifndef UTILS_FOR_EACH_ASSOC_H
#define UTILS_FOR_EACH_ASSOC_H

#include <kactivities-features.h>

/********************************************************************
 *  Associative container's for_each (for hash, map, and similar )  *
 ********************************************************************/

namespace kamd {
namespace utils {

namespace details { //_

// Iterator Functions

template <typename Iterator, typename Function>
Function qt_for_each_assoc(Iterator start, Iterator end, Function f)
{
    for (; start != end; ++start)
        f(start.key(), start.value());

    return f;
}

template <typename Iterator, typename Function>
Function stl_for_each_assoc(Iterator start, Iterator end, Function f)
{
    for (; start != end; ++start)
        f(start->first, start->second);

    return f;
}

// Container functions

template <typename Container, typename Function>
Function _for_each_assoc_helper_container(const Container &c, Function f,
                                          decltype(&Container::constBegin) *)
{
    // STL will never have methods with camelCase :)
    return qt_for_each_assoc(c.constBegin(), c.constEnd(), f);
}

template <typename Container, typename Function, typename Default>
Function _for_each_assoc_helper_container(const Container &c, Function f,
                                          Default *)
{
    return stl_for_each_assoc(c.cbegin(), c.cend(), f);
}

} //^ namespace details

template <typename Container, typename Function>
Function for_each_assoc(const Container &c, Function f)
{
    return details::_for_each_assoc_helper_container
        <Container, Function>(c, f, Q_NULLPTR);
}

} // namespace utils
} // namespace kamd

#endif // UTILS_FOR_EACH_ASSOC_H
