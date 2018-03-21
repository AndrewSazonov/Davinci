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

#include <QMap>
#include <QString>

#include "Constants.hpp"

#include "ScanDatabase.hpp"

/*!
\class As::ScanDatabase

\brief The ScanDatabase class is a class that provides a database with
all the possible parameters of the diffraction scan.

\inmodule Diffraction
*/

/*!
Constructs the database.
*/
As::ScanDatabase::ScanDatabase()
{
    //  --------------------------------------------------------------------------------------
    //  Holds the number of the experimental scan.
    //  --------------------------------------------------------------------------------------
    //  section   name         format  units     tooltip
    //  --------------------------------------------------------------------------------------
    set("number", "Scan",      "i",    "",       "Relative index number of the experimental scan");
    set("number", "Excluded",  "i",    "",       "Is current scan excluded (1) or correctly measured (0)");
    //set("number", "file",  "File num",  "g",     "",       "Index number of the file");

    //  --------------------------------------------------------------------------------------
    //  Holds the arrays of the experimental angles.
    //  --------------------------------------------------------------------------------------
    //  section   name      format  units     tooltip
    //  --------------------------------------------------------------------------------------
    set("angles", "2Theta", "0.2f", "\u00B0", "two-theta detector angle");
    set("angles", "Gamma",  "0.2f", "\u00B0", "gamma detector angle");
    set("angles", "Omega",  "0.2f", "\u00B0", "omega instrument angle");
    set("angles", "Nu",     "0.2f", "\u00B0", "nu detector angle");
    set("angles", "Chi",    "0.2f", "\u00B0", "chi instrument angle");
    set("angles", "Chi1",   "0.2f", "\u00B0", "chi1 instrument angle");
    set("angles", "Chi2",   "0.2f", "\u00B0", "chi2 instrument angle");
    set("angles", "Phi",    "0.2f", "\u00B0", "phi instrument angle");

    //  --------------------------------------------------------------------------------------
    //  Holds the experimental conditions.
    //  --------------------------------------------------------------------------------------
    //  section       name                     format                units     tooltip
    //  --------------------------------------------------------------------------------------
    set("conditions", "Absolute index",        "i",                  "",       "Absolute index number of the experimental scan");
    set("conditions", "Points count",          "i",                  "",       "The number of data points in the scan");
    set("conditions", "Date & Time",           As::DATA_TIME_FORMAT, "",       "Date and time of the measurements");
    set("conditions", "Temperature",           "0.3f",               "K",      "Temperature during the measurements");
    set("conditions", "Magnetic field",        "0.2f",               "T",      "Magnetic field during the measurements");
    set("conditions", "Electric field",        "0.2f",               "kV",     "Electric field during the measurements");
    set("conditions", "Wavelength",            "0.3f",               "\u212B", "Neutron wavelength");
    set("conditions", "Time/step",             "0.2f",               "s",      "Time per step");
    set("conditions", "Time/step(+)",          "0.2f",               "s",      "Time per step (Up)");
    set("conditions", "Time/step(-)",          "0.2f",               "s",      "Time per step (Down)");
    set("conditions", "Polarisation (in/out)", "s",                  "",       "Measured polarisation cross-section");

    //  --------------------------------------------------------------------------------------
    //  Holds the polarisation parameters.
    //  --------------------------------------------------------------------------------------
    //  section         name    format  units     tooltip
    //  -------------------------------------------------------------
    set("polarisation", "Pin",  "s",    "",       "Direction of the input polarization");
    set("polarisation", "Pout", "s",    "",       "Direction of the output polarization");
    set("polarisation", "Fin",  "s",    "",       "Status of the input flipper");
    set("polarisation", "Fout", "s",    "",       "Status of the output flipper");

    //  --------------------------------------------------------------------------------------
    //  Holds the date and time of the measurements.
    //  --------------------------------------------------------------------------------------
    //  section     name        label        format              units  tooltip
    //  --------------------------------------------------------------------------------------
    //set("dateTime", "dateTime", "Date&Time", "dd/MM/yyyy hh:mm", "",    "Date and time of the measurements");
    //set("dateTime", "date",     "Date",      "dd/MM/yyyy",       "",    "Date of the measurements");
    //set("dateTime", "time",     "Time",      "hh:mm",            "",    "Time of the measurements");

    //  --------------------------------------------------------------------------------------
    //  Holds the arrays of the Miller indicies \a h, \a k and \l.
    //  --------------------------------------------------------------------------------------
    //  section    name  format   units  tooltip
    //  ---------------- -------------------------------------------------------------
    set("indices", "H",  "0.3f",  "",    "h Miller index");
    set("indices", "K",  "0.3f",  "",    "k Miller index");
    set("indices", "L",  "0.3f",  "",    "l Miller index");
    //set("indices", "mean", "HKL",  "",    "(hkl) Miller indices");

    //  --------------------------------------------------------------------------------------
    //  Holds the major measured intensities.
    //  -------------------------------------------------------------------------------------
    //  section        name                format units     tooltip
    //  -------------------------------------------------------------------------------
    set("intensities", "Detector",         "",    "counts", "Intensity measured in the detector");
    set("intensities", "sDetector",        "",    "counts", "ESD Intensity measured in the detector");
    set("intensities", "Detector(+)",      "",    "counts", "Intensity (Up) measured in the detector");
    set("intensities", "sDetector(+)",     "",    "counts", "ESD Intensity measured in the detector");
    set("intensities", "Detector(-)",      "",    "counts", "Intensity (Down) measured in the detector");
    set("intensities", "sDetector(-)",     "",    "counts", "ESD Intensity measured in the detector");
    set("intensities", "Monitor",          "",    "counts", "Intensity measured in the monitor");
    set("intensities", "sMonitor",         "",    "counts", "ESD Intensity measured in the monitor");
    set("intensities", "Monitor(+)",       "",    "counts", "Intensity (Up) measured in the monitor");
    set("intensities", "sMonitor(+)",      "",    "counts", "ESD Intensity measured in the monitor");
    set("intensities", "Monitor(-)",       "",    "counts", "Intensity (Down) measured in the monitor");
    set("intensities", "sMonitor(-)",      "",    "counts", "ESD Intensity measured in the monitor");
    set("intensities", "Monitor1",         "",    "counts", "Intensity measured in the first monitor");
    set("intensities", "Monitor1(+)",      "",    "counts", "Intensity (Up) measured in the monitor");
    set("intensities", "Monitor1(-)",      "",    "counts", "Intensity (Down) measured in the monitor");
    set("intensities", "Monitor2",         "",    "counts", "Intensity measured in the second monitor");
    set("intensities", "Monitor2(+)",      "",    "counts", "Intensity (Up) measured in the monitor");
    set("intensities", "Monitor2(-)",      "",    "counts", "Intensity (Down) measured in the monitor");
    set("intensities", "DetectorNorm",     "",    "counts", "Intensity measured in the detector normalised by time");
    set("intensities", "sDetectorNorm",    "",    "counts", "ESD Intensity measured in the detector normalised by time");
    set("intensities", "DetectorNorm(+)",  "",    "counts", "Intensity measured in the detector normalised by time");
    set("intensities", "sDetectorNorm(+)", "",    "counts", "ESD Intensity measured in the detector normalised by time");
    set("intensities", "DetectorNorm(-)",  "",    "counts", "Intensity measured in the detector normalised by time");
    set("intensities", "sDetectorNorm(-)", "",    "counts", "ESD Intensity measured in the detector normalised by time");
    set("intensities", "MonitorNorm",      "",    "counts", "Intensity measured in the monitor normalised by time");
    set("intensities", "sMonitorNorm",     "",    "counts", "ESD Intensity measured in the monitor normalised by time");
    set("intensities", "MonitorNorm(+)",   "",    "counts", "Intensity measured in the monitor normalised by time");
    set("intensities", "sMonitorNorm(+)",  "",    "counts", "ESD Intensity measured in the monitor normalised by time");
    set("intensities", "MonitorNorm(-)",   "",    "counts", "Intensity measured in the monitor normalised by time");
    set("intensities", "sMonitorNorm(-)",  "",    "counts", "ESD Intensity measured in the monitor normalised by time");

    //  --------------------------------------------------------------------------------------
    //  Holds the calculated parameters.
    //  --------------------------------------------------------------------------------------
    //  section         name            format  units        tooltip
    //  --------------------------------------------------------------------------------------
    set("calculations", "BkgNorm",      "0.4f", "arb.units", "Bkg");
    set("calculations", "BkgNormErr",   "0.4f", "arb.units", "ESD Bkg");
    set("calculations", "Fwhm",         "0.4f", "deg",       "Full width at half max");
    set("calculations", "FwhmErr",      "0.4f", "deg",       "ESD Full width at half max");
    set("calculations", "FR",           "0.4f", "arb.units", "Flipping ratio");
    set("calculations", "sFR",          "0.4f", "arb.units", "ESD Flipping ratio");
    set("calculations", "|FR-1|/sFR",   "0.2f", "arb.units", "Flipping ratio");
    //  --------------------------------------------------------------------------------------
    set("calculations", "numBkgLeft",   "0.2f", "counts",    "Number of the left background points");
    set("calculations", "numBkgRight",  "0.2f", "counts",    "Number of the right background points");
    set("calculations", "numSkipLeft",  "0.2f", "counts",    "Number of the left skipped points");
    set("calculations", "numSkipRight", "0.2f", "counts",    "Number of the right skipped points");
    //  --------------------------------------------------------------------------------------
    for (const QString &t : As::COUNT_TYPES) {
        set("calculations", "IntMax"+t,      "0.2f", "arb.units", "Peak intensity in maximum");
        set("calculations", "IntMaxErr"+t,   "0.2f", "arb.units", "ESD Peak intensity in maximum");
        set("calculations", "IntSum"+t,      "0.2f", "arb.units", "Total peak intensity... sum");
        set("calculations", "IntSumErr"+t,   "0.2f", "arb.units", "ESD Total peak intensity... sum");
        set("calculations", "Area"+t,        "0.2f", "arb.units", "Raw integrated intensity");
        set("calculations", "AreaErr"+t,     "0.2f", "arb.units", "ESD Raw integrated intensity");
        set("calculations", "AreaNorm"+t,    "0.2f", "arb.units", "Normalised integrated intensity");
        set("calculations", "AreaNormErr"+t, "0.2f", "arb.units", "ESD Normalised integrated intensity");
        set("calculations", "Sf2"+t,         "0.2f", "arb.units", "Corrected and normalised integrated intensity (structure factor)");
        set("calculations", "Sf2Err"+t,      "0.2f", "arb.units", "ESD Corrected and normalised integrated intensity"); }

    //  --------------------------------------------------------------------------------------
    //  Holds the orientation matrix.
    //  --------------------------------------------------------------------------------------
    //  section        name       format units  tooltip
    //  --------------------------------------------------------------------------------------
    set("orientation", "matrix",  "",    "",    "Orientation matrix UB");

    //  --------------------------------------------------------------------------------------
    //  Holds the scan data array.
    //  --------------------------------------------------------------------------------------
    //  section     name        format  units  tooltip
    //  --------------------------------------------------------------------------------------
    set("scandata", "data",     "",     "",    "Whole set of the scan data");
    set("scandata", "headers",  "",     "",    "Headers for the whole set of the scan data");

    //  --------------------------------------------------------------------------------------
    //  Holds the miscellaneous parameters.
    //  --------------------------------------------------------------------------------------
    //  section  name          format  units   tooltip
    //  --------------------------------------------------------------------------------------
    set("file",  "File name",  "",     "",     "Name of the file with the original data");
    set("file",  "File path",  "",     "",     "Path to the file with the original data"); // should path also include the name?
    set("misc",  "lines",      "",     "",     "Numbers of lines of the scan points in the file");
    //set("conditions",  "step",        "Time/step",   "s",      "This is a time per step");
    //set("misc",  "nPoints",  "Points count",  "",     "",     "The number of data points in the scan");

}

/*!
Destroys the database.
*/
As::ScanDatabase::~ScanDatabase() {}

/*!
Returns the value associated with the key \a section.
*/
const As::ScanSectionMap As::ScanDatabase::operator[](const QString &section) const
{
    return m_scanDatabase[section];
}

/*!
Returns a list containing all the keys in the map in ascending order.
*/
const QStringList As::ScanDatabase::keys() const
{
    return m_scanDatabase.keys();
}

/*!
Adds a new entry to the database with the given \a section, \a entry, \a format,
\a units and \a tooltip.
*/
void As::ScanDatabase::set(const QString &section,
                           const QString &entry,
                           const QString &format,
                           const QString &units,
                           const QString &tooltip)
{
    m_scanDatabase[section][entry] = QMap<QString, QString>{
        {"format", format}, {"units", units}, {"tooltip", tooltip} };
}
