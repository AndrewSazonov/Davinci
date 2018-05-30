/*
 * Davinci, a software for the single-crystal diffraction data reduction.
 * Copyright (C) 2015-2017 Andrew Sazonov
 *
 * This file is part of Davinci.
 *
 * Davinci is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Davinci is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Davinci.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtWidgets>

#include "Constants.hpp"

#include "SortFilterProxyModel.hpp"


/*!
\class As::SortFilterProxyModel

\brief The SortFilterProxyModel is a custom class based on the QSortFilterProxyModel. Required
to add custom sort order for tables (date and time, numbers in strings, etc.)

\inmodule Widgets
\ingroup Widgets
*/

/*!
Constructs a SortFilterProxyModel with the given \a parent.
*/
As::SortFilterProxyModel::SortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

/*!
Returns \c true if the value of the item referred to by the given index \a left is less than
the value of the item referred to by the given index \a right, otherwise returns \c false.

This function is used as the < operator when sorting.

The default function of the QSortingFilterProxyModel is reimplemented here in order to add the
custom sort order for tables.

If strings can be converted to date and time, than sort by date is applied.

If strings can be converted to numbers, than natural sort order is applied.

Otherwise, alphabetical order is applied.

\sa \link https://en.wikipedia.org/wiki/Natural_sort_order Wiki: Natural sort order \endlink
*/
bool As::SortFilterProxyModel::lessThan(const QModelIndex &left,
                                        const QModelIndex &right) const
{
    const QString leftDataStr = sourceModel()->data(left).toString();
    const QString rightDataStr = sourceModel()->data(right).toString();

    // Skip empty rows
    if (leftDataStr.isEmpty())
        return false;

    // Check if the cell contains date&time object in the specific format
    const QDateTime leftDateTime = QDateTime::fromString(leftDataStr, As::DATA_TIME_FORMAT);
    if (leftDateTime.isValid()) {
        const QDateTime rightDateTime = QDateTime::fromString(rightDataStr, As::DATA_TIME_FORMAT);
        return leftDateTime < rightDateTime;
    }

    // Check if the cell contains double (real) number
    bool ok;
    const qreal leftDouble = leftDataStr.toDouble(&ok);
    if (ok) {
        const qreal rightDouble = rightDataStr.toDouble();
        return leftDouble < rightDouble; }

    // Default comparison as strings
    return leftDataStr < rightDataStr;
}

/*
QVariant leftData = sourceModel()->data(left);
QVariant rightData = sourceModel()->data(right);

if (leftData.type() == QVariant::DateTime) {
    return leftData.toDateTime() < rightData.toDateTime();
} else {
    static QRegExp emailPattern("[\\w\\.]*@[\\w\\.]*)");

    QString leftString = leftData.toString();
    if(left.column() == 1 && emailPattern.indexIn(leftString) != -1)
        leftString = emailPattern.cap(1);

    QString rightString = rightData.toString();
    if(right.column() == 1 && emailPattern.indexIn(rightString) != -1)
        rightString = emailPattern.cap(1);

    return QString::localeAwareCompare(leftString, rightString) < 0;
}
*/

/*
    Returns \c true if the value of the item referred to by the given
    index \a source_left is less than the value of the item referred to by
    the given index \a source_right, otherwise returns \c false.

    This function is used as the < operator when sorting, and handles
    the following QVariant types:

    \list
    \li QMetaType::Int
    \li QMetaType::UInt
    \li QMetaType::LongLong
    \li QMetaType::ULongLong
    \li QMetaType::Float
    \li QMetaType::Double
    \li QMetaType::QChar
    \li QMetaType::QDate
    \li QMetaType::QTime
    \li QMetaType::QDateTime
    \li QMetaType::QString
    \endlist

    Any other type will be converted to a QString using
    QVariant::toString().

    Comparison of \l{QString}s is case sensitive by default; this can
    be changed using the \l {QSortFilterProxyModel::sortCaseSensitivity}
    {sortCaseSensitivity} property.

    By default, the Qt::DisplayRole associated with the
    \l{QModelIndex}es is used for comparisons. This can be changed by
    setting the \l {QSortFilterProxyModel::sortRole} {sortRole} property.

    \note The indices passed in correspond to the source model.

    \sa sortRole, sortCaseSensitivity, dynamicSortFilter
*/
/*
bool QSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    Q_D(const QSortFilterProxyModel);
    QVariant l = (source_left.model() ? source_left.model()->data(source_left, d->sort_role) : QVariant());
    QVariant r = (source_right.model() ? source_right.model()->data(source_right, d->sort_role) : QVariant());
    return QAbstractItemModelPrivate::isVariantLessThan(l, r, d->sort_casesensitivity, d->sort_localeaware);
}
*/
