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
class QElapsedTimer;
class QString;
class QStringList;
template <class Key, class T> class QHash;
template <class Key, class T> class QMap;

namespace As { //AS_BEGIN_NAMESPACE
//Q_NAMESPACE

class ScanDatabase;

// Externally defined constants or variables

extern QElapsedTimer elapsedTimer;
extern int debugCount;

extern const QColor SELECTION_BACKGROUND;
extern const QString DATA_TIME_FORMAT;
extern const QStringList COUNT_TYPES;

extern const QHash<int, double> MC_CANDLISH_FACTOR;

extern const ScanDatabase SCAN_DATABASE;

// Alias names

typedef QMap<QString, QMap<QString, QString> > ScanSectionMap;
typedef QMap<QString, QMap<QString, QMap<QString, QString> > > ScanMap;

// Internally defined constants or enums

const int DEBUG_INDEX_WIDTH = 5;
const int DEBUG_FILE_WIDTH = 10;
const int DEBUG_LINE_WIDTH = 4;
const int DEBUG_FUNCTION_WIDTH = 25;
const int DEBUG_TIME_WIDTH = 5;
const int DEBUG_MESSAGE_WIDTH = 90;
const int DEBUG_SPACE = 3;
const int DEBUG_WIDTH =
        DEBUG_INDEX_WIDTH + DEBUG_SPACE +
        DEBUG_FILE_WIDTH + DEBUG_SPACE +
        DEBUG_LINE_WIDTH + DEBUG_SPACE +
        DEBUG_FUNCTION_WIDTH + DEBUG_SPACE +
        DEBUG_TIME_WIDTH + DEBUG_SPACE +
        DEBUG_MESSAGE_WIDTH;

const int APP_WINDOW_WIDTH  = 900;
const int APP_WINDOW_HEIGHT = 600;

const int UI_VERTICAL_SPACE_ROWS = 7;

const int MIN_NUM_SCAN = 3;
const int MIN_NUM_SKIP = 0;
const int MIN_NUM_BKG  = 1;
const int ADD_NUM_PEAK = 1; //1

const double MONITOR_NORM = 10000.;

const int SIDEBAR_WIDTH = 300; //settings.value("Sidebar/width", 300).toInt();

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

enum OutputFileType
{
    GENERAL_CSV,
    SHELX_HKL,
    TBAR_TB,
    CCSL_DAT,
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

} //AS_END_NAMESPACE

#endif // AS_CONSTANTS_HPP

//#include <QObject> // to recognise the macros 'Q_NAMESPACE', 'Q_ENUM_NS', 'Q_ENUM_NS_IMPL', etc.
//#include <QtGlobal> // to recognise the type name 'qreal', etc. (also loaded by <QObject>)
//https://www.kdab.com/new-qt-5-8-meta-object-support-namespaces/
//#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
//Q_NAMESPACE // Tells moc to generate meta-object information for the enclosing namespace, i.e. 'As'
//#endif
//#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
//Q_ENUM_NS(GlobalColor) // Tells moc to generate meta-object information for enum
//#endif


