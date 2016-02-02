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

#ifndef KACTIVITIES_IMPORTS_UTILS_P_H
#define KACTIVITIES_IMPORTS_UTILS_P_H

// -----------------------------------------
// RAII classes for model updates ----------
// -----------------------------------------

#define DECLARE_RAII_MODEL_UPDATERS(Class)                                     \
    template <typename T> class _model_reset {                                 \
        T *model;                                                              \
                                                                               \
    public:                                                                    \
        _model_reset(T *m) : model(m)                                          \
        {                                                                      \
            model->beginResetModel();                                          \
        }                                                                      \
        ~_model_reset()                                                        \
        {                                                                      \
            model->endResetModel();                                            \
        }                                                                      \
    };                                                                         \
    template <typename T> class _model_insert {                                \
        T *model;                                                              \
                                                                               \
    public:                                                                    \
        _model_insert(T *m, const QModelIndex &parent, int first, int last)    \
            : model(m)                                                         \
        {                                                                      \
            model->beginInsertRows(parent, first, last);                       \
        }                                                                      \
        ~_model_insert()                                                       \
        {                                                                      \
            model->endInsertRows();                                            \
        }                                                                      \
    };                                                                         \
    template <typename T> class _model_remove {                                \
        T *model;                                                              \
                                                                               \
    public:                                                                    \
        _model_remove(T *m, const QModelIndex &parent, int first, int last)    \
            : model(m)                                                         \
        {                                                                      \
            model->beginRemoveRows(parent, first, last);                       \
        }                                                                      \
        ~_model_remove()                                                       \
        {                                                                      \
            model->endRemoveRows();                                            \
        }                                                                      \
    };                                                                         \
    typedef _model_reset<Class> model_reset;                                   \
    typedef _model_remove<Class> model_remove;                                 \
    typedef _model_insert<Class> model_insert;

// -----------------------------------------

#endif // KACTIVITIES_IMPORTS_UTILS_P_H

