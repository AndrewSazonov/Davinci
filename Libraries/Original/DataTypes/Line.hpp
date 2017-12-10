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

#ifndef AS_DATATYPES_LINE_HPP
#define AS_DATATYPES_LINE_HPP

#include <QtGlobal>

namespace As { //AS_BEGIN_NAMESPACE

class Line
{

public:
    Line();
    Line(const qreal x1,
         const qreal y1,
         const qreal x2,
         const qreal y2,
         const qreal esdX1 = 0.,
         const qreal esdY1 = 0.,
         const qreal esdX2 = 0.,
         const qreal esdY2 = 0.);

    virtual ~Line();

    qreal xForY(const qreal y) const;
    qreal esdYForY(const qreal y) const;
    qreal esdXForY(const qreal y, const qreal esdY) const;

private:
    qreal m_x1 = 0.;
    qreal m_y1 = 0.;
    qreal m_x2 = 0.;
    qreal m_y2 = 0.;
    qreal m_esdX1 = 0.;
    qreal m_esdY1 = 0.;
    qreal m_esdX2 = 0.;
    qreal m_esdY2 = 0.;

};

} //AS_END_NAMESPACE

#endif // AS_DATATYPES_LINE_HPP
