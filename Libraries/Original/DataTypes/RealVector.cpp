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

#include "RealVector.hpp"

/*!
    \class As::RealVector

    \brief The RealVector class is a class that provides an array of the
    QVector<qreal> type, which is extended with additional functions.

    \inmodule DataTypes
*/

/*!
    Constructs an empty vector.
*/
As::RealVector::RealVector() :
    As::RealArray() {}

/*!
    Constructs a copy of \a other.
*/
As::RealVector::RealVector(const As::RealVector& other) :
    As::RealArray(other) {}

/*!
    Constructs a copy of \a other.
*/
As::RealVector::RealVector(const As::RealArray& other) :
    As::RealArray(other) {}

/*!
    Constructs a copy of \a other.
*/
As::RealVector::RealVector(const QVector<qreal>& other) :
    As::RealArray(other) {}

/*!
    Constructs a vector with an initial size of \a size elements.
    Each element is initialized with \a defaultValue.
*/
As::RealVector::RealVector(const int size,
                           const qreal defaultValue = 0.0) :
    As::RealArray(size, defaultValue) {}

/*!
    Sets the vector with elements stored in \a string.
*/
As::RealVector::RealVector(const QString& string) :
    As::RealArray(string) {}

/*!
    Destroys the vector.
*/
As::RealVector::~RealVector() {}

/*!
    Returns the smallest element of the vector.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.min(): 1.0
    \endcode
*/
qreal As::RealVector::min() const {
    AASSERT(size() > 0, QString("vector size = '%1', which is too small for this function").arg(size()));
    return *std::min_element(begin(), end()); }

/*!
    Returns the largest element of the vector.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.max(): 5.0
    \endcode
*/
qreal As::RealVector::max() const {
    AASSERT(size() > 0, QString("vector size = '%1', which is too small for this function").arg(size()));
    return *std::max_element(begin(), end()); }

/*!
    Returns the sum of the vector elements.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.sum(): 3.0 + 5.0 + 1.0 = 9.0
    \endcode
*/
qreal As::RealVector::sum() const {
    AASSERT(size() > 0, QString("vector size = '%1', which is too small for this function").arg(size()));
    return std::accumulate(begin(), end(), 0.0); }

/*!
    Returns the sum of the squared vector elements.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.sumSqr(): 9.0 + 25.0 + 1.0 = 35.0
    \endcode
*/
qreal As::RealVector::sumSqr() const {
    AASSERT(size() > 0, QString("vector size = '%1', which is too small for this function").arg(size()));
    qreal out = 0.0;

    for (const auto v : *this) {
        out += v * v; }

    return out; }

/*!
    Returns the mean (average) of the vector elements.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.mean(): 3.0
    \endcode
*/
qreal As::RealVector::mean() const {
    if (size() == 0) {
        return qQNaN(); }

    return sum() / size(); }

/*!
    Returns the range of the vector elements.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.range(): 4.0
    \endcode
*/
qreal As::RealVector::range() const {
    //AASSERT(this->size() > 0, "vector size is too small");
    return max() - min(); }

/*!
    Returns the middle point between the min and max vector elements.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.middle(): 2.0
    \endcode
*/
qreal As::RealVector::middle() const {
    //AASSERT(this->size() > 0, "vector size is too small");
    return (max() + min()) / 2.; }

/*!
    Returns the mean (average) step between the vector elements.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.step(): 2.0
    \endcode
*/
qreal As::RealVector::step() const {
    AASSERT(size() > 1, QString("vector size = '%1', which is too small for this function").arg(size()));
    return range() / (size() - 1); }

/*!
    Returns the index position of the first occurrence of the min() element in the vector,
    searching forward from the beginning.
*/
int As::RealVector::indexOfMin() const {
    AASSERT(size() > 0, QString("vector size = '%1', which is too small for this function").arg(size()));
    return indexOf(min()); }


/*!
    Returns the index position of the first occurrence of the max() element in the vector,
    searching forward from the beginning.
*/
int As::RealVector::indexOfMax() const {
    AASSERT(size() > 0, QString("vector size = '%1', which is too small for this function").arg(size()));
    return indexOf(max()); }

/*!
    Returns true if all the vector elements are 0; otherwise returns false.

    Example:
    \code
    // vector: [-3.0, 0.0, 3.0]
    // vector.isZero(): false
    \endcode
*/
bool As::RealVector::isZero() const {
    //AASSERT(this->size() > 0, "vector size is too small");
    if (size() == 0) {
        return false; }

    for (const auto& value : *this)
        if (value != 0.0) {
            return false; }

    return true; }

/*!
    Returns a vector with a reversed order of elements.

    Example:
    \code
    // vector: [3.0, 5.0, 1.0]
    // vector.reverse(): [1.0, 5.0, 3.0]
    \endcode
*/
As::RealVector As::RealVector::reverse() const {
    As::RealVector out;

    for (const auto& value : *this) {
        out.prepend(value); }

    return out; }

/*!
    Returns a vector with elements divided by the real value \a v.

    Example:
    \code
    // vector: [4.0, 9.0, 100.0]
    // v:  2.0
    // vector.normalizeBy(v): [2.0, 4.5, 50.0]
    \endcode
*/
As::RealVector As::RealVector::normalizeBy(const qreal v) const {
    AASSERT(v != 0.0, "dividing by zero");

    if (FuzzyCompareDouble(v, 1.0)) {
        return *this; }

    As::RealVector out;

    for (int i = 0; i < size(); ++i) {
        out.append(this->at(i) / v); }

    return out; }

/*!
    Returns a vector with elements divided by the respective elements of the \a other vector.

    Example:
    \code
    // vector: [4.0, 9.0, 100.0]
    // other:  [2.0, 2.0,  25.0]
    // vector.normalizeBy(other): [2.0, 4.5, 4.0]
    \endcode
*/
As::RealVector As::RealVector::normalizeBy(const As::RealVector& other) const {
    bool isEqSize = (size() == other.size());
    AASSERT(isEqSize, QString("vectors sizes are different: '%1' and '%2'").arg(size()).arg(other.size()));

    if (!isEqSize) {
        return *this; }

    As::RealVector out;

    for (int i = 0; i < size(); ++i) {
        out.append(this->at(i) / other[i]); }

    return out; }

/*!
    Returns a vector with elements equal to square root of the original vector elements.

    Example:
    \code
    // vector: [4.0, 9.0, 100.0]
    // vector.sqrt(): [2.0, 3.0, 10.0]
    \endcode
*/
As::RealVector As::RealVector::sqrt() const {
    As::RealVector out;

    for (const auto value : *this) {
        AASSERT(value >= 0.0, QString("negative vector element '%1' is found for square root").arg(value));
        out.append(qSqrt(value)); }

    return out; }

/*!
    Returns a vector with just a single element if all the vector elements are equal;
    otherwise returns the original vector.

    Example:
    \code
    // vector1: [3.0, 3.0, 3.0]
    // vector1.simplify(): [3.0]
    // vector2: [0.0, 3.0, 3.0]
    // vector2.simplify(): [0.0, 3.0, 3.0]
    \endcode
*/
As::RealVector As::RealVector::simplify() const {
    //AASSERT(this->size() > 0, "vector size is too small");
    for (int i = 0; i < this->size() - 1; ++i) {
        if (this->at(i) != this->at(i + 1)) {
            return *this; } }

    return As::RealVector(1, at(0)); }

/**
    Overloads operator<< for QDebug to accept RealVector output
*/
QDebug operator<<(QDebug debug, const As::RealVector& vector) {
    return QtPrivate::printSequentialContainer(debug, "As::RealVector", vector.toQVector()); }


