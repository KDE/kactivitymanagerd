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

#include <memory>
#include <string>
#include <iostream>
#include <utility>

struct Question {
    long answer;
    std::string description;
};

int main()
{
    std::unique_ptr < Question > node_original(new Question());

    node_original->answer = 42;
    node_original->description = "The Answer to the Ultimate Question of Life, the Universe, and Everything";

    std::unique_ptr < Question > node_second(std::move(node_original));

    return (!node_original && (node_second->answer == 42))?0:1;
}
