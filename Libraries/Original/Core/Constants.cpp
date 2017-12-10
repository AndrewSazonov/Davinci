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
#include <QElapsedTimer>
#include <QHash>
#include <QString>
#include <QStringList>

#include "ScanDatabase.hpp"

#include "Constants.hpp"

// Externally defined constants or variables

/*!
\variable As::elapsedTimer
\brief the stopwatch displaying the elapsed time.
*/
QElapsedTimer As::elapsedTimer;

/*!
\variable As::debugCount
\brief the index of the debug message.
*/
int As::debugCount = 0;

/*!
\variable As::SELECTION_BACKGROUND
\brief the color of the selection background.
*/
const QColor As::SELECTION_BACKGROUND("#f8f8f8");

/*!
\variable As::DATA_TIME_FORMAT
\brief the data and time format.
*/
const QString As::DATA_TIME_FORMAT("yyyy/MM/dd hh:mm");

/*!
\variable As::COUNT_TYPES
\brief the list of count types (unpolarised, spin up, spin down).
*/
const QStringList As::COUNT_TYPES({"", "(+)", "(-)"});

/*!
\variable As::MC_CANDLISH_FACTOR
\brief the McCandlish factor hash.
*/
const QHash<int, double> As::MC_CANDLISH_FACTOR
{
    {As::UNKNOWN_FILE,  0.},
    {As::NICOS_DAT,     0.},
    {As::HEIDI_DAT,     2.5e-2},
    {As::HEIDI_LOG,     2.5e-2},
    {As::POLI_LOG,      0.},
    {As::S6T2_DAT,      0.}
};

/*!
\variable As::SCAN_DATABASE
\brief the version of the Macintosh operating system on which the application is run (Mac only).
*/
const As::ScanDatabase As::SCAN_DATABASE;

// Alias names

/*!
\typedef As::ScanSectionMap

Synonym for QMap<QString, QMap<QString, QString> >.
*/

/*!
\typedef As::ScanMap

Synonym for QMap<QString, QMap<QString, QMap<QString, QString> > >.
*/

// Internally defined constants or enums

/*!
\variable As::MONITOR_NORM
\brief the monitor used for the normsalisation.
*/

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
\value SHELX_HKL        ShelX
\value TBAR_TB          Tbar
\value CCSL_DAT         CCSL
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


