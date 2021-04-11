/*
 *   SPDX-FileCopyrightText: 2011, 2012 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef PLUGINS_SQLITE_RESOURCE_SCORE_MAINTAINER_H
#define PLUGINS_SQLITE_RESOURCE_SCORE_MAINTAINER_H

#include <QObject>

// Utils
#include <utils/d_ptr.h>

class ResourceScoreMaintainerPrivate;
class QString;

/**
 * ResourceScoreMaintainer represents a queue of resource processing requests.
 */
class ResourceScoreMaintainer : public QObject
{
public:
    static ResourceScoreMaintainer *self();

    ~ResourceScoreMaintainer() override;

    void processResource(const QString &resource, const QString &application);

private:
    ResourceScoreMaintainer();

    D_PTR;
};

#endif // PLUGINS_SQLITE_RESOURCE_SCORE_MAINTAINER_H
