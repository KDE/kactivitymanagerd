/*
 *   SPDX-FileCopyrightText: 2013-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "dbusfuture_p.h"

namespace DBusFuture
{
namespace detail
{ //_

template<>
void DBusCallFutureInterface<void>::callFinished()
{
    deleteLater();

    // qCDebug(KAMD_LOG_RESOURCES) << "This is call end";

    this->reportFinished();
}

ValueFutureInterface<void>::ValueFutureInterface()
{
}

QFuture<void> ValueFutureInterface<void>::start()
{
    auto future = this->future();

    this->reportFinished();

    deleteLater();

    return future;
}

} //^ namespace detail

QFuture<void> fromVoid()
{
    using namespace detail;

    auto valueFutureInterface = new ValueFutureInterface<void>();

    return valueFutureInterface->start();
}

} // namespace DBusFuture
