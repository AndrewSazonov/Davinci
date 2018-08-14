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

#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTime>

#include "Functions.hpp"
#include "Macros.hpp"

#include "RealVector.hpp"
#include "ScanDatabase.hpp"

#include "Scan.hpp"


#include "ScanArray.hpp"

#include <QtConcurrent>


/*!
    \class As::Scan

    \brief The Scan is a class that provides a set of variables
    required to describe the generic diffraction scan.

    \inmodule Diffraction
*/

/*!
    Constructs and initializes a scan with the given \a parent.
*/
As::Scan::Scan(QObject* parent)
    : QObject(parent) {
    init(); }

/*!
    Destroys the scan.
*/
As::Scan::~Scan() {
    ADESTROYED; }

/*!
    Initializes a scan with default parameters.
*/
void As::Scan::init() {
    // Not completed yet!?

    // Single calculated values (doesn't depend on polarisation)
    m_numLeftSkipPoints  = MIN_NUM_SKIP;
    m_numRightSkipPoints = MIN_NUM_SKIP;
    m_numNonSkipPoints   = 0;
    m_numLeftBkgPoints   = MIN_NUM_BKG;
    m_numRightBkgPoints  = MIN_NUM_BKG;
    m_numPeakPoints      = 0;
    m_normMeanBkg        = qQNaN();
    m_fullWidthHalfMax   = qQNaN(); m_fullWidthHalfMaxErr = qQNaN();
    m_flippingRatio      = qQNaN(); m_flippingRatioErr    = qQNaN();
    m_peakPosition       = qQNaN(); // Is it used? Calc esd?
    m_plotType           = As::PlotType(0);

    // Multi calculated values (depend on polarisation).
    for (const QString& countType : As::COUNT_TYPES) {
        m_maxPeakInty[countType]  = qQNaN(); m_maxPeakIntyErr[countType]  = qQNaN();
        m_sumPeakInty[countType]  = qQNaN(); m_sumPeakIntyErr[countType]  = qQNaN();
        m_peakArea[countType]     = qQNaN(); m_peakAreaErr[countType]     = qQNaN();
        m_normPeakArea[countType] = qQNaN(); m_normPeakAreaErr[countType] = qQNaN();
        m_structFactor[countType] = qQNaN(); m_structFactorErr[countType] = qQNaN(); } }

/*!
    Replaces the \a data of the given scan \a section and \a entry, if they exist.

    If the scan doesn't contain [section][entry]["data"] yet, but the ScanDatabase does,
    the data is inserted.
*/
void As::Scan::setData(const QString& section,
                       const QString& entry,
                       const QString& data) {
    if (data.isEmpty()) {
        //AASSERT(false, QString("empty data array [%1][%2] passed to the function").arg(section).arg(entry));
        return; }

    if (m_scan[section].contains(entry)) {
        m_scan[section][entry].insert("data", data); }

    else if (As::SCAN_DATABASE[section].contains(entry)) {
        m_scan[section][entry] = As::SCAN_DATABASE[section][entry];
        m_scan[section][entry].insert("data", data); }

    else {
        AASSERT(false, QString("no such section '%1' or entry '%2' in ScanDatabase").arg(section).arg(entry)); } }

/*!
    Appends the \a data of the given scan \a section and \a entry, if they exist.

    If the scan doesn't contain [section][entry]["data"] yet, but the ScanDatabase does,
    the data is inserted.
*/
void As::Scan::appendData(const QString& section,
                          const QString& entry,
                          const QString& data) {
    if (data.isEmpty()) {
        //AASSERT(false, QString("empty data array [%1][%2] passed to the function").arg(section).arg(entry));
        return; }

    if (m_scan[section].contains(entry)) {
        m_scan[section][entry].insert("data", m_scan[section][entry]["data"] + " " + data); }

    else if (As::SCAN_DATABASE[section].contains(entry)) {
        m_scan[section][entry] = As::SCAN_DATABASE[section][entry];
        m_scan[section][entry].insert("data", data); }

    else {
        AASSERT(false, QString("no such section '%1' or entry '%2' in ScanDatabase").arg(section).arg(entry)); } }

/*!
    Removes the given \a section and \a entry from the scan.
*/
void As::Scan::removeData(const QString& section,
                          const QString& entry) {
    if (m_scan[section].contains(entry)) {
        m_scan[section][entry].remove("data"); }

    else {
        AASSERT(false, QString("no such section '%1' or entry '%2' in ScanDatabase").arg(section).arg(entry)); } }

/*!
    Returns the required field value by the given scan \a section, \a entry and field \a name.

    If an error occurs, *\a{ok} is set to \c false; otherwise *\a{ok} is set to \c true.
*/
const QString As::Scan::value(const QString& section,
                              const QString& entry,
                              const QString& name,
                              bool* ok) const {
    if (m_scan[section][entry].contains(name)) {
        if (ok) {
            *ok = true; }

        return m_scan[section][entry][name]; }

    if (ok) {
        *ok = false; }

    return QString(); }

/*!
    Returns the data field of the given scan \a section and \a entry.

    If an error occurs, *\a{ok} is set to \c false; otherwise *\a{ok} is set to \c true.
*/
const QString As::Scan::data(const QString& section,
                             const QString& entry,
                             bool* ok) const {
    return value(section, entry, "data", ok); }

/*!
    Returns the format field of the given scan \a section and \a entry.

    If an error occurs, *\a{ok} is set to \c false; otherwise *\a{ok} is set to \c true.
*/
const QString As::Scan::format(const QString& section,
                               const QString& entry,
                               bool* ok) const {
    return value(section, entry, "format", ok); }

/*!
    Returns the single data value of the given scan \a section and \a entry
    formatted with its corresponding format.
*/
const QString As::Scan::printDataSingle(const QString& section,
                                        const QString& entry) const {
    if (!m_scan[section].contains(entry)) {
        AASSERT(false, QString("no such section '%1' or entry '%2' in the current scan"));
        return QString(); }

    const QString format = m_scan[section][entry]["format"];
    const QString data = m_scan[section][entry]["data"];
    return As::FormatString(data, format); }

/*!
    Returns the range data values of the given scan \a section and \a entry.
*/
const QString As::Scan::printDataRange(const QString& section,
                                       const QString& entry) const {
    if (!m_scan[section].contains(entry)) {
        AASSERT(false, QString("no such section '%1' or entry '%2' in the current scan"));
        return QString(); }

    const QString format = m_scan[section][entry]["format"];

    if (!format.contains("f")) {
        return printDataSingle(section, entry); }

    const As::RealVector data = m_scan[section][entry]["data"];
    const qreal min = data.min();
    const qreal max = data.max();

    if ( ( qAbs(max / min) < 1.01 ) OR ( (max - min) < 0.01 ) ) {
        return printDataSingle(section, entry); }

    return QString().sprintf(qPrintable("%" + format + " - " + "%" + format), min, max); }

/*!
    Returns a list containing all the keys in the scan map in ascending order.
*/
const QStringList As::Scan::keys() const {
    return m_scan.keys(); }

/*!
    Returns the As::ScanSection_t associated with the key \a key as a modifiable reference.

    If the map contains no item with key \a key, the function inserts a default-constructed
    value into the map with key \a key, and returns a reference to it. If the map contains
    multiple items with key \a key key, this function returns a reference to the most recently
    inserted value.
*/
As::ScanSection_t& As::Scan::operator[](const QString& key) {
    return m_scan[key]; }

/*!
    \overload
*/
const As::ScanSection_t As::Scan::operator[](const QString& key) const {
    return m_scan[key]; }

/*!
    Returns the scan as a QMap.
*/
const As::Scan_t As::Scan::toQMap() const {
    return m_scan; }

/*!
    Sets \a name to the scan angle.
*/
void As::Scan::setScanAngle(const QString& name) {
    m_scanAngle = name; }

/*!
    Finds the scan angle and sets it.
*/
void As::Scan::findAndSetScanAngle() {

    const QStringList subitemKeys = m_scan["angles"].keys();

    for (const auto& subitemKey : subitemKeys) {
        const As::RealVector v = data("angles", subitemKey);

        // add all the angles with non-zero range to the list of scan angles,
        // and allow user to chose the axis in the plot!?

        if ( ( v.simplify().size() > 1 ) AND ( v.range() > 0.1 ) ) {
            setScanAngle(subitemKey);
            return; } } }

/*!
    Returns the name of the scan angle.
*/
const QString As::Scan::scanAngle() const {
    return m_scanAngle; }

/*!
    Sets \a value to the scan step size.
*/
void As::Scan::setScanStep(const qreal value) {
    m_scanStep = value; }

/*!
    Returns the scan step size.
*/
qreal As::Scan::scanStep() const {
    return m_scanStep; }

/*!
    Sets \a name to the absolute path of the file (including file name) which contains the scan.
*/
void As::Scan::setAbsoluteFilePath(const QString& name) {
    m_absoluteFilePath = name; }

/*!
    Returns the absolute path of the file (including file name) which contains the scan.
*/
const QString As::Scan::absoluteFilePath() const {
    return m_absoluteFilePath; }

/*!
    Returns the absolute path of the file (without file name) which contains the scan.
*/
const QString As::Scan::absolutePath() const {
    // Absolute File Path:             /tmp/dummy.dat
    // Absolute Path:                  /tmp
    return QFileInfo(m_absoluteFilePath).absolutePath(); }

/*!
    Returns the base name (without extension) of the file which contains the scan.
*/
const QString As::Scan::baseName() const {
    // Absolute File Path:             /tmp/dummy.dat
    // Base Name:                      dummy
    return QFileInfo(m_absoluteFilePath).baseName(); }

/*!
    Returns the absolute path of the file which contains the scan.
*/
const QString As::Scan::absolutePathWithBaseName() const {
    /*
        Absolute File Path:             /tmp/dummy.dat
        Absolute Path:                  /tmp
        Base Name:                      dummy
        Absolute Path with Base Name:   /tmp/dummy
    */
    return QDir(absolutePath()).filePath(baseName()); }

/*!
    Returns the absolute path (with base name and Miller indices h,k,l) of
    the file which contains the scan.
*/
const QString As::Scan::absolutePathWithBaseNameAndHkl() const {
    return QString("%1_hkl(%2,%3,%4)").
           arg(absolutePathWithBaseName()).
           arg(millerIndex("H"), 0, 'f', 3).
           arg(millerIndex("K"), 0, 'f', 3).
           arg(millerIndex("L"), 0, 'f', 3); }

/*!
    Sets the \a index of the scan file.
*/
void As::Scan::setFileIndex(const int index) {
    m_fileIndex = index; }

/*!
    Returns the index of the file which contains the scan.
*/
int As::Scan::fileIndex() const {
    return m_fileIndex; }

/*!
    Sets a scan plot type with the \a plotType.
*/
void As::Scan::setPlotType(const As::PlotType plotType) {
    m_plotType = plotType; }

/*!
    Returns the scan plot type.
*/
As::PlotType As::Scan::plotType() const {
    return m_plotType; }

/*!
    Returns the averaged Miller index value depends on its name (H, K, or L).
*/
qreal As::Scan::millerIndex(const QString& name) const {
    if (name == "H" OR name == "K" OR name == "L") {
        return As::RealVector(m_scan["indices"][name]["data"]).mean(); }

    AASSERT(false, QString("Miller index name '%1' is not correct").arg(name));
    return qQNaN(); }

/*!
    Returns the number of points (detector data count) in the scan.
*/
qreal As::Scan::numPoints() const {

    // Search for maximum of all the COUNT_TYPES
    int numPointsMax = 0;

    for (const QString& countType : As::COUNT_TYPES) {

        //const QString& string = m_scan["intensities"]["Detector" + countType]["data"];
        const QString string = data("intensities", "Detector" + countType);

        if (string.length() == 0) {
            continue; }

        // Possible solutions

        // simple, but too slow: 3.9s
        //const int numPoints = string.split(" ", QString::SkipEmptyParts).size();

        // still slow: 1.2s
        //const int numPoints = string.count(" ") + 1;

        // faster, but still slow: 0.8s
        //int numPoints = 0;
        //for (int i = 0; i < string.length(); ++i) {
        //    if (string[i] == ' ') {
        //        ++numPoints; } }

        // String: "2.1 5.3 7.0 1.2". numSpaces: 3, numPoints = numDataElements = numSpaces + 1 = 4
        int numPoints = 0;

        for (int i = 0; i < string.length(); ++i) {
            if (string[i] == ' ') {
                ++numPoints; } }

        ++numPoints;

        numPointsMax = qMax(numPoints, numPointsMax); }

    return numPointsMax; }

/*!
    Returns the first line corresponding to scan data in the input file.
*/
int As::Scan::scanLine() const {
    bool ok = false;
    const int line = data("misc", "lines", &ok).section(" ", 0, 0).toInt(&ok) + 1;

    if (ok) {
        return line; }

    return 1; }


/*!
    Creates the table model of extracted scans.
*/
// Get rid of this in the scan!
void As::Scan::createExtractedTableModel() {

    // Get the table heading, cell elements values and their formats
    // from the selected scan sections listed in 'items'

    QStringList headers;
    QStringList formats;
    QList<QStringList> dataTable;

    const QStringList items({ "indices", "angles", "intensities", "conditions" });
    for (const QString& item : items) {

        const QStringList subitems = m_scan.value(item).keys();
        for (const QString& subitem : subitems) {

            const QString dataString = data(item, subitem);
            const QString formatString = format(item, subitem);
            if (!dataString.isEmpty() AND !formatString.isEmpty()) {

                headers << subitem;
                formats << formatString;
                dataTable << dataString.split(" "); } } }

    // Extracted table size

    int columnCount = dataTable.size();
    int rowCount = dataTable[0].size();

    // Create the table model based on the extracted one from above

    m_tableModel = new QStandardItemModel;
    m_tableModel->setColumnCount(columnCount);
    m_tableModel->setRowCount(rowCount);

    for (int column = 0; column < columnCount; ++column) {
        // Headers
        m_tableModel->setHorizontalHeaderItem(column, new QStandardItem(headers[column]));
        // Values
        for (int row = 0; row < rowCount; ++row) {
            auto const value = As::FormatString( dataTable[column][row], formats[column] );
            m_tableModel->setItem( row, column, new QStandardItem(value) );
            m_tableModel->item(row, column)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); } } }


/*!
    \variable As::Scan::NeighborsRemoveTypeDict
    \brief the dictionary, which stores the types of the neighbor peaks removing procedures
    as enum and thier associated descriptions as string.
*/
const QMap<As::Scan::NeighborsRemoveType, QString> As::Scan::NeighborsRemoveTypeDict  = {
    { As::Scan::ManualNeighborsRemove, "Manually remove neighbors" },
    { As::Scan::AutoNeighborsRemove, "Automatically remove neighbors" } };

/*!
    Sets the neighbor peaks removing type as \a type.
*/
void As::Scan::setNeighborsRemoveType(const As::Scan::NeighborsRemoveType type) {
    m_neighborsRemoveType = type; }

/*!
    Returns the neighbor peaks removing type.
*/
As::Scan::NeighborsRemoveType As::Scan::neighborsRemoveType() const {
    return m_neighborsRemoveType; }

/*!
    \enum As::Scan::BkgDetectType

    This enum type describes types of the background detection procedures.

    \value ManualBkgSet     Manually set background
    \value AutoBkgDetect    Automatically detect background
*/

/*!
    \enum As::Scan::PeakAnalysisType

    This enum type describes types of the peak analysis procedures.

    \value PeakIntegration    Conventional peak integration
*/

/*!
    \variable As::Scan::PeakAnalysisTypeDict
    \brief the dictionary, which stores the types of the peak analysis procedures
    as enum and thier associated descriptions as string.
*/
const QMap<As::Scan::PeakAnalysisType, QString> As::Scan::PeakAnalysisTypeDict  = {
    //{ As::Scan::PeakFit, "Peak fitting" }
    { As::Scan::PeakIntegration, "Conventional peak integration" } };

/*!
    Sets the neighbor peaks removing type as \a type.
*/
void As::Scan::setPeakAnalysisType(const As::Scan::PeakAnalysisType type) {
    m_peakAnalysisType = type; }

/*!
    Returns the neighbor peaks removing type.
*/
As::Scan::PeakAnalysisType As::Scan::peakAnalysisType() const {
    return m_peakAnalysisType; }

/*!
    \variable As::Scan::BkgDetectTypeDict
    \brief the dictionary, which stores the types of the background detection procedures
    as enum and thier associated descriptions as string.
*/
const QMap<As::Scan::BkgDetectType, QString> As::Scan::BkgDetectTypeDict  = {
    { As::Scan::ManualBkgSet, "Manually set background" },
    { As::Scan::AutoBkgDetect, "Automatically detect background" } };

/*!
    Sets the neighbor peaks removing type as \a type.
*/
void As::Scan::setBkgDetectType(const As::Scan::BkgDetectType type) {
    m_bkgDetectType = type; }

/*!
    Returns the neighbor peaks removing type.
*/
As::Scan::BkgDetectType As::Scan::bkgDetectType() const {
    return m_bkgDetectType; }

/*!
    \enum As::Scan::PeakFitType

    This enum type describes types of the peak fit functions.

    \value GaussFit         Gaussian function
    \value LorentzFit       Lorentzian function
    \value PseudoVoigtFit   Pseudo-Voigt function
*/

/*!
    \variable As::Scan::PeakFitTypeDict
    \brief the dictionary, which stores the types of the peak fit functions
    as enum and thier associated descriptions as string.
*/
const QMap<As::Scan::PeakFitType, QString> As::Scan::PeakFitTypeDict  = {
    { As::Scan::GaussFit, "Gaussian function" },
    { As::Scan::LorentzFit, "Lorentzian function" },
    { As::Scan::PseudoVoigtFit, "Pseudo-Voigt function" } };

/*!
    Sets the peak fit type as \a type.
*/
void As::Scan::setPeakFitType(const As::Scan::PeakFitType type) {
    m_peakFitType = type; }

/*!
    Returns the peak fit type.
*/
As::Scan::PeakFitType As::Scan::peakFitType() const {
    return m_peakFitType; }





/**
    Overloads operator<< for QDebug to accept the Scan output.
*/
QDebug operator<<(QDebug debug, const As::Scan& scan) {
    return QtPrivate::printSequentialContainer(debug, "As::Scan", scan.toQMap()); }
