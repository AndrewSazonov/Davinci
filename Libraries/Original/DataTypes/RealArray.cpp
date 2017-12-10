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

#include "Functions.hpp"

#include "RealArray.hpp"

/*!
\class As::RealArray

\brief The RealArray class is a base class that provides an array of the
QVector<qreal> type, which takes just the required functions of QVector.

\inmodule DataTypes
*/

/*!
Constructs an empty array.
*/
As::RealArray::RealArray() {}

/*!
Constructs a copy of \a other.
*/
As::RealArray::RealArray(const As::RealArray &other) :
    m_array(other.m_array) {}

/*!
Constructs a copy of \a other.
*/
As::RealArray::RealArray(const QVector<qreal> &other) :
    m_array(other) {}

/*!
Constructs an array with an initial size of \a size elements.
Each element is initialized with \a defaultValue.
*/
As::RealArray::RealArray(const int size, const qreal defaultValue) :
    m_array(size, defaultValue) {}

/*!
Destroys the array.
*/
As::RealArray::~RealArray() {}

/*!
Sets the array with \a other.
*/
void As::RealArray::set(const QVector<qreal> &other)
{
    m_array = other;
}

/*!
Returns the element at index position \a i in the base array.

\a i must be a valid index position in the array (i.e., 0 <= \a i < size()).

Example:
\code
// array: [3.0, 5.0, 1.0]
// array[0]: 3.0
\endcode
*/
qreal &As::RealArray::operator[](const int i)
{
    //Q_ASSERT_X(i >= 0 AND i < size(), "As::RealArray::operator[]", "index out of range");
    return m_array[i];
}

/*!
\overload
*/
const qreal &As::RealArray::operator[](const int i) const
{
    //Q_ASSERT_X(i >= 0 AND i < size(), "As::RealArray::operator[]", "index out of range");
    return m_array.begin()[i];
}

/*!
Assigns \a other to this array and returns a reference to this array.
*/
QVector<qreal> &As::RealArray::operator=(const QVector<qreal> &other)
{
    m_array = other;
    return m_array;
}

/*!
Returns an array that contains sum of the items from this array and the \a other array.
*/
As::RealArray As::RealArray::operator+(const As::RealArray &other) const
{
    As::RealArray out = m_array;
    for (int i = 0; i < out.size(); ++i)
        out[i] += other[i];
    return out;
}

/*!
Returns \c true if \a other is equal to this array; otherwise returns \c false.
Two arrays are considered equal if they contain the same values in the same order.
*/
bool As::RealArray::operator==(const As::RealArray &other) const
{
    return m_array == other.m_array;
}

/*!
Returns the number of elements in the array.

Example:
\code
// array: [3.0, 5.0, 1.0]
// array.size(): 3
\endcode
*/
int As::RealArray::size() const
{
    return m_array.size();
}

/*!
Returns the index position of the first occurrence of \a value in the vector.
Returns -1 if no item matched.
*/
int As::RealArray::indexOf(const qreal value) const
{
    return m_array.indexOf(value);
}

/*!
Inserts \a value at the end of the array.
*/
void As::RealArray::append(const qreal value)
{
    m_array.append(value);
}

/*!
Inserts \a value at the beginning of the array.
*/
void As::RealArray::prepend(const qreal value)
{
    m_array.prepend(value);
}

/*!
Returns \c true if the base array is empty; otherwise returns \c false.
*/
bool As::RealArray::isEmpty() const
{
    return m_array.isEmpty();
}

/*!
Returns the base array as QString with single spaces between the numbers.
*/
QString As::RealArray::toQString()
{
    QStringList list;
    for (const qreal value : m_array)
        list.append(QString::number(value));
    return list.join(" ");
}

/*!
\overload
*/
const QString As::RealArray::toQString() const
{
    QStringList list;
    for (const qreal value : m_array)
        list.append(QString::number(value));
    return list.join(" ");
}

/*!
Returns the base array as QVector<qreal>.
*/
const QVector<qreal> As::RealArray::toQVector() const
{
    return m_array;
}

/*!
Sets the size of the array to \a size. If \a size is greater than the current size, elements
are added to the end; the new elements are initialized with a default-constructed value. If
size is less than the current size, elements are removed from the end.
*/
void As::RealArray::resize(const int size)
{
    m_array.resize(size);
}

/*!
Returns a const pointer to the first item in the array.
*/
const qreal *As::RealArray::begin() const
{
    return m_array.begin();
}

/*!
Returns a const pointer to the imaginary item after the last item in the array.
*/
const qreal *As::RealArray::end() const
{
    return m_array.end();
}

/*!
Returns a sub-vector which contains elements from this vector, starting at position \a pos.
If \a length is -1 (the default), all elements after \a pos are included; otherwise \a length
elements (or all remaining elements if there are less than \a length elements) are included.
 */
QVector<qreal> As::RealArray::mid(const int pos, const int length) const
{
    return m_array.mid(pos, length);
}

/**
Overloads operator<< for QDebug to accept the RealArray output.
*/
QDebug operator<<(QDebug debug, const As::RealArray &other)
{
    //QDebugStateSaver saver(debug);
    return QtPrivate::printSequentialContainer(debug, "As::RealArray", other.toQVector());
}

