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

#include "RealMatrix9.hpp"

/*!
    \class As::RealMatrix9

    \brief The RealMatrix9 class is a class that provides an array of the
    QVector<qreal> type with just 9 elements (corresponds to the 3x3 array),
    which is extended with additional functions.

    \inmodule DataTypes
*/

/*!
    Constructs a matrix of 9 elements, which corresponds to the 3x3 identity matrix.
*/
As::RealMatrix9::RealMatrix9() :
    RealArray({ 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 }) {}

/*!
    Constructs a copy of \a other.
*/
As::RealMatrix9::RealMatrix9(const As::RealMatrix9& other) :
    As::RealArray(other) {}

/*!
    Constructs a matrix and initialize it with \a a1, \a a2, \a a3, \a b1,
    \a b2, \a b3, \a c1, \a c2 and \a c3.

    \note Number of the input parameters must be equal to 9.
*/
As::RealMatrix9::RealMatrix9(const qreal a1, const qreal a2, const qreal a3,
                             const qreal b1, const qreal b2, const qreal b3,
                             const qreal c1, const qreal c2, const qreal c3) :
    RealArray({ a1, a2, a3, b1, b2, b3, c1, c2, c3 }) {}

/*!
    Constructs a matrix and initialize it with elements stored in \a string.

    \note Number of the elements in \a string must be equal to 9.
*/
As::RealMatrix9::RealMatrix9(const QString& string) :
    As::RealArray(string) {
    resize(9); }

/*!
    Destroys the matrix.
*/
As::RealMatrix9::~RealMatrix9() {}

/*!
    Returns the determinant of a matrix.

    Example:
    \code
    // matrix: (2., 1., -2.)
    //         (3., 2.,  4.)
    //         (2., 1., -1.)
    // matrix.det(): 1.
    \endcode

    \sa \link https://en.wikipedia.org/wiki/Determinant Wiki: Determinant \endlink
*/
qreal As::RealMatrix9::det() const {
    const As::RealArray& m = *this;

    return m[0] * (m[4] * m[8] - m[7] * m[5]) -
           m[1] * (m[3] * m[8] - m[5] * m[6]) +
           m[2] * (m[3] * m[7] - m[4] * m[6]); }

/*!
    Returns a transposed matrix obtained by reflecting the elements along its main diagonal.

    \sa \link https://en.wikipedia.org/wiki/Transpose Wiki: Transpose \endlink
*/
const As::RealMatrix9 As::RealMatrix9::trans() const {
    const As::RealArray& m = *this;

    return RealMatrix9{ m[0], m[3], m[6],
                        m[1], m[4], m[7],
                        m[2], m[5], m[8] }; }

/*!
    Returns an inversed matrix if det() is not zero.

    \sa \link https://en.wikipedia.org/wiki/Invertible_matrix Wiki: Invertible matrix \endlink
*/
const As::RealMatrix9 As::RealMatrix9::inv() const {
    const qreal d = det();
    AASSERT(d != 0.0, "determinant is zero");

    const As::RealArray& m = *this;

    const qreal a1 = (m[4] * m[8] - m[7] * m[5]) / d;
    const qreal a2 = (m[2] * m[7] - m[1] * m[8]) / d;
    const qreal a3 = (m[1] * m[5] - m[2] * m[4]) / d;

    const qreal b1 = (m[5] * m[6] - m[3] * m[8]) / d;
    const qreal b2 = (m[0] * m[8] - m[2] * m[6]) / d;
    const qreal b3 = (m[3] * m[2] - m[0] * m[5]) / d;

    const qreal c1 = (m[3] * m[7] - m[6] * m[4]) / d;
    const qreal c2 = (m[6] * m[1] - m[0] * m[7]) / d;
    const qreal c3 = (m[0] * m[4] - m[3] * m[1]) / d;

    return RealMatrix9{ a1, a2, a3, b1, b2, b3, c1, c2, c3 }; }

/*!
    Returns a matrix with columns normalized to a length of 1.
*/
const As::RealMatrix9 As::RealMatrix9::normColumns() const {
    const As::RealArray& m = *this;

    // Normalization coefficients for every row
    const qreal k1 = qSqrt( As::Sqr(m[0]) + As::Sqr(m[3]) + As::Sqr(m[6]) );
    const qreal k2 = qSqrt( As::Sqr(m[1]) + As::Sqr(m[4]) + As::Sqr(m[7]) );
    const qreal k3 = qSqrt( As::Sqr(m[2]) + As::Sqr(m[5]) + As::Sqr(m[8]) );

    AASSERT(k1 != 0.0, "normalization of 1st matrix column fails (all elements are zeros)");
    AASSERT(k2 != 0.0, "normalization of 2nd matrix column fails (all elements are zeros)");
    AASSERT(k3 != 0.0, "normalization of 3rd matrix column fails (all elements are zeros)");

    return RealMatrix9{ m[0] / k1, m[1] / k2, m[2] / k3,
                        m[3] / k1, m[4] / k2, m[5] / k3,
                        m[6] / k1, m[7] / k2, m[8] / k3 }; }

/*!
    Returns a matrix with rows normalized to a length of 1.

    Example:
    \code
    // matrix: (1., 2., 0.)
    //         (3., 4., 0.)
    //         (1., 1., 1.)
    // matrix.normRows(): (0.4472, 0.8944, 0.0000)
    //                    (0.6000, 0.8000, 0.0000)
    //                    (0.5774, 0.5774, 0.5774)
    \endcode
*/
const As::RealMatrix9 As::RealMatrix9::normRows() const {
    const As::RealArray& m = *this;

    // Normalization coefficients for every row
    const qreal k1 = qSqrt( As::Sqr(m[0]) + As::Sqr(m[1]) + As::Sqr(m[2]) );
    const qreal k2 = qSqrt( As::Sqr(m[3]) + As::Sqr(m[4]) + As::Sqr(m[5]) );
    const qreal k3 = qSqrt( As::Sqr(m[6]) + As::Sqr(m[7]) + As::Sqr(m[8]) );

    AASSERT(k1 != 0.0, "normalization of 1st matrix row fails (all elements are zeros)");
    AASSERT(k2 != 0.0, "normalization of 2nd matrix row fails (all elements are zeros)");
    AASSERT(k3 != 0.0, "normalization of 3rd matrix row fails (all elements are zeros)");

    return RealMatrix9{ m[0] / k1, m[1] / k1, m[2] / k1,
                        m[3] / k2, m[4] / k2, m[5] / k2,
                        m[6] / k3, m[7] / k3, m[8] / k3 };

}

/**
    Overloads operator<< for QDebug to accept RealMatrix9 output
*/
QDebug operator<<(QDebug debug, const As::RealMatrix9& matrix) {
    return QtPrivate::printSequentialContainer(debug, "As::RealMatrix9", matrix.toQVector()); }

