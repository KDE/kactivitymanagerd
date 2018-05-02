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
#include "Module.h"

// Qt
#include <QHash>
#include <QString>
#include <QObject>

// Utils
#include <utils/d_ptr_implementation.h>

// Local
#include "DebugApplication.h"


class Module::Private {
public:
    static QHash<QString, QObject *> s_modules;
};

QHash<QString, QObject *> Module::Private::s_modules;

Module::Module(const QString &name, QObject *parent)
    : QObject(parent)
    , d()
{
    if (!name.isEmpty()) {
        Private::s_modules[name] = this;
    }
}

Module::~Module()
{
}

QObject *Module::get(const QString &name)
{
    Q_ASSERT(!name.isEmpty());

    if (Private::s_modules.contains(name)) {
        qCDebug(KAMD_LOG_APPLICATION) << "Returning a valid module object for:" << name;
        return Private::s_modules[name];
    }

    qCWarning(KAMD_LOG_APPLICATION) << "The requested module doesn't exist:" << name;
    return Q_NULLPTR;
}

QHash<QString, QObject *> &Module::get()
{
    return Private::s_modules;
}

bool Module::isFeatureEnabled(const QStringList &feature) const
{
    Q_UNUSED(feature);
    return false;
}

bool Module::isFeatureOperational(const QStringList &feature) const
{
    Q_UNUSED(feature);
    return false;
}

void Module::setFeatureEnabled(const QStringList &feature, bool value)
{
    Q_UNUSED(feature);
    Q_UNUSED(value);
}

QStringList Module::listFeatures(const QStringList &feature) const
{
    Q_UNUSED(feature);
    return QStringList();
}

QDBusVariant Module::featureValue(const QStringList &property) const
{
    Q_UNUSED(property);

    return QDBusVariant();
}

void Module::setFeatureValue(const QStringList &property, const QDBusVariant &value)
{
    Q_UNUSED(property);
    Q_UNUSED(value);
}

