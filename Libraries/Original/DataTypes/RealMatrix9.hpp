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

#ifndef AS_DATATYPES_REALMATRIX9_HPP
#define AS_DATATYPES_REALMATRIX9_HPP

#include <QDebug>
#include <QtGlobal>

#include "RealArray.hpp"

namespace As { //AS_BEGIN_NAMESPACE

class RealMatrix9 : public RealArray {

  public:
    // contructors and destructor
    RealMatrix9();
    RealMatrix9(const As::RealMatrix9& other);
    RealMatrix9(const qreal a1, const qreal a2, const qreal a3,
                const qreal b1, const qreal b2, const qreal b3,
                const qreal c1, const qreal c2, const qreal c3);
    RealMatrix9(const QString& string);
    virtual ~RealMatrix9() Q_DECL_OVERRIDE;

    // methods
    qreal det() const;
    const RealMatrix9 trans() const;
    const RealMatrix9 inv() const;
    const RealMatrix9 normRows() const;
    const RealMatrix9 normColumns() const;

};

} //AS_END_NAMESPACE

QDebug operator<<(QDebug debug, const As::RealMatrix9& matrix);

Q_DECLARE_METATYPE(As::RealMatrix9) // To use it as a custom type in QVariant.

#endif // AS_DATATYPES_REALMATRIX9_HPP
