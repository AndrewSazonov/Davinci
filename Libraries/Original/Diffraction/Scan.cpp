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


/*!
\class As::Scan

\brief The Scan is a class that provides a set of variables
required to describe the generic diffraction scan.

\inmodule Diffraction
*/

/*!
Constructs and initializes a scan with the given \a parent.
*/
As::Scan::Scan(QObject *parent)
    : QObject(parent)
{
    //connect(this, &sizeChanged, this, &Counter::setValue);
    init();
}

/*!
Destroys the scan.
*/
As::Scan::~Scan()
{
    ADESTROYED;
}

/*!
Initializes a scan with default parameters.
*/
void As::Scan::init()
{
    // Not completed yet!

    // Single calculated values (doesn't depend on polarisation)
    m_numPoints          = 0;
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
    m_meanIndexH         = qQNaN();
    m_meanIndexK         = qQNaN();
    m_meanIndexL         = qQNaN();
    m_plotType           = As::PlotType(0);

    // Multi calculated values (depend on polarisation).
    for (const QString &countType : As::COUNT_TYPES) {
        m_maxPeakInty[countType]  = qQNaN(); m_maxPeakIntyErr[countType]  = qQNaN();
        m_sumPeakInty[countType]  = qQNaN(); m_sumPeakIntyErr[countType]  = qQNaN();
        m_peakArea[countType]     = qQNaN(); m_peakAreaErr[countType]     = qQNaN();
        m_normPeakArea[countType] = qQNaN(); m_normPeakAreaErr[countType] = qQNaN();
        m_structFactor[countType] = qQNaN(); m_structFactorErr[countType] = qQNaN(); }
}

/*!
Sets the \a data to the given scan \a section and \a entry.
*/
void As::Scan::setData(const QString &section,
                       const QString &entry,
                       const QString &data)
{
    QString message;

    if (data.isEmpty()) {
        //message = QString("empty data array [%1][%2] passed to the function").arg(section).arg(entry);
        //Q_ASSERT_X(false, AFUNC, qPrintable(message));
        return; }

    if (m_scan[section].contains(entry)) {
        m_scan[section][entry].insert("data", data); }

    else if (As::SCAN_DATABASE[section].contains(entry)) {
        m_scan[section][entry] = As::SCAN_DATABASE[section][entry];
        m_scan[section][entry].insert("data", data); }

    else {
        message = QString("no such section '%1' or entry '%2' in ScanDatabase").arg(section).arg(entry);
        Q_ASSERT_X(false, AFUNC, qPrintable(message)); }
}

/*!
Appends the \a data to the given scan \a section and \a entry.
*/
void As::Scan::appendData(const QString &section,
                          const QString &entry,
                          const QString &data)
{
    QString message;

    if (data.isEmpty()) {
        message = QString("empty data array passed to the function");
        Q_ASSERT_X(false, AFUNC, qPrintable(message));
        return; }

    if (m_scan[section].contains(entry)) {
        m_scan[section][entry].insert("data", m_scan[section][entry]["data"] + " " + data); }

    else if (As::SCAN_DATABASE[section].contains(entry)) {
        m_scan[section][entry] = As::SCAN_DATABASE[section][entry];
        m_scan[section][entry].insert("data", data); }

    else {
        message = QString("no such section '%1' or entry '%2' in ScanDatabase").arg(section).arg(entry);
        Q_ASSERT_X(false, AFUNC, qPrintable(message)); }
}

/*!
Removes the given scan \a section and \a entry.
*/
void As::Scan::removeData(const QString &section,
                          const QString &entry)
{
    if (m_scan[section].contains(entry)) {
        m_scan[section][entry].remove("data"); }

    else {
        QString message = QString("no such section '%1' or entry '%2' in ScanDatabase").arg(section).arg(entry);
        Q_ASSERT_X(false, AFUNC, qPrintable(message)); }
}

/*!
Returns the required field value of the given scan \a section, \a entry and \a name.

If an error occurs, *\a{ok} is set to \c false; otherwise *\a{ok} is set to \c true.
*/
const QString As::Scan::value(const QString &section,
                              const QString &entry,
                              const QString &name,
                              bool *ok) const
{
    if (m_scan[section][entry].contains(name)) {
        if (ok)
            *ok = true;
        return m_scan[section][entry][name]; }

    if (ok) {
        *ok = false;
        return QString(); }

    const QString message = QString("no such section '%1', entry '%2', or name '%3' in the current scan")
            .arg(section).arg(entry).arg(name);
    Q_ASSERT_X(false, AFUNC, qPrintable(message));

    return QString();
}

/*!
Returns the data field of the given scan \a section and \a entry.

If an error occurs, *\a{ok} is set to \c false; otherwise *\a{ok} is set to \c true.
*/
const QString As::Scan::data(const QString &section,
                             const QString &entry,
                             bool *ok) const
{
    return value(section, entry, "data", ok);
}

/*!
Returns the format field of the given scan \a section and \a entry.

If an error occurs, *\a{ok} is set to \c false; otherwise *\a{ok} is set to \c true.
*/
const QString As::Scan::format(const QString &section,
                               const QString &entry,
                               bool *ok) const
{
    return value(section, entry, "format", ok);
}

/*!
Returns the single data value of the given scan \a section and \a entry
formatted with \a format.
*/
const QString As::Scan::printDataSingle(const QString &section,
                                        const QString &entry,
                                        const QString &format) const
{
    if (m_scan[section].contains(entry)) {
        const QString string = m_scan[section][entry]["data"];
        return As::FormatString(string, format); }

    const QString message = QString("no such section '%1' or entry '%2' in the current scan")
            .arg(section).arg(entry);
    Q_ASSERT_X(false, AFUNC, qPrintable(message));
    return QString();
}

/*!
Returns the single data value of the given scan \a section and \a entry.

\overload
*/
const QString As::Scan::printDataSingle(const QString &section,
                                        const QString &entry) const
{
    if (m_scan[section].contains(entry)) {
        const QString format = m_scan[section][entry]["format"];
        return printDataSingle(section, entry, format); }

    const QString message = QString("no such section '%1' or entry '%2' in the current scan")
            .arg(section).arg(entry);
    Q_ASSERT_X(false, AFUNC, qPrintable(message));
    return QString();
}

/*!
Returns the range data values of the given scan \a section and \a entry.
*/
const QString As::Scan::printDataRange(const QString &section,
                                       const QString &entry) const
{
    if (!m_scan[section].contains(entry)) {
        const QString message = QString("no such section '%1' or entry '%2' in the current scan")
                .arg(section).arg(entry);
        Q_ASSERT_X(false, AFUNC, qPrintable(message));
        return QString(); }

    const QString format = m_scan[section][entry]["format"];

    if (!format.contains("f"))
        return printDataSingle(section, entry, format);

    const As::RealVector data = m_scan[section][entry]["data"];
    const qreal min = data.min();
    const qreal max = data.max();

    if (qAbs(max/min) < 1.01 OR max-min < 0.01) {
        return printDataSingle(section, entry, format); }

    return QString().sprintf(qPrintable("%"+format + " - " + "%"+format), min, max);
}

/*!
Returns a list containing all the keys in the scan map in ascending order.
*/
const QStringList As::Scan::keys() const
{
    return m_scan.keys();
}

/*!
Returns the value associated with the \a key.
*/
const As::ScanSectionMap_t As::Scan::operator[](const QString &key) const
{
    return m_scan[key];
}
//const QMap<QString, QMap<QString, QString> > &operator[](const QString &akey) const;
//As::Scan &operator[](const int i);
//const As::Scan &operator[](const int i) const;

/*!
Returns the scan as a QMap.
*/
const As::ScanMap_t As::Scan::toQMap() const
{
    return m_scan;
}

/*!
Sets \a name to the scan angle.
*/
void As::Scan::setScanAngle(const QString &name)
{
    m_scanAngle = name;
}

/*!
Returns the name of the scan angle.
*/
const QString As::Scan::scanAngle() const
{
    return m_scanAngle;
}

/*!
Sets \a value to the scan step size.
*/
void As::Scan::setScanStep(const qreal value)
{
    m_scanStep = value;
}

/*!
Returns the scan step size.
*/
qreal As::Scan::scanStep() const
{
    return m_scanStep;
}

/*!
Sets \a value of the first line corresponding to scan data in the input file.
*/
void As::Scan::setScanLine(const int value)
{
    m_scanLine = value;
}

/*!
Returns the first line corresponding to scan data in the input file.
*/
int As::Scan::scanLine() const
{
    return m_scanLine;
}

/*!
Sets \a name to the absolute path of the file (including file name) which contains the scan.
*/
void As::Scan::setAbsoluteFilePath(const QString &name)
{
    m_absoluteFilePath = name;
}

/*!
Returns the absolute path of the file (including file name) which contains the scan.
*/
const QString As::Scan::absoluteFilePath() const
{
    return m_absoluteFilePath;
}

/*!
Returns the absolute path of the file (without file name) which contains the scan.
*/
const QString As::Scan::absolutePath() const
{
    // Absolute File Path:             /tmp/dummy.dat
    // Absolute Path:                  /tmp
    return QFileInfo(m_absoluteFilePath).absolutePath();
}

/*!
Returns the base name (without extension) of the file which contains the scan.
*/
const QString As::Scan::baseName() const
{
    // Absolute File Path:             /tmp/dummy.dat
    // Base Name:                      dummy
    return QFileInfo(m_absoluteFilePath).baseName();
}

/*!
Returns the absolute path of the file which contains the scan.
*/
const QString As::Scan::absolutePathWithBaseName() const
{
    /*
    Absolute File Path:             /tmp/dummy.dat
    Absolute Path:                  /tmp
    Base Name:                      dummy
    Absolute Path with Base Name:   /tmp/dummy
    */
    return QDir(absolutePath()).filePath(baseName());
}

/*!
Returns the absolute path (with base name and Miller indices h,k,l) of
the file which contains the scan.
*/
const QString As::Scan::absolutePathWithBaseNameAndHkl() const
{
    return QString("%1_hkl(%2,%3,%4)").
            arg(absolutePathWithBaseName()).
            arg(m_meanIndexH, 0, 'f', 3).
            arg(m_meanIndexK, 0, 'f', 3).
            arg(m_meanIndexL, 0, 'f', 3);
}

/*!
Sets the \a index of the scan file.
*/
void As::Scan::setFileIndex(const int index)
{
    m_fileIndex = index;
}

/*!
Returns the index of the file which contains the scan.
*/
int As::Scan::fileIndex() const
{
    return m_fileIndex;
}

/*!
Sets the \a index of the scan.
*/
void As::Scan::setIndex(const int index)
{
    m_index = index;
}

/*!
Returns the scan index.
*/
int As::Scan::index() const
{
    return m_index;
}

/*!
Sets the \a size (number of points) of the scan.
*/
void As::Scan::setSize(const int size)
{
    m_size = size;
}
//    if (m_size != size) {
//        m_size = size;
//        emit sizeChanged(size); } }

/*!
Returns the size (number of points) of the scan.
*/
int As::Scan::size() const
{
    return m_size;
}

/*!
Sets a scan plot type with the \a plotType.
*/
void As::Scan::setPlotType(const As::PlotType plotType)
{
    m_plotType = plotType;
}

/*!
Returns the scan plot type.
*/
As::PlotType As::Scan::plotType() const
{
    return m_plotType;
}

/*!
\variable As::Scan::BKG_TYPES
\brief the background determination types available for the scan.
*/
const QStringList As::Scan::BKG_TYPES = {
    "Automatically detect background",
    "Manually set background" };

/*!
\variable As::Scan::FIT_TYPES
\brief the peak profile fit types available for the scan.
*/
const QStringList As::Scan::FIT_TYPES = {
    "Gaussian function",
    "Lorentzian function",
    "Pseudo-Voigt function"};

/*!
Creates the table model of extracted scans.
*/
// Get rid of this in the scan!
void As::Scan::createExtractedTableModel_Slot()
{
    /*
    if (m_tableModel == tableModel)
        return;

    m_tableModel = tableModel;
    */

    QStringList headers;
    QStringList formats;
    QList<QStringList> dataTable;

    // Make a list of items to be potentially used in the table
    const QStringList items{"indices", "angles", "intensities", "conditions"};
    //////ADEBUG << items;

    // Go through all their subitems
    for (const QString &item : items) {
        const QStringList subitems = this->operator[](item).keys();
        /////ADEBUG << item << subitems;
        for (const QString &subitem : subitems) {
            bool ok;
            const QString str = data(item, subitem, &ok);
            //////ADEBUG << "----------------------" << item << subitem << ok;
            if (ok) {
                headers << subitem;
                dataTable << str.split(" ");
                formats << format(item, subitem); } } }

    ///ADEBUG;
    // Create tableModel
    // Number of columns and rows
    int columnCount = dataTable.size();
    ///ADEBUG << columnCount;
    int rowCount = dataTable[0].size();
    ///ADEBUG << rowCount;
    // Model
    m_tableModel = new QStandardItemModel;
    //auto m_tableModel = new QAbstractItemModel();

    ////scan.setExtractedTablesModels_Slot(new QStandardItemModel);
    ////m_tableModel->setRowCount(qMax(rowCount, 50));
    ////m_tableModel->setColumnCount(qMax(columnCount, 15));
    m_tableModel->setRowCount(rowCount);
    m_tableModel->setColumnCount(columnCount);
    ///ADEBUG;
    // Fill model
    for (int column = 0; column < columnCount; ++column) {
        // Headers
        m_tableModel->setHorizontalHeaderItem(column, new QStandardItem(headers[column]));
        ///ADEBUG << column;
        // Values
        for (int row = 0; row < rowCount; ++row) {
            //ADEBUG_H2 << column << row << dataTable[column][row] << As::FormatString(dataTable[column][row], formats[column]);
            //scan.m_tableModel->setItem(row, column, new QStandardItem(dataTable[column][row]));
            m_tableModel->setItem(row, column, new QStandardItem(As::FormatString(dataTable[column][row], formats[column])));
            m_tableModel->item(row, column)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); } }
    //emit extractedTablesModelsChanged_Signal(tableModel);
}

/*
const QHash<int, QString> As::Scan::BkgTypes{
    {As::Scan::autoBkg,  "Automatically detect background"},
    {As::Scan::manualBkg,"Manually set background"}};
*/

/**
Overloads operator<< for QDebug to accept the Scan output.
*/
QDebug operator<<(QDebug debug, const As::Scan &scan)
{
    return QtPrivate::printSequentialContainer(debug, "Scan", scan.toQMap());
}
