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

#include <QtMath>

#include "Functions.hpp"

#include "Line.hpp"

/*!
\class As::Line

\brief The Line class provides a geometrical line build by 2 points with given coordinates.

\inmodule DataTypes
*/

/*!
Constructs a default line.
*/
As::Line::Line() {}

/*!
Constructs a line based on the coordinates of two points (\a x1, \a y1) and (\a x2, \a y2).
The respective estimated standard deviation (ESD) of the coordinates (\a esdX1, \a esdY1) and
(\a esdX2, \a esdY2) should also be given if differ from 0.
*/
As::Line::Line(const qreal x1,
               const qreal y1,
               const qreal x2,
               const qreal y2,
               const qreal esdX1,
               const qreal esdY1,
               const qreal esdX2,
               const qreal esdY2) :
    m_x1(x1),
    m_y1(y1),
    m_x2(x2),
    m_y2(y2),
    m_esdX1(esdX1),
    m_esdY1(esdY1),
    m_esdX2(esdX2),
    m_esdY2(esdY2) {}

/*!
Destroys the array.
*/
As::Line::~Line() {}

/*!
Returns the calculated x coordinate for the given \a y coordinate.
*/
qreal As::Line::xForY(const qreal y) const
{
    /// Add assert!
    // y = mx + b;
    // m = (y2 - y1) / (x2 - x1)
    // b = y2 - mx2
    // x = (y - b) / m = (y - y2) * (x2 - x1) / (y2 - y1) + x2
    if (m_y2 == m_y1)
        return m_x1;
    return (y - m_y2) * (m_x2 - m_x1) / (m_y2 - m_y1) + m_x2;
}

/*!
Returns the calculated y ESD for the given \a y coordinate.
*/
qreal As::Line::esdYForY(const qreal y) const
{
    if (m_y2 == m_y1)
        return m_esdY1;
    const qreal coeffYtoY2 = (m_y2 - y) / (m_y2 - m_y1);
    const qreal coeffY1toY = (y - m_y1) / (m_y2 - m_y1);
    return qSqrt(As::Sqr(m_esdY1) * coeffYtoY2 +
                 As::Sqr(m_esdY2) * coeffY1toY);
}

/*!
Returns the calculated x ESD for the given \a y coordinate and its \a esdY ESD.
*/
qreal As::Line::esdXForY(const qreal y, const qreal esdY) const
{
    // x is defined in the xForY function
    // sx = sqrt((dFy * esdY)^2 + ...)
    if (m_y2 == m_y1)
        return m_esdX1;
    const qreal dFy  =  (m_x2 - m_x1) / (m_y2 - m_y1);
    const qreal dFx1 = -(y    - m_y2) / (m_y2 - m_y1);
    const qreal dFx2 =  (y    - m_y1) / (m_y2 - m_y1);
    const qreal dFy1 =  (y    - m_y2) * (m_x2 - m_x1) / As::Sqr(m_y2 - m_y1);
    const qreal dFy2 = -(y    - m_y1) * (m_x2 - m_x1) / As::Sqr(m_y2 - m_y1);
    return qSqrt(As::Sqr(dFy  * esdY) +
                 As::Sqr(dFx1 * m_esdX1) +
                 As::Sqr(dFx2 * m_esdX2) +
                 As::Sqr(dFy1 * m_esdY1) +
                 As::Sqr(dFy2 * m_esdY2));
}

