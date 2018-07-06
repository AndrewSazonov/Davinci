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
#include "ScanDatabase.hpp"

#include "ScanArray.hpp"

/*!
\class As::ScanArray

\brief The ScanArray is a class that provides an array of the
measured scans.

\inmodule Diffraction
*/

/*!
Constructs an empty array of the scans with the given \a parent.
*/
As::ScanArray::ScanArray(QObject *parent)
    : QObject(parent) {}

/*!
Destroys the array.
*/
As::ScanArray::~ScanArray()
{
    ADESTROYED;
}

/*!
Returns a pointer to the modifiable As::Scan at index position \a i in the base
array As::ScanArray.

\a i must be a valid index position in the array (i.e., 0 <= \a i < size()).

\sa operator[]()
*/
As::Scan *As::ScanArray::at(const int i)
{
    const QString message = QString("index out of range (index: '%1', size: '%2')").arg(i).arg(size());
    Q_ASSERT_X(i >= 0 AND i < size(), AFUNC, qPrintable(message));
    return m_scanArray[i];
}

/*!
\overload
*/
const As::Scan *As::ScanArray::at(const int i) const
{
    const QString message = QString("index out of range (index: '%1', size: '%2')").arg(i).arg(size());
    Q_ASSERT_X(i >= 0 AND i < size(), AFUNC, qPrintable(message));
    return m_scanArray.begin()[i];
}

/*!
\overload

Same as at().
*/
As::Scan *As::ScanArray::operator[](const int i)
{
    const QString message = QString("index out of range (index: '%1', size: '%2')").arg(i).arg(size());
    Q_ASSERT_X(i >= 0 AND i < size(), AFUNC, qPrintable(message));
    return m_scanArray[i];
}

/*!
\overload

Same as at().
*/
const As::Scan *As::ScanArray::operator[](const int i) const
{
    const QString message = QString("index out of range (index: '%1', size: '%2')").arg(i).arg(size());
    Q_ASSERT_X(i >= 0 AND i < size(), AFUNC, qPrintable(message));
    return m_scanArray.begin()[i];
}

/*!
Returns the number of elements in the array.
*/
int As::ScanArray::size() const
{
    return m_scanArray.size();
}

/*!
Returns a pointer to the first item in the array.
*/
As::Scan *As::ScanArray::begin()
{
    return *m_scanArray.begin();
}

/*!
Returns a pointer to the imaginary item after the last item in the array.
*/
As::Scan *As::ScanArray::end()
{
    return *m_scanArray.end();
}

/*!
Inserts \a scan at the end of the array.
*/
void As::ScanArray::append(As::Scan *scan)
{
    m_scanArray.QVector::append(scan);
}

/*!
Removes all the elements from the array.
*/
void As::ScanArray::clear()
{
    m_scanArray.clear();
}

/*!
Sets the index of the currently processed scan to be \a index.
*/
void As::ScanArray::setScanIndex(const int index)
{
    ADEBUG << "old index:" << m_scanIndex << ", new index:" << index;

    if (m_scanIndex == index)
        return;

    m_scanIndex = index;

    emit scanIndexChanged(m_scanIndex);

    /*
    if (m_scanIndex != index) {
        m_scanIndex = index;

        const int chunkInPercent = 5;
        const int chunkInNumber = qMax(1, size() * chunkInPercent / 100);

        if (index % chunkInNumber == 0 OR index + 1 == size()) {
            const int chunk = (index + 1) * 100 / size();
            emit scanIndexChanged(chunk); } }
    */
}

/*!
Returns the index of the currently processed scan.
*/
int As::ScanArray::scanIndex() const
{
    return m_scanIndex;
}

/*!
Sets the index of the file which contains the currently processed scan
to be \a index.
*/
void As::ScanArray::setFileIndex(const int index)
{
    ADEBUG << "old index:" << m_fileIndex << ", new index:" << index;

    // fix it! the 1st new file does not shows up, if open new files...
    if (m_fileIndex == index)
        return;

    m_fileIndex = index;
    emit fileIndexChanged(m_fileIndex);
}

/*!
Sets the index of the file which contains the currently
processed scan using the scan \a index.
*/
void As::ScanArray::setFileIndexByScanIndex(const int index)
{
    ADEBUG << "index" << index;

    setFileIndex(m_scanArray.at(index)->fileIndex());
}

/*!
Returns the index of the file which contains the currently processed scan.
*/
int As::ScanArray::fileIndex() const
{
    return m_fileIndex;
}

/*!
Sets the selected columns for the output \a table according to the given
headers \a saveHeaders.
*/
void As::ScanArray::setSelectedOutputColumns(As::SaveHeaders &saveHeaders,
                                             QString &table)
{
    ADEBUG;

    // Read global settings: Save excluded scan
    auto exportExcluded = QSettings().value("OutputSettings/exportExcluded", false).toBool();

    // Index of the exclude current scan column
    int indexOfExcluded = m_outputTableHeaders.indexOf("Excluded");

    // Write down the table headers
    if (saveHeaders.m_addHeader) {
        for (const QString &header : saveHeaders.m_name) {
            const int i = saveHeaders.m_name.indexOf(header);
            table.append(As::FormatStringToText(header, saveHeaders.m_format[i])); }
        table.append("\n"); }

    // Write down the table data
    for (const QStringList &row : m_outputTableData) {

        // Read the exclude current scan flag
        const bool isCurrentScanExcluded = QString(row[indexOfExcluded]).toInt();

        // Conditions to jump to the next row in the output table
        if (!exportExcluded AND isCurrentScanExcluded)
            goto nextrow;

        // Add data cell by cell
        // What if empty cell?
        for (const QString &header : saveHeaders.m_name) {
            QString stringNum = "0"; // default value, if header is not found
            if (m_outputTableHeaders.contains(header))
                stringNum = row[m_outputTableHeaders.indexOf(header)];
            const QString format = saveHeaders.m_format[saveHeaders.m_name.indexOf(header)];
            table.append(As::FormatString(stringNum, format));
            /*if (m_outputTableHeaders.contains(header)) {
                const int k = m_outputTableHeaders.indexOf(header);
                const int i = saveHeaders.m_name.indexOf(header);
                table.append(As::FormatString(row[k], saveHeaders.m_format[i])); } */

        }

        // Go to the new line
        table.append("\n");
        nextrow: continue; }

    // Remove extra ';' at the end of each line (if any) (after csv format)
    //text.replace(";\n", "\n"); /// select ';' or ',' depends on the localisation? move it to setSelectedOutputColumns?

    // Replace empty lines (if any) (after skipping of excluded scans)
    //text.replace("\n\n", "\n");

    /*
    // Index of the exclude current scan column
    int columnIndexOfExcluded = outputTableHeaders.indexOf("Excluded");

    // Determine the headers' indices
    for (const auto header : saveHeaders.m_name) {
        int index = outputTableHeaders.indexOf(header);
        if (index > -1) {
            saveHeaders.m_index << index; } }

    // Write down the table data
    for (int rowIndex = 0; rowIndex < outputTableData->rowCount(); ++rowIndex) {
        for (int i = 0; i < saveHeaders.m_index.size(); ++i) {

            // Read the cell's content
            int columnIndex = saveHeaders.m_index[i];
            const QModelIndex index = outputTableData->index(rowIndex, columnIndex);
            const QString cell = outputTableData->data(index).toString();

            // Read the exclude current scan flag
            const QModelIndex indexOfExcluded = outputTableData->index(rowIndex, columnIndexOfExcluded);
            const bool isCurrentScanExcluded = outputTableData->data(indexOfExcluded).toBool();

            // Conditions to jump to the next row in the output table
            if (!exportExcluded AND isCurrentScanExcluded)
                goto nextrow;
            if (cell.isEmpty()) /// what if an empty cell is in the middle of the row?
                goto nextrow;

            // Save the formatted cell to the row, if above conditions are satisfied
            textStream << As::FormatString(cell, saveHeaders.m_format[i]); }

        // Go to the new line
        textStream << endl;
        nextrow: continue; }
    */
}

/*!
Saves the selected columns for the output file \a fileName according to the
given \a filter.
*/
void As::ScanArray::saveSelectedOutputColumns(const QString &fileName,
                                              const QString &filter)
{
    ADEBUG;

    // Open file for writing
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    // Define structure for the columns to be saved
    As::SaveHeaders saveHeaders(filter, m_outputTableHeaders);
    // Define a text variable to append the required info via text stream
    QString table;
    // Write down the table data to the text variable
    setSelectedOutputColumns(saveHeaders, table);
    // Save the text to file
    QTextStream stream(&file);
    stream << table;
    // Close file
    file.close();
}

