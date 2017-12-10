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

#ifndef AS_COLORS_HPP
#define AS_COLORS_HPP

#include "Constants.hpp"

#include <QColor>

namespace As { //AS_BEGIN_NAMESPACE

class Color : public QColor
{

public:
    Color();
    Color(const QColor &color);
    Color(const Color &color);
    Color(const int r,
          const int g,
          const int b,
          const int a = 255);
    Color(As::GlobalColor color);

    ~Color();

    Color lighter(const int factor) const;
    Color darker(const int factor) const;
    Color transparenter(const int alpha) const;

};

} //AS_END_NAMESPACE

#endif // AS_COLORS_HPP


