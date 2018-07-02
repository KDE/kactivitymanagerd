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

#ifndef MODULE_H
#define MODULE_H

#include "kactivitymanagerd_plugin_export.h"

// Qt
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDBusVariant>

// Utils
#include <utils/d_ptr.h>


/**
 * Module
 */
class KACTIVITYMANAGERD_PLUGIN_EXPORT Module : public QObject {
    Q_OBJECT

public:
    explicit Module(const QString &name, QObject *parent = nullptr);
    virtual ~Module();

    static QObject *get(const QString &name);
    static QHash<QString, QObject *> &get();

    virtual bool isFeatureOperational(const QStringList &feature) const;
    virtual bool isFeatureEnabled(const QStringList &feature) const;
    virtual void setFeatureEnabled(const QStringList &feature, bool value);
    virtual QStringList listFeatures(const QStringList &feature) const;

    virtual QDBusVariant featureValue(const QStringList &property) const;
    virtual void setFeatureValue(const QStringList &property, const QDBusVariant &value);

private:
    D_PTR;
};

#endif // MODULE_H
