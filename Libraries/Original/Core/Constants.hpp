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

#ifndef AS_CONSTANTS_HPP
#define AS_CONSTANTS_HPP

class QColor;
class QString;

namespace As { //AS_BEGIN_NAMESPACE

// Internally defined enums

enum InputFileType
{
    UNKNOWN_FILE,
    NICOS_DAT,
    HEIDI_DAT,
    HEIDI_LOG,
    POLI_LOG,
    S6T2_DAT
};
//Q_ENUM_NS(InputFileType)

enum class OutputFileType // Currently not in use
{
    GENERAL_CSV,
    SHELX_INT_HKL,
    SHELX_REAL_HKL,
    TBAR_TB,
    CCSL_DAT,
    UMWEG_OBS
};
//Q_ENUM_NS(OutputFileType)

enum PlotType
{
    Raw,
    Integrated,
    Fitted,
    Excluded
};
//Q_ENUM_NS(PlotType)

enum GlobalColor
{
    black,

    white,
    whiteTransparent,

    red,
    redDark,
    redDarkVery,
    redDarkUltra,
    redLight,
    redLightVery,
    redLightUltra,
    redTransparent,
    redLightTransparent,

    green,
    greenDark,
    greenDarkVery,
    greenDarkUltra,
    greenLight,
    greenLightVery,
    greenLightUltra,
    greenTransparent,
    greenLightTransparent,

    blue,
    blueDark,
    blueDarkVery,
    blueDarkUltra,
    blueLight,
    blueLightVery,
    blueLightUltra,
    blueTransparent,
    blueLightTransparent,

    gray,
    grayDark,
    grayDarkVery,
    grayDarkUltra,
    grayLight,
    grayLightVery,
    grayLightUltra,
    grayTransparent,
    grayLightTransparent,

    yellow,
    yellowDark,
    yellowLight,
    yellowLightVery,

    orange,
    orangeLight
};
//Q_ENUM_NS(GlobalColor)

// Externally defined constants

extern const int APP_FULL_HEIGHT;
extern const int APP_FULL_WIDTH;
extern const int APP_SIDEBAR_WIDTH;
extern const int REDUCED_VERTICAL_SPACING;

extern const QColor SELECTION_BACKGROUND;
extern const QString DATE_TIME_FORMAT;

} //AS_END_NAMESPACE

#endif // AS_CONSTANTS_HPP
