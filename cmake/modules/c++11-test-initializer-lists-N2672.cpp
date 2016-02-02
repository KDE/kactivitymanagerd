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

// #include <iostream>
// #include <vector>
#include <string>

struct test {
    int i;
    int j;
    double k;
    std::string s;
};

int main()
{
    test t { 1, 2, 4, "asdf" };
    // std::vector<int> v { 1, 2, 3, 4 };

    // return (
    //         t.i == v[0]
    //         && t.j == v[1]
    //         && t.k > v[2]
    //         && t.s.size() == v[3]
    //         )
    //     ? 0 : 1;

    return 0;
}
