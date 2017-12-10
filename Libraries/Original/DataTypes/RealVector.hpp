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

#ifndef AS_DATATYPES_REALVECTOR_HPP
#define AS_DATATYPES_REALVECTOR_HPP

#include <QDebug>
#include <QtGlobal>

#include "RealArray.hpp"

namespace As { //AS_BEGIN_NAMESPACE

class RealVector : public RealArray
{

public:
    RealVector();
    RealVector(const As::RealVector &other);
    RealVector(const As::RealArray &other);
    RealVector(const QVector<qreal> &other);
    RealVector(const int size, const qreal defaultValue);
    RealVector(const QString &string);

    virtual ~RealVector();

    using RealArray::operator=;
    //using RealArray::operator+;

    qreal min() const;
    qreal max() const;
    qreal sum() const;
    qreal sumSqr() const;
    qreal mean() const;
    qreal range() const;
    qreal middle() const;
    qreal step() const;
    int indexOfMax() const;
    int indexOfMin() const;
    bool isZero() const;
    RealVector reverse() const;
    RealVector normalizeBy(const As::RealVector &other) const;
    const RealVector sqrt() const;
    RealVector simplified() const;

};

} //AS_END_NAMESPACE

QDebug operator<<(QDebug debug, const As::RealVector &other);

Q_DECLARE_METATYPE(As::RealVector) // To use it as a custom type in QVariant.

#endif // AS_DATATYPES_REALVECTOR_HPP
