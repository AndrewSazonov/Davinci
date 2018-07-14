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
class QStringList;
template <class Key, class T> class QHash;
template <class Key, class T> class QMap;

namespace As { //AS_BEGIN_NAMESPACE

class ScanDatabase;

// Alias names (using instead of typedef)
using ScanSectionMap_t = QMap<QString, QMap<QString, QString> >;
using ScanMap_t = QMap<QString, QMap<QString, QMap<QString, QString> > >;

// Internally defined constants

constexpr int DEBUG_INDEX_WIDTH = 5;
constexpr int DEBUG_FUNCTION_WIDTH = 25;
constexpr int DEBUG_TIME_WIDTH = 6;
constexpr int DEBUG_HYPERLINK_WIDTH = 35;

constexpr int APP_WINDOW_WIDTH  = 900;
constexpr int APP_WINDOW_HEIGHT = 600;

constexpr int UI_VERTICAL_SPACE_ROWS = 7;

constexpr int MIN_NUM_SCAN = 3;
constexpr int MIN_NUM_SKIP = 0;
constexpr int MIN_NUM_BKG  = 1;
constexpr int ADD_NUM_PEAK = 1; //1

constexpr int SIDEBAR_WIDTH = 300; //settings.value("Sidebar/width", 300).toInt();

constexpr double MONITOR_NORM = 10000.;

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

enum class PlotType
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

extern const QColor SELECTION_BACKGROUND;
extern const QString DATA_TIME_FORMAT;
extern const QStringList COUNT_TYPES;

extern const QHash<int, double> MC_CANDLISH_FACTOR;

extern const ScanDatabase SCAN_DATABASE;

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


