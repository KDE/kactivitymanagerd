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

// Self
#include "Features.h"

// Qt
#include <QDBusConnection>

// Utils
#include <utils/d_ptr_implementation.h>

// Local
#include "featuresadaptor.h"
#include "common/dbus/common.h"


class Features::Private {
};

Features::Features(QObject *parent)
    : Module(QStringLiteral("features"), parent)
    , d()
{
    new FeaturesAdaptor(this);
    QDBusConnection::sessionBus().registerObject(
        KAMD_DBUS_OBJECT_PATH(Features), this);
}

Features::~Features()
{
}

// Features object is just a gateway to the other KAMD modules.
// This is a convenience method to pass the request down to the module

template <typename RetType, typename Function>
static RetType passToModule(const QString &key, RetType defaultResult, Function f)
{
    if (key.isEmpty()) {
        return defaultResult;
    }

    const auto params = key.split(QLatin1Char('/'));
    const auto module = Module::get(params.first());

    if (!module) {
        return defaultResult;
    }

    return f(static_cast<Module *>(module),
            params.mid(1));
}

#define FEATURES_PASS_TO_MODULE(RetType, DefaultResult, What)                  \
    passToModule(key, DefaultResult,                                           \
                 [=](Module *module, const QStringList &params)->RetType{      \
                     What                                                      \
                 });

bool Features::IsFeatureOperational(const QString &key) const
{
    return FEATURES_PASS_TO_MODULE(bool, false,
                                   return module->isFeatureOperational(params););
}

QStringList Features::ListFeatures(const QString &key) const
{
    if (key.isEmpty()) {
        return Module::get().keys();
    }

    return FEATURES_PASS_TO_MODULE(QStringList, QStringList(),
                                   return module->listFeatures(params););
}

QDBusVariant Features::GetValue(const QString &key) const
{
    return FEATURES_PASS_TO_MODULE(QDBusVariant, QDBusVariant(),
                                   return module->featureValue(params););
}

void Features::SetValue(const QString &key, const QDBusVariant &value)
{
    FEATURES_PASS_TO_MODULE(bool, true,
                            module->setFeatureValue(params, value);
                            return true;
                            );
}

#undef FEATURES_PASS_TO_MODULE
