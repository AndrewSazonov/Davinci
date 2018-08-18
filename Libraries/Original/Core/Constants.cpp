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

#include <QColor>
#include <QHash>
#include <QString>
#include <QStringList>

#include "ScanDict.hpp"

#include "Constants.hpp"

// Alias names

/*!
\typedef As::ScanSection_t

Synonym for QMap<QString, QMap<QString, QString> >.
*/

/*!
\typedef As::Scan_t

Synonym for QMap<QString, QMap<QString, QMap<QString, QString> > >.
*/

// Externally defined constants

/*!
\variable As::APP_FULL_HEIGHT
\brief the default full height of the application window.
*/
const int As::APP_FULL_HEIGHT = 900;

/*!
\variable As::APP_FULL_WIDTH
\brief the default full width of the application window.
*/
const int As::APP_FULL_WIDTH = 600;

/*!
\variable As::APP_SIDEBAR_WIDTH
\brief the width of the application sidebar.
*/
const int As::APP_SIDEBAR_WIDTH = 300; //settings.value("Sidebar/width", 300).toInt();

/*!
\variable As::UI_VERTICAL_SPACE_ROWS
\brief the...
*/
const int As::UI_VERTICAL_SPACE_ROWS = 7;

/*!
\variable As::SELECTION_BACKGROUND
\brief the color of the selection background.
*/
const QColor As::SELECTION_BACKGROUND("#f8f8f8");

/*!
\variable As::DATE_TIME_FORMAT
\brief the data and time format.
*/
//const QString As::DATE_TIME_FORMAT("yyyy/MM/dd hh:mm");
//const QString As::DATE_TIME_FORMAT("dd-MM-yyyy hh:mm");
const QString As::DATE_TIME_FORMAT("yyyy-MM-dd hh:mm");



// Internally defined enums

/*!
\enum As::InputFileType

This enum type describes types of the input files.

\value UNKNOWN_FILE     File type is unknown
\value NICOS_DAT        NICOS data
\value HEIDI_DAT        HEiDi diffractometer data collection
\value HEIDI_LOG        HEiDi diffractometer DIF4 log
\value POLI_LOG         POLI diffractometer log
\value S6T2_DAT         Super 6T2 diffractometer data collection
*/

/*!
\enum As::OutputFileType

This enum type describes types of the output files.

\value GENERAL_CSV      General comma separated
\value SHELX_INT_HKL    ShelX with direction cosines. Integer HKLs. Format: 3i4, 2f8.2, i4, 6f8.5
\value SHELX_REAL_HKL   ShelX with direction cosines. Real HKLs. Format: 3f8.3, 2f8.2, i4, 6f8.5
\value TBAR_TB          Tbar
\value CCSL_DAT         CCSL
\value UMWEG_OBS        Umweg
*/

/*!
\enum As::PlotType

This enum type describes types of the plots.

\value Raw              Raw
\value Integrated       Integrated
\value Fitted           Fitted
\value Excluded         Excluded
*/

/*!
\enum As::GlobalColor

This enum type describes types of colors.

\value black                  Black

\value white                  White
\value whiteTransparent       Transparen white

\value red                    Red
\value redDark                Dark red
\value redDarkVery            Very dark red
\value redDarkUltra           Ultra dark red
\value redLight               Light red
\value redLightVery           Very light red
\value redLightUltra          Ultra light red
\value redTransparent         Transparen red
\value redLightTransparent    Transparent light red

\value green                  Green
\value greenDark              Dark green
\value greenDarkVery          Very dark green
\value greenDarkUltra         Ultra dark green
\value greenLight             Light green
\value greenLightVery         Very light green
\value greenLightUltra        Ultra light green
\value greenTransparent       Transparen green
\value greenLightTransparent  Transparent light green

\value blue                   Blue
\value blueDark               Dark blue
\value blueDarkVery           Very dark blue
\value blueDarkUltra          Ultra dark blue
\value blueLight              Light blue
\value blueLightVery          Very light blue
\value blueLightUltra         Ultra light blue
\value blueTransparent        Transparen blue
\value blueLightTransparent   Transparent light blue

\value gray                   Gray
\value grayDark               Dark gray
\value grayDarkVery           Very dark gray
\value grayDarkUltra          Ultra dark gray
\value grayLight              Light gray
\value grayLightVery          Very light gray
\value grayLightUltra         Ultra light gray
\value grayTransparent        Transparen gray
\value grayLightTransparent   Transparent light gray

\value yellow                 Yellow
\value yellowDark             Dark yellow
\value yellowLight            Light yellow
\value yellowLightVery        Very light yellow

\value orange                 Orange
\value orangeLight            Light orange
*/


