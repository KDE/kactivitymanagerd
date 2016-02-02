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

#ifndef APPLICATION_H
#define APPLICATION_H

// Qt
#include <QApplication>

// Utils
#include <utils/d_ptr.h>


class Resources;
class Activities;
class Features;

/**
 * Main application object
 * This can *not* be a QGuiApplication because
 * we need KWindowSystem widgets.
 */
class Application : public QApplication {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.ActivityManager.Application")

public:
    Application(int &argc, char **argv);
    virtual ~Application();

    virtual int newInstance();

    // static Application * self();
    // static void quit();

    Resources &resources() const;
    Activities &activities() const;
    Features &features() const;

public Q_SLOTS:
    void quit();
    QString serviceVersion() const;
    bool loadPlugin(const QString &plugin);

private Q_SLOTS:
    void init();
    void loadPlugins();

private:
    D_PTR;

    friend int main(int, char**);
};

#endif // APPLICATION_H
