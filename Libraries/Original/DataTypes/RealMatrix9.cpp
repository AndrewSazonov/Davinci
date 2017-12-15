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
#include "Macros.hpp"

#include "RealMatrix9.hpp"

/*!
\class As::RealMatrix9

\brief The RealMatrix9 class is a class that provides an array of the
QVector<qreal> type with just 9 elements (corresponds to the 3x3 array),
which is extended with additional functions.

\inmodule DataTypes
*/

/*!
Constructs a matrix of 9 elements (3x3 array) and initialize it with zeros.
*/
As::RealMatrix9::RealMatrix9()
{
    resize(9);
}

/*!
Constructs a matrix and initialize it with \a a1, \a a2, \a a3, \a b1,
\a b2, \a b3, \a c1, \a c2 and \a c3.

\note Number of the input parameters must be equal to 9.
*/
As::RealMatrix9::RealMatrix9(const qreal a1, const qreal a2, const qreal a3,
                             const qreal b1, const qreal b2, const qreal b3,
                             const qreal c1, const qreal c2, const qreal c3)
{
    resize(9);
    set(a1, a2, a3, b1, b2, b3, c1, c2, c3);
}

/*!
Constructs a matrix and initialize it with elements stored in \a string.

\note Number of the elements in \a string must be equal to 9.
*/
As::RealMatrix9::RealMatrix9(const QString &string)
{
    resize(9);
    set(string);
}

/*!
Destroys the matrix.
*/
As::RealMatrix9::~RealMatrix9() {}

/*!
Sets the array with \a other.

\note Number of the elements in \a other must be equal to 9.
*/
void As::RealMatrix9::set(const QVector<qreal> &other)
{
    Q_ASSERT_X(false, AFUNC, "matrix size is not 9");
    As::RealArray::set(other);
}

/*!
Sets the array with elements stored in \a string.

\note Number of the elements in \a string must be equal to 9.
*/
void As::RealMatrix9::set(const QString &string)
{
    QStringList list = string.split(QRegExp("\\s"), QString::SkipEmptyParts);
    QVector<qreal> vector;
    bool ok;
    for (const QString &num : list) {
        vector << num.toDouble(&ok);
        Q_ASSERT_X(ok == true, AFUNC, "conversation of num to double fails"); }
    Q_ASSERT_X(vector.size() == 9, AFUNC, "vector size is not 9");
    As::RealArray::set(vector);
}

/*!
\overload

Sets the matrix with \a a1, \a a2, \a a3, \a b1, \a b2, \a b3, \a c1, \a c2 and \a c3.

\note Number of the input parameters must be equal to 9.
*/
void As::RealMatrix9::set(const qreal a1, const qreal a2, const qreal a3,
                          const qreal b1, const qreal b2, const qreal b3,
                          const qreal c1, const qreal c2, const qreal c3)
{
    QVector<qreal> v{a1, a2, a3, b1, b2, b3, c1, c2, c3};
    As::RealArray::set(v);
}

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
qreal As::RealMatrix9::det() const
{
    As::RealArray m(*this);
    return m[0] * (m[4] * m[8] - m[7] * m[5]) -
           m[1] * (m[3] * m[8] - m[5] * m[6]) +
           m[2] * (m[3] * m[7] - m[4] * m[6]);
}

/*!
Returns a transposed matrix obtained by reflecting the elements along its main diagonal.

\sa \link https://en.wikipedia.org/wiki/Transpose Wiki: Transpose \endlink
*/
const As::RealMatrix9 As::RealMatrix9::trans() const
{
    As::RealArray m(*this);
    return RealMatrix9 {m[0], m[3], m[6],
                        m[1], m[4], m[7],
                        m[2], m[5], m[8]};
}

/*!
Returns an inversed matrix if det() is not zero.

\sa \link https://en.wikipedia.org/wiki/Invertible_matrix Wiki: Invertible matrix \endlink
*/
const As::RealMatrix9 As::RealMatrix9::inv() const
{
    Q_ASSERT_X(det() != 0., AFUNC, "determinant is zero");
    As::RealArray m(*this);
    const qreal a1 = (m[4] * m[8] - m[7] * m[5]) / det();
    const qreal a2 = (m[2] * m[7] - m[1] * m[8]) / det();
    const qreal a3 = (m[1] * m[5] - m[2] * m[4]) / det();
    const qreal b1 = (m[5] * m[6] - m[3] * m[8]) / det();
    const qreal b2 = (m[0] * m[8] - m[2] * m[6]) / det();
    const qreal b3 = (m[3] * m[2] - m[0] * m[5]) / det();
    const qreal c1 = (m[3] * m[7] - m[6] * m[4]) / det();
    const qreal c2 = (m[6] * m[1] - m[0] * m[7]) / det();
    const qreal c3 = (m[0] * m[4] - m[3] * m[1]) / det();
    return RealMatrix9{a1, a2, a3, b1, b2, b3, c1, c2, c3};
}

/**
Overloads operator<< for QDebug to accept RealMatrix9 output
*/
QDebug operator<<(QDebug debug, const As::RealMatrix9 &other)
{
    //QDebugStateSaver saver(debug);
    return QtPrivate::printSequentialContainer(debug, "As::RealMatrix9", other.toQVector());
}

