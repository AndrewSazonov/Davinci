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

#include <QDir>
#include <QFileInfo>
#include <QSettings>

#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "RealMatrix9.hpp"
#include "RealVector.hpp"
#include "SaveHeaders.hpp"
#include "Scan.hpp"
#include "ScanDict.hpp"

#include "ScanArray.hpp"

#include <QtConcurrent>

/*!
    \class As::ScanArray

    \brief The ScanArray is a class that provides an array of the measured scans.

    \inmodule Diffraction
*/

/*!
    Constructs an empty array of the scans with the given \a parent.
*/
As::ScanArray::ScanArray(QObject* parent)
    : QObject(parent) {}

/*!
    Destroys the array.
*/
As::ScanArray::~ScanArray() {
    ADESTROYED; }

/*!
    Returns a pointer to the modifiable As::Scan at index position \a i in the base
    array As::ScanArray.

    \a i must be a valid index position in the array (i.e., 0 <= \a i < size()).

    \sa operator[]()
*/
As::Scan* As::ScanArray::at(const int i) {
    AASSERT(i >= 0 AND i < size(), QString("index out of range (index: '%1', size: '%2')").arg(i).arg(size()));
    return m_scanArray[i]; }

/*!
    \overload
*/
const As::Scan* As::ScanArray::at(const int i) const {
    AASSERT(i >= 0 AND i < size(), QString("index out of range (index: '%1', size: '%2')").arg(i).arg(size()));
    return m_scanArray.begin()[i]; }

/*!
    \overload

    Same as at().
*/
As::Scan* As::ScanArray::operator[](const int i) {
    AASSERT(i >= 0 AND i < size(), QString("index out of range (index: '%1', size: '%2')").arg(i).arg(size()));
    return m_scanArray[i];

    float x[] = {1.3f, 2.5f, 4.6f };
    QFuture<void> f = QtConcurrent::map(x, x + 3, [](float & a) {
        a = 2 * a; });
    f.waitForFinished();
    qDebug() << x[0] << x[1] << x[2];

}

/*!
    \overload

    Same as at().
*/
const As::Scan* As::ScanArray::operator[](const int i) const {
    AASSERT(i >= 0 AND i < size(), QString("index out of range (index: '%1', size: '%2')").arg(i).arg(size()));
    return m_scanArray.begin()[i]; }

/*!
    Returns the number of elements in the array.
*/
int As::ScanArray::size() const {
    return m_scanArray.size(); }

/*!
    Returns a pointer to the first item in the array.
*/
As::Scan* As::ScanArray::begin() {
    return *m_scanArray.begin(); }

/*!
    Returns a pointer to the imaginary item after the last item in the array.
*/
As::Scan* As::ScanArray::end() {
    return *m_scanArray.end(); }

/*!
    Inserts \a scan at the end of the array.
*/
void As::ScanArray::append(As::Scan* scan) {
    m_scanArray.append(scan);
    const int i = m_scanArray.size();
    scan->setData("number", "Scan", QString::number(i)); }

/*!
    Removes all the elements from the array.
*/
void As::ScanArray::clear() {
    m_scanArray.clear(); }

/*!
    Sets the index of the currently processed scan to be \a index.
*/
void As::ScanArray::setScanIndex(const int index) {
    ADEBUG << "old index:" << m_scanIndex << ", new index:" << index;

    if (m_scanIndex == index) {
        return; }

    m_scanIndex = index;

    emit scanIndexChanged(m_scanIndex); }

/*!
    Returns the index of the currently processed scan.
*/
int As::ScanArray::scanIndex() const {
    return m_scanIndex; }

/*!
    Sets the index of the file which contains the currently processed scan to be \a index.
*/
void As::ScanArray::setFileIndex(const int index) {
    ADEBUG << "old index:" << m_fileIndex << ", new index:" << index;

    // fix it! the 1st new file does not shows up, if open new files...
    if (m_fileIndex == index) {
        return; }

    m_fileIndex = index;
    emit fileIndexChanged(m_fileIndex); }

/*!
    Sets the index of the file which contains the currently
    processed scan using the scan \a index.
*/
void As::ScanArray::setFileIndexByScanIndex(const int index) {
    ADEBUG << "index" << index;

    setFileIndex(at(index)->fileIndex()); }

/*!
    Returns the index of the file which contains the currently processed scan.
*/
int As::ScanArray::fileIndex() const {
    return m_fileIndex; }

/*!
    Sets the selected columns for the output \a table according to the given
    headers \a saveHeaders.
*/
void As::ScanArray::setSelectedOutputColumns(As::SaveHeaders& saveHeaders,
                                             QString& table) {
    ADEBUG;

    auto exportExcluded = QSettings().value("OutputSettings/exportExcluded", false).toBool();
    int indexOfExcluded = m_outputTableHeaders.indexOf("Excluded");

    // Set the table headers
    if (saveHeaders.m_addHeader) {
        for (const QString& header : saveHeaders.m_name) {
            const int i = saveHeaders.m_name.indexOf(header);
            table.append(As::FormatStringToText(header, saveHeaders.m_format[i])); }
        table.append("\n"); }

    // Set the table data
    for (const QStringList& row : m_outputTableData) {
        const bool isCurrentScanExcluded = QString(row[indexOfExcluded]).toInt();

        // Conditions to jump to the next row in the output table
        if (!exportExcluded AND isCurrentScanExcluded) {
            goto nextrow; }

        // Add data cell by cell. What if cell is empty?
        for (const QString& header : saveHeaders.m_name) {
            QString stringNum = "0"; // default value, if header is not found

            if (m_outputTableHeaders.contains(header)) {
                stringNum = row[m_outputTableHeaders.indexOf(header)]; }

            const QString format = saveHeaders.m_format[saveHeaders.m_name.indexOf(header)];
            table.append(As::FormatString(stringNum, format)); }

        // Go to the new line
        table.append("\n");

nextrow: continue; } }

/*!
    Saves the selected columns for the output file \a fileName according to the
    given \a filter.
*/
void As::ScanArray::saveSelectedOutputColumns(const QString& fileName,
                                              const QString& filter) {
    ADEBUG;

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    // Define structure for the columns to be saved
    As::SaveHeaders saveHeaders(filter, m_outputTableHeaders);

    // Define a text variable to append the required info via text stream
    // Write down the table data to the text variable
    QString table;
    setSelectedOutputColumns(saveHeaders, table);

    QTextStream stream(&file);
    stream << table;

    file.close(); }

