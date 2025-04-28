/*
 *   SPDX-FileCopyrightText: 2010-2016 Ivan Cukic <ivan.cukic@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

// Self
#include "Resources.h"

// Local
#include "resourcesadaptor.h"

class Resources::Private : public QThread
{
    Q_OBJECT

public:
    Private(Resources *parent);
    ~Private() override;

    void run() override;

    // Inserts the event directly into the queue
    void insertEvent(const Event &newEvent);

    // Processes the event and inserts it into the queue
    void addEvent(const Event &newEvent);

private:
    Resources *const q;
};
