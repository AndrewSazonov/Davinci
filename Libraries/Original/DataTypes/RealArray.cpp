/*
    Davinci, a software for the single-crystal diffraction data reduction.
    Copyright (C) 2015-2017 Andrew Sazonov

    This file is part of Davinci.

    Davinci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Davinci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Davinci.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtMath>

#include "Macros.hpp"
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
As::RealArray::RealArray(const As::RealArray& other) :
    m_array(other.m_array) {}

/*!
    Constructs a copy of \a other.
*/
As::RealArray::RealArray(const QVector<qreal>& other) :
    m_array(other) {}

/*!
    Constructs an array with an initial size of \a size elements.
    Each element is initialized with \a defaultValue.
*/
As::RealArray::RealArray(const int size,
                         const qreal defaultValue) :
    m_array(size, defaultValue) {}

/*!
    Sets the array with elements stored in \a string.
*/
As::RealArray::RealArray(const QString& string) {
    if (string.isEmpty()) {
        return; }

    QStringList list = string.split(QRegExp("\\s"), QString::SkipEmptyParts);
    bool ok;

    for (const QString& num : list) {
        m_array.append(num.toDouble(&ok));
        AASSERT(ok == true, "conversation of num to double fails"); } }

/*!
    Destroys the array.
*/
As::RealArray::~RealArray() {}

/*!
    Returns the element at index position \a i in the base array as a modifiable reference.

    \a i must be a valid index position in the array (i.e., 0 <= \a i < size()).

    Example:
    \code
    // array: [3.0, 5.0, 1.0]
    // array[0]: 3.0
    \endcode
*/
qreal& As::RealArray::operator[](const int i) {
    AASSERT(i >= 0 AND i < size(), "index out of range");
    return m_array[i]; }

/*!
    \overload

    Same as at(\a i).
*/
const qreal& As::RealArray::operator[](const int i) const {
    AASSERT(i >= 0 AND i < size(), "index out of range");
    return m_array[i]; }

/*!
    Assigns \a other to this array and returns a reference to this array.
*/
As::RealArray& As::RealArray::operator=(const As::RealArray& other) {
    if (this != &other) {
        m_array = other.m_array; }

    return *this; }

/*!
    Returns an array that contains all the items in this array followed by all the items
    in the \a other array.
*/
As::RealArray As::RealArray::operator+(const As::RealArray& other) const {
    return As::RealArray(m_array + other.m_array); }

/*!
    Returns \c true if \a other is equal to this array; otherwise returns \c false.
    Two arrays are considered equal if they contain the same values in the same order.
*/
bool As::RealArray::operator==(const As::RealArray& other) const {
    return (m_array == other.m_array); }

/*!
    Returns the element at index position \a i in the base array as a const reference.

    \a i must be a valid index position in the array (i.e., 0 <= \a i < size()).

    Example:
    \code
    // array: [3.0, 5.0, 1.0]
    // array[0]: 3.0
    \endcode

    \sa operator[]()
*/
const qreal& As::RealArray::at(const int i) const {
    AASSERT(i >= 0 AND i < size(), "index out of range");
    return m_array.at(i); }

/*!
    Returns the number of elements in the array.

    Example:
    \code
    // array: [3.0, 5.0, 1.0]
    // array.size(): 3
    \endcode
*/
int As::RealArray::size() const {
    return m_array.size(); }

/*!
    Returns the index position of the first occurrence of \a value in the vector.
    Returns -1 if no item matched.
*/
int As::RealArray::indexOf(const qreal value) const {
    return m_array.indexOf(value); }

/*!
    Inserts \a value at the end of the array.
*/
void As::RealArray::append(const qreal value) {
    m_array.append(value); }

/*!
    Returns \c true if the base array is empty; otherwise returns \c false.
*/
bool As::RealArray::isEmpty() const {
    return m_array.isEmpty(); }

/*!
    Returns the base array as QString with single spaces between the array elements.
*/
QString As::RealArray::toQString() const {
    QStringList list;

    for (const qreal value : m_array) {
        list.append(QString::number(value)); }

    return list.join(" "); }

/*!
    Returns the base array as QVector<qreal>.
*/
QVector<qreal> As::RealArray::toQVector() const {
    return m_array; }

/*!
    Sets the size of the array to \a size. If \a size is greater than the current size, elements
    are added to the end; the new elements are initialized with a default-constructed value. If
    size is less than the current size, elements are removed from the end.
*/
void As::RealArray::resize(const int size) {
    m_array.resize(size); }

/*!
    Returns a const pointer to the first item in the array.
*/
const qreal* As::RealArray::begin() const {
    return m_array.begin(); }

/*!
    Returns a const pointer to the imaginary item after the last item in the array.
*/
const qreal* As::RealArray::end() const {
    return m_array.end(); }

/*!
    Returns a sub-vector which contains elements from this vector, starting at position \a pos.
    If \a length is -1 (the default), all elements after \a pos are included; otherwise \a length
    elements (or all remaining elements if there are less than \a length elements) are included.
*/
As::RealArray As::RealArray::mid(const int pos,
                                 const int length) const {
    return As::RealArray(m_array.mid(pos, length)); }

/**
    Overloads operator<< for QDebug to accept the RealArray output.
*/
QDebug operator<<(QDebug debug, const As::RealArray& array) {
    return QtPrivate::printSequentialContainer(debug, "As::RealArray", array.toQVector()); }

