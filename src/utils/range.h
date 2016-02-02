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

#ifndef UTILS_RANGE_H
#define UTILS_RANGE_H

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>

/********************************************************************
 *  Syntactic sugar for converting ranges to collections            *
 ********************************************************************/

namespace kamd {
namespace utils {

template <typename Collection, typename Range>
__inline Collection as_collection(Range range)
{
    Collection result;

    boost::copy(range, std::back_inserter(result));

    return result;
}

template <typename Member, typename ...Args>
__inline auto transformed(Member member, Args... args)
    -> decltype(boost::adaptors::transformed(
                std::bind(member, args..., std::placeholders::_1)))
{
    return boost::adaptors::transformed(
        std::bind(member, args..., std::placeholders::_1)
    );

}

template <typename Member, typename ...Args>
__inline auto filtered(Member member, Args... args)
    -> decltype(boost::adaptors::filtered(
                std::bind(member, args..., std::placeholders::_1)))
{
    return boost::adaptors::filtered(
        std::bind(member, args..., std::placeholders::_1)
    );

}

template <typename Class, typename Member>
__inline auto filtered(Class *const self, Member member)
    -> decltype(boost::adaptors::filtered(
                std::bind(member, self, std::placeholders::_1)))
{
    return boost::adaptors::filtered(
        std::bind(member, self, std::placeholders::_1)
    );

}


} // namespace utils
} // namespace kamd

#endif // UTILS_RANGE_H
