/*
 *   SPDX-FileCopyrightText: 2015-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

namespace kamd
{
namespace utils
{
template<typename F>
class lazy_val
{
public:
    lazy_val(F f)
        : _f(std::forward<F>(f))
        , valueRetrieved(false)
    {
    }

private:
    F _f;
    mutable decltype(_f()) value;
    mutable bool valueRetrieved;

public:
    operator decltype(_f())() const
    {
        if (!valueRetrieved) {
            valueRetrieved = true;
            value = _f();
        }

        return value;
    }
};

template<typename F>
inline lazy_val<F> make_lazy_val(F &&f)
{
    return lazy_val<F>(std::forward<F>(f));
}

} // namespace utils
} // namespace kamd
