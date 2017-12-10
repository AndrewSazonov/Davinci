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

#include "Colors.hpp"

/*!
\class As::Color

\brief The Color is a class that is based on the QColor class.

It has its own color scheme and some additional methods.

\inmodule Core
*/

/*!
Constructs an invalid color with the RGB value (0, 0, 0). An invalid color is a color
that is not properly set up for the underlying window system.

The alpha value of an invalid color is unspecified.
*/
As::Color::Color() {}

/*!
Constructs a color that is a copy of QColor \a color.
*/
As::Color::Color(const QColor &color)
    : QColor(color) {}

/*!
Constructs a color that is a copy of As::Color \a color.
*/
As::Color::Color(const Color &color)
    : QColor(color) {}

/*!
Constructs a color with the RGB value \a r, \a g, \a b, and the
alpha-channel (transparency) value of \a a.

The color is left invalid if any of the arguments are invalid.
*/
As::Color::Color(const int r,
                 const int g,
                 const int b,
                 const int a)
    : QColor(r, g, b, a) {}

/*!
Constructs a color with a color value of enum As::GlobalColor \a color.
*/
As::Color::Color(GlobalColor color)
{
    const Color red    (253,  93,  91);
    const Color green  (102, 153, 102);
    const Color blue   ( 79, 159, 251);
    const Color gray   (128, 128, 128);
    const Color yellow (254, 251,   0);
    const Color orange (253, 187,  68);
    const Color white  (255, 255, 255);
    const Color black  (  0,   0,   0);

    switch (color) {

    case As::black:                 setRgba(black.rgba()); break;

    case As::white:                 setRgba(white.rgba()); break;
    case As::whiteTransparent:      setRgba(white.transparenter(60).rgba()); break;

    case As::red:                   setRgba(red.rgba()); break;
    case As::redDark:               setRgba(red.darker(120).rgba()); break;
    case As::redDarkVery:           setRgba(red.darker(140).rgba()); break;
    case As::redDarkUltra:          setRgba(red.darker(160).rgba()); break;
    case As::redLight:              setRgba(red.lighter(120).rgba()); break;
    case As::redLightVery:          setRgba(red.lighter(140).rgba()); break;
    case As::redLightUltra:         setRgba(red.lighter(160).rgba()); break;
    case As::redTransparent:        setRgba(red.transparenter(220).rgba()); break;
    case As::redLightTransparent:   setRgba(red.lighter(120).transparenter(40).rgba()); break;

    case As::green:                 setRgba(green.rgba()); break;
    case As::greenDark:             setRgba(green.darker(120).rgba()); break;
    case As::greenDarkVery:         setRgba(green.darker(140).rgba()); break;
    case As::greenDarkUltra:        setRgba(green.darker(160).rgba()); break;
    case As::greenLight:            setRgba(green.lighter(120).rgba()); break;
    case As::greenLightVery:        setRgba(green.lighter(140).rgba()); break;
    case As::greenLightUltra:       setRgba(green.lighter(160).rgba()); break;
    case As::greenTransparent:      setRgba(green.transparenter(220).rgba()); break;
    case As::greenLightTransparent: setRgba(green.lighter(120).transparenter(60).rgba()); break;

    case As::blue:                  setRgba(blue.rgba()); break;
    case As::blueDark:              setRgba(blue.darker(120).rgba()); break;
    case As::blueDarkVery:          setRgba(blue.darker(140).rgba()); break;
    case As::blueDarkUltra:         setRgba(blue.darker(160).rgba()); break;
    case As::blueLight:             setRgba(blue.lighter(120).rgba()); break;
    case As::blueLightVery:         setRgba(blue.lighter(140).rgba()); break;
    case As::blueLightUltra:        setRgba(blue.lighter(160).rgba()); break;
    case As::blueTransparent:       setRgba(blue.transparenter(220).rgba()); break;
    case As::blueLightTransparent:  setRgba(blue.lighter(120).transparenter(40).rgba()); break;

    case As::gray:                  setRgba(gray.rgba()); break;
    case As::grayDark:              setRgba(gray.darker(130).rgba()); break;
    case As::grayDarkVery:          setRgba(gray.darker(160).rgba()); break;
    case As::grayDarkUltra:         setRgba(gray.darker(190).rgba()); break;
    case As::grayLight:             setRgba(gray.lighter(130).rgba()); break;
    case As::grayLightVery:         setRgba(gray.lighter(160).rgba()); break;
    case As::grayLightUltra:        setRgba(gray.lighter(190).rgba()); break;
    case As::grayTransparent:       setRgba(gray.transparenter(220).rgba()); break;
    case As::grayLightTransparent:  setRgba(gray.lighter(130).transparenter(60).rgba()); break;

    case As::yellow:                setRgba(yellow.rgba()); break;
    case As::yellowDark:            setRgba(yellow.darker(130).rgba()); break;
    case As::yellowLight:           setRgba(yellow.lighter(130).rgba()); break;
    case As::yellowLightVery:       setRgba(yellow.lighter(160).rgba()); break;

    case As::orange:                setRgba(orange.rgba()); break;
    case As::orangeLight:           setRgba(orange.lighter(130).rgba()); break;

    default:                        setRgba(black.rgba()); break;

    }
}

/*!
Destroys the color.
*/
As::Color::~Color() {}

/*!
Returns a more transparent color, but does not change this object.

The alpha of the return color is set to \a alpha. Integer \a alpha is
specified in the range 0-255.
*/
As::Color As::Color::transparenter(const int alpha) const
{
    As::Color out(*this);
    out.setAlpha(alpha);
    return out;
}

/*!
Returns a lighter (or darker) color, but does not change this object.

If the \a factor is greater than 100, this functions returns a lighter color.
Setting \a factor to 150 returns a color that is 50% brighter. If the \a factor
is less than 100, the return color is darker, but it is recommend to use the
darker() function for this purpose. If the \a factor is 0 or negative, the return
value is unspecified.
*/
As::Color As::Color::lighter(const int factor) const
{
    return As::Color(QColor::lighter(factor));
}

/*!
Returns a darker (or lighter) color, but does not change this object.

If the \a factor is greater than 100, this functions returns a darker color.
Setting \a factor to 300 returns a color that has one-third the brightness. If
the \a factor is less than 100, the return color is lighter, but we recommend
using the lighter() function for this purpose. If the \a factor is 0 or negative,
the return value is unspecified.
*/
As::Color As::Color::darker(const int factor) const
{
    return As::Color(QColor::darker(factor));
}
