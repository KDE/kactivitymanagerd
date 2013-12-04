/*
 *   Copyright (C) 2012 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef KACTIVITIES_MODELS_ACTIVITY_MODEL_H
#define KACTIVITIES_MODELS_ACTIVITY_MODEL_H

// Qt
#include <QObject>
#include <QAbstractListModel>
#include <QJSValue>

// STL and Boost
#include <boost/container/flat_set.hpp>
#include <memory>

// Local
#include <lib/core/controller.h>
#include <lib/core/consumer.h>
#include <lib/core/info.h>

class QModelIndex;
class QDBusPendingCallWatcher;

namespace KActivities {
namespace Models {

/**
 * ActivityModel
 */

class ActivityModel : public QAbstractListModel {
    Q_OBJECT

public:
    ActivityModel(QObject *parent = 0);
    virtual ~ActivityModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    enum Roles {
        ActivityId = Qt::UserRole,
        ActivityState = Qt::UserRole + 1
    };

    enum State {
        Invalid = 0,
        Running = 2,
        Starting = 3,
        Stopped = 4,
        Stopping = 5
    };

public Q_SLOTS:
    void setActivityName(const QString &id, const QString &name,
                         const QJSValue &callback);
    void setActivityIcon(const QString &id, const QString &icon,
                         const QJSValue &callback);

    void setCurrentActivity(const QString &id, const QJSValue &callback);

    void addActivity(const QString &name, const QJSValue &callback);
    void removeActivity(const QString &id, const QJSValue &callback);

    void stopActivity(const QString &id, const QJSValue &callback);
    void startActivity(const QString &id, const QJSValue &callback);

private Q_SLOTS:
    void onActivityNameChanged(const QString &name);
    void onActivityIconChanged(const QString &icon);
    void onActivityStateChanged(KActivities::Info::State state);

    void replaceActivities(const QStringList &activities);
    void onActivityAdded(const QString &id);
    void onActivityRemoved(const QString &id);

    void setServiceStatus(KActivities::Consumer::ServiceStatus status);

private:
    KActivities::Controller m_service;

    typedef std::unique_ptr<Info> InfoPtr;

    struct InfoPtrComparator {
        bool operator() (const InfoPtr& left, const InfoPtr& right) const
        {
            return left->name().toLower() < right->name().toLower();
        }
    };

    boost::container::flat_set<InfoPtr, InfoPtrComparator> m_activities;

    unsigned int addActivitySilently(const QString &id);

    class Private;
    friend class Private;
};

} // namespace Models
} // namespace KActivities

#endif // KACTIVITIES_MODELS_ACTIVITY_MODEL_H
