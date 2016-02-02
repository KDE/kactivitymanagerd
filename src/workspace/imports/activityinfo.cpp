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
#include "activityinfo.h"

namespace KActivities {
namespace Imports {

ActivityInfo::ActivityInfo(QObject *parent)
    : QObject(parent)
    , m_showCurrentActivity(false)
{
    connect(&m_service, &KActivities::Controller::currentActivityChanged,
            this, &ActivityInfo::setCurrentActivity);
}

ActivityInfo::~ActivityInfo()
{
}

void ActivityInfo::setCurrentActivity(const QString &id)
{
    if (!m_showCurrentActivity) return;

    setIdInternal(id);

    emit nameChanged(m_info->name());
    emit descriptionChanged(m_info->description());
    emit iconChanged(m_info->icon());
}

void ActivityInfo::setActivityId(const QString &id)
{
    m_showCurrentActivity = (id == ":current");

    setIdInternal(m_showCurrentActivity ?
            m_service.currentActivity() : id);
}

void ActivityInfo::setIdInternal(const QString &id)
{
    using namespace KActivities;

    // We are killing the old info object, if any
    m_info.reset(new KActivities::Info(id));

    auto ptr = m_info.get();

    connect(ptr, &Info::nameChanged,
            this, &ActivityInfo::nameChanged);
    connect(ptr, &Info::descriptionChanged,
            this, &ActivityInfo::descriptionChanged);
    connect(ptr, &Info::iconChanged,
            this, &ActivityInfo::iconChanged);
}

#define CREATE_GETTER_AND_SETTER(WHAT, What)                                   \
    QString ActivityInfo::What() const                                         \
    {                                                                          \
        return m_info ? m_info->What() : "";                                   \
    }                                                                          \
                                                                               \
    void ActivityInfo::set##WHAT(const QString &value)                         \
    {                                                                          \
        if (!m_info)                                                           \
            return;                                                            \
                                                                               \
        m_service.setActivity##WHAT(m_info->id(), value);                      \
    }

CREATE_GETTER_AND_SETTER(Name, name)
CREATE_GETTER_AND_SETTER(Description, description)
CREATE_GETTER_AND_SETTER(Icon, icon)

#undef CREATE_GETTER_AND_SETTER

QString ActivityInfo::activityId() const
{
    return m_info ? m_info->id() : "";
}

bool ActivityInfo::valid() const
{
    return true;
}


} // namespace Imports
} // namespace KActivities

// #include "activityinfo.moc"

