/*
    Davinci, a software for the single-crystal diffraction data reduction.
    Copyright (C) 2015-2017 Andrew Sazonov

    This file is part of Davinci.

    Davinci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Davinci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Davinci.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QString>

#include "Constants.hpp"
#include "Macros.hpp"

#include "ScanDict.hpp"

/*!
    \class As::ScanDict

    \brief The ScanDict class is a class that provides a dictionary with
    all the possible parameters of the diffraction scan.

    \inmodule Diffraction
*/

/*!
    \typedef As::ScanDict::ElementAttributes_t

    Synonym for QMap<QString, QString>.
*/

/*!
    \typedef As::ScanDict::GroupElements_t

    Synonym for QMap<QString, ElementAttributes_t>.
*/

/*!
    \typedef As::ScanDict::PropertyGroups_t

    Synonym for QMap<QString, GroupElements_t>.
*/

/*!
    \variable As::ScanDict::MIN_DATA_POINTS
    \brief the minimum number of data points in the measured scan.
*/
const int As::ScanDict::MIN_DATA_POINTS = 3;

/*!
    \variable As::ScanDict::MIN_SKIP_DATA_POINTS
    \brief the minimum number of data points to be skipped in the measured scan.
*/
const int As::ScanDict::MIN_SKIP_DATA_POINTS = 0;

/*!
    \variable As::ScanDict::MIN_BKG_DATA_POINTS
    \brief the minimum number of data points considered as background (in contrast to peak)
    in the measured scan.
*/
const int As::ScanDict::MIN_BKG_DATA_POINTS = 1;

/*!
    \variable As::ScanDict::EXTRA_PEAK_DATA_POINTS
    \brief the number of extra points to extend the peak in both left and right directions
    by reducing the background, if possible.
*/
const int As::ScanDict::EXTRA_PEAK_DATA_POINTS = 1;

/*!
    \variable As::ScanDict::DEFAULT_MONITOR
    \brief the default value of the monitor counts used for the normsalisation.
*/
const qreal As::ScanDict::DEFAULT_MONITOR = 10000.0;

/*!
    \variable As::ScanDict::DATE_TIME_FORMAT
    \brief the date and time format of the measured scan used for output.
*/
//const QString As::ScanDict::DATE_TIME_FORMAT("yyyy/MM/dd hh:mm");
//const QString As::ScanDict::DATE_TIME_FORMAT("dd-MM-yyyy hh:mm");
const QString As::ScanDict::DATE_TIME_FORMAT("yyyy-MM-dd hh:mm");

/*!
    \variable As::ScanDict::MC_CANDLISH_FACTOR
    \brief the McCandlish factor lookup table depends on the instrument type.
*/
const QMap<int, qreal> As::ScanDict::MC_CANDLISH_FACTOR {
    { As::UNKNOWN_FILE,  0. },
    { As::NICOS_DAT,     0. },
    { As::HEIDI_DAT,     2.5e-2 },
    { As::HEIDI_LOG,     2.5e-2 },
    { As::POLI_LOG,      0. },
    { As::S6T2_DAT,      0. } };

/*!
    \enum As::ScanDict::BeamTypes

    This enum type describes types of the neutron beam.

    \value UNPOLARISED      Unpolarised neutron beam
    \value POLARISED_UP     Polarised neutron beam with spins up
    \value POLARISED_DOWN   Polarised neutron beam with spins down
*/

/*!
    \variable As::ScanDict::BEAM_TYPES
    \brief the types of the neutron beam as dictionary:
    enum type as keys, string type as abbreviations.
*/
const QMap<int, QString> As::ScanDict::BEAM_TYPES = {
    { As::ScanDict::UNPOLARISED,     "" },
    { As::ScanDict::POLARISED_UP,    "(+)" },
    { As::ScanDict::POLARISED_DOWN,  "(-)" } };

/*!
    Constructs the dictionary.
*/
As::ScanDict::ScanDict()  {

    QString a = As::ScanDict::DATE_TIME_FORMAT;
    ADEBUG << As::ScanDict::DATE_TIME_FORMAT;
    ADEBUG << a;
    //  --------------------------------------------------------------------------------------
    //  Holds the number of the experimental scan.
    //  --------------------------------------------------------------------------------------
    selectGroup("number");
    //  --------------------------------------------------------------------------------------
    //  element       format  units     tooltip
    //  --------------------------------------------------------------------------------------
    set("Scan",      "i",    "",       "Relative index number of the experimental scan");
    set("Excluded",  "i",    "",       "Is current scan excluded (1) or correctly measured (0)");
    set("Batch",     "i",    "",       "A batch number to distinguish between groups of reflections with separate scale factors");

    //  --------------------------------------------------------------------------------------
    //  Holds the arrays of the experimental angles.
    //  --------------------------------------------------------------------------------------
    selectGroup("angles");
    //  --------------------------------------------------------------------------------------
    //  element   format  units     tooltip
    //  --------------------------------------------------------------------------------------
    set("2Theta", "0.2f", "\u00B0", "two-theta detector angle");
    set("Theta",  "0.2f", "\u00B0", "theta detector angle"); // for convenience: tbar, etc.
    set("Gamma",  "0.2f", "\u00B0", "gamma detector angle");
    set("Omega",  "0.2f", "\u00B0", "omega instrument angle");
    set("Nu",     "0.2f", "\u00B0", "nu detector angle");
    set("Chi",    "0.2f", "\u00B0", "chi instrument angle");
    set("Chi1",   "0.2f", "\u00B0", "chi1 instrument angle");
    set("Chi2",   "0.2f", "\u00B0", "chi2 instrument angle");
    set("Phi",    "0.2f", "\u00B0", "phi instrument angle");
    set("Psi",    "0.2f", "\u00B0", "psi instrument angle");

    //  --------------------------------------------------------------------------------------
    //  Holds the arrays of the direction cosines of incident (s0) and diffracted (s2) beams
    //  --------------------------------------------------------------------------------------
    selectGroup("cosines");
    //  --------------------------------------------------------------------------------------
    //  element   format   units  tooltip
    //  --------------------------------------------------------------------------------------
    set("S0X",    "0.5f",  "",    "...");
    set("S0Y",    "0.5f",  "",    "...");
    set("S0Z",    "0.5f",  "",    "...");
    set("S2X",    "0.5f",  "",    "...");
    set("S2Y",    "0.5f",  "",    "...");
    set("S2Z",    "0.5f",  "",    "...");

    //  --------------------------------------------------------------------------------------
    //  Holds the arrays of the Miller indicies \a h, \a k and \l.
    //  --------------------------------------------------------------------------------------
    selectGroup("indices");
    //  --------------------------------------------------------------------------------------
    //  element  format  units   tooltip
    //  --------------------------------------------------------------------------------------
    set("H",     "0.3f",  "",    "h Miller index");
    set("K",     "0.3f",  "",    "k Miller index");
    set("L",     "0.3f",  "",    "l Miller index");
    //set("indices", "mean", "HKL",  "",    "(hkl) Miller indices");

    //  --------------------------------------------------------------------------------------
    //  Holds the experimental conditions.
    //  --------------------------------------------------------------------------------------
    selectGroup("conditions");
    //  --------------------------------------------------------------------------------------
    //  element                  format                units     tooltip
    //  --------------------------------------------------------------------------------------
    set("Absolute index",        "i",                  "",       "Absolute index number of the experimental scan");
    set("Points count",          "i",                  "",       "The number of data points in the scan");
    set("Date & Time",           As::ScanDict::DATE_TIME_FORMAT, "",       "Date and time of the measurements");
    set("Temperature",           "0.3f",               "K",      "Temperature during the measurements");
    set("Magnetic field",        "0.2f",               "T",      "Magnetic field during the measurements");
    set("Electric field",        "0.2f",               "kV",     "Electric field during the measurements");
    set("Wavelength",            "0.3f",               "\u212B", "Neutron wavelength");
    set("Time/step",             "0.2f",               "s",      "Time per step");
    set("Time/step(+)",          "0.2f",               "s",      "Time per step (Up)");
    set("Time/step(-)",          "0.2f",               "s",      "Time per step (Down)");
    set("Polarisation (in/out)", "s",                  "",       "Measured polarisation cross-section");

    //  --------------------------------------------------------------------------------------
    //  Holds the polarisation parameters.
    //  --------------------------------------------------------------------------------------
    selectGroup("polarisation");
    //  --------------------------------------------------------------------------------------
    //  element  format  units     tooltip
    //  --------------------------------------------------------------------------------------
    set("Pin",   "s",    "",       "Direction of the input polarization");
    set("Pout",  "s",    "",       "Direction of the output polarization");
    set("Fin",   "s",    "",       "Status of the input flipper");
    set("Fout",  "s",    "",       "Status of the output flipper");

    //  --------------------------------------------------------------------------------------
    //  Holds the major measured intensities.
    //  -------------------------------------------------------------------------------------
    selectGroup("intensities");
    //  --------------------------------------------------------------------------------------
    //  element             format  units     tooltip
    //  --------------------------------------------------------------------------------------
    set("Detector",         "",    "counts", "Intensity measured in the detector");
    set("sDetector",        "",    "counts", "ESD Intensity measured in the detector");
    set("Detector(+)",      "",    "counts", "Intensity (Up) measured in the detector");
    set("sDetector(+)",     "",    "counts", "ESD Intensity measured in the detector");
    set("Detector(-)",      "",    "counts", "Intensity (Down) measured in the detector");
    set("sDetector(-)",     "",    "counts", "ESD Intensity measured in the detector");
    set("Monitor",          "",    "counts", "Intensity measured in the monitor");
    set("sMonitor",         "",    "counts", "ESD Intensity measured in the monitor");
    set("Monitor(+)",       "",    "counts", "Intensity (Up) measured in the monitor");
    set("sMonitor(+)",      "",    "counts", "ESD Intensity measured in the monitor");
    set("Monitor(-)",       "",    "counts", "Intensity (Down) measured in the monitor");
    set("sMonitor(-)",      "",    "counts", "ESD Intensity measured in the monitor");
    set("Monitor1",         "",    "counts", "Intensity measured in the first monitor");
    set("Monitor1(+)",      "",    "counts", "Intensity (Up) measured in the monitor");
    set("Monitor1(-)",      "",    "counts", "Intensity (Down) measured in the monitor");
    set("Monitor2",         "",    "counts", "Intensity measured in the second monitor");
    set("Monitor2(+)",      "",    "counts", "Intensity (Up) measured in the monitor");
    set("Monitor2(-)",      "",    "counts", "Intensity (Down) measured in the monitor");
    set("DetectorNorm",     "",    "counts", "Intensity measured in the detector normalised by time");
    set("sDetectorNorm",    "",    "counts", "ESD Intensity measured in the detector normalised by time");
    set("DetectorNorm(+)",  "",    "counts", "Intensity measured in the detector normalised by time");
    set("sDetectorNorm(+)", "",    "counts", "ESD Intensity measured in the detector normalised by time");
    set("DetectorNorm(-)",  "",    "counts", "Intensity measured in the detector normalised by time");
    set("sDetectorNorm(-)", "",    "counts", "ESD Intensity measured in the detector normalised by time");
    set("MonitorNorm",      "",    "counts", "Intensity measured in the monitor normalised by time");
    set("sMonitorNorm",     "",    "counts", "ESD Intensity measured in the monitor normalised by time");
    set("MonitorNorm(+)",   "",    "counts", "Intensity measured in the monitor normalised by time");
    set("sMonitorNorm(+)",  "",    "counts", "ESD Intensity measured in the monitor normalised by time");
    set("MonitorNorm(-)",   "",    "counts", "Intensity measured in the monitor normalised by time");
    set("sMonitorNorm(-)",  "",    "counts", "ESD Intensity measured in the monitor normalised by time");

    //  --------------------------------------------------------------------------------------
    //  Holds the calculated parameters.
    //  --------------------------------------------------------------------------------------
    selectGroup("calculations");
    //  --------------------------------------------------------------------------------------
    //  element         format  units         tooltip
    //  --------------------------------------------------------------------------------------
    set("BkgNorm",      "0.4f", "arb.units", "Bkg");
    set("BkgNormErr",   "0.4f", "arb.units", "ESD Bkg");
    set("Fwhm",         "0.4f", "deg",       "Full width at half max");
    set("FwhmErr",      "0.4f", "deg",       "ESD Full width at half max");
    set("FR",           "0.4f", "arb.units", "Flipping ratio");
    set("FRerr",        "0.4f", "arb.units", "ESD Flipping ratio");
    set("|FR-1|/FRerr", "0.2f", "arb.units", "Flipping ratio");
    //  --------------------------------------------------------------------------------------
    set("numBkgLeft",   "0.2f", "counts",    "Number of the left background points");
    set("numBkgRight",  "0.2f", "counts",    "Number of the right background points");
    set("numSkipLeft",  "0.2f", "counts",    "Number of the left skipped points");
    set("numSkipRight", "0.2f", "counts",    "Number of the right skipped points");
    //  --------------------------------------------------------------------------------------
    for (const QString& t : As::ScanDict::BEAM_TYPES.values()) {
        set("IntMax" + t,      "0.2f", "arb.units", "Peak intensity in maximum");
        set("IntMaxErr" + t,   "0.2f", "arb.units", "ESD Peak intensity in maximum");
        set("IntSum" + t,      "0.2f", "arb.units", "Total peak intensity... sum");
        set("IntSumErr" + t,   "0.2f", "arb.units", "ESD Total peak intensity... sum");
        set("Area" + t,        "0.2f", "arb.units", "Raw integrated intensity");
        set("AreaErr" + t,     "0.2f", "arb.units", "ESD Raw integrated intensity");
        set("AreaNorm" + t,    "0.2f", "arb.units", "Normalised integrated intensity");
        set("AreaNormErr" + t, "0.2f", "arb.units", "ESD Normalised integrated intensity");
        set("Sf2" + t,         "0.2f", "arb.units", "Corrected and normalised integrated intensity (structure factor)");
        set("Sf2Err" + t,      "0.2f", "arb.units", "ESD Corrected and normalised integrated intensity"); }

    //  --------------------------------------------------------------------------------------
    //  Holds the orientation matrix.
    //  --------------------------------------------------------------------------------------
    selectGroup("orientation");
    //  --------------------------------------------------------------------------------------
    //  element    format  units  tooltip
    //  --------------------------------------------------------------------------------------
    set("matrix",  "",     "",    "Orientation matrix UB");

    //  --------------------------------------------------------------------------------------
    //  Holds the scan data array.
    //  --------------------------------------------------------------------------------------
    selectGroup("scandata");
    //  --------------------------------------------------------------------------------------
    //  element     format  units  tooltip
    //  --------------------------------------------------------------------------------------
    set("data",     "",     "",    "Whole set of the scan data");
    set("headers",  "",     "",    "Headers for the whole set of the scan data");

    //  --------------------------------------------------------------------------------------
    //  Holds the input file parameters.
    //  --------------------------------------------------------------------------------------
    selectGroup("file");
    //  --------------------------------------------------------------------------------------
    //  element       format  units   tooltip
    //  --------------------------------------------------------------------------------------
    set("File name",  "",     "",     "Name of the file with the original data");
    set("File path",  "",     "",     "Path to the file with the original data"); // should path also include the name?

    //  --------------------------------------------------------------------------------------
    //  Holds the miscellaneous parameters.
    //  --------------------------------------------------------------------------------------
    selectGroup("misc");
    //  --------------------------------------------------------------------------------------
    //  element       format  units   tooltip
    //  --------------------------------------------------------------------------------------
    set("lines",      "",     "",     "Numbers of lines of the scan points in the file");


}

/*!
    Destroys the dictionary.
*/
As::ScanDict::~ScanDict() {
    ADESTROYED; }

/*!
    Returns the value associated with the key \a section.
*/
const As::ScanDict::GroupElements_t As::ScanDict::operator[](const QString& section) const {
    return m_scanDict[section]; }

/*!
    Returns a list containing all the keys in the map in ascending order.
*/
const QStringList As::ScanDict::keys() const {
    return m_scanDict.keys(); }

/*!
    Selects a \a group where all the set methods write to.
*/
void As::ScanDict::selectGroup(const QString& group) {
    m_selectedGroup = group; }

/*!
    Adds a new \a element to the previously selected group with the given \a format,
    \a units and \a tooltip. The group should be selected with selectGroup method.
*/
void As::ScanDict::set(const QString& element,
                       const QString& format,
                       const QString& units,
                       const QString& tooltip) {

    //ADEBUG << BEAM_TYPES;

    m_scanDict[m_selectedGroup][element] = ElementAttributes_t{
        {"format", format }, {"units", units }, {"tooltip", tooltip } }; }








/*!
    \variable As::ScanDict::Properties
    \brief the dictionary with all the possible scan properties.
*/
const As::ScanDict As::ScanDict::Properties;

