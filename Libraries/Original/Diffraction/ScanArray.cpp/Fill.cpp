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

#include <QtConcurrent>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "Macros.hpp"
#include "Functions.hpp"

#include "RealVector.hpp"
#include "Scan.hpp"

#include "ScanArray.hpp"

/*!
Fills single array with single element...
*/
void As::ScanArray::fillSingleEmptyArray(As::Scan *scan) {

    // Define the total measured intensities and times based on up and down polarized measurements
    calcUnpolData("intensities", "Detector",  scan);
    calcUnpolData("intensities", "Monitor",   scan);
    calcUnpolData("conditions",  "Time/step", scan);

    ///// test
    ///As::RealVector detector  = (*scan)["intensities"]["Detector"]["data"];
    ///scan->setData("intensities", "Detector", detector.toQString());

    // Set some common parameters
    // find a better way to get the size of all the scans!
    // numPoints?
    int size = (*scan)["intensities"]["Detector"]["data"].split(" ").size(); // returns 1 for "" !!! fix
    scan->setData("conditions", "Points count", QString::number(size));
    // or ???
    scan->setSize(size);

    // Set McCandlish factor depends on the instrument
    scan->m_mcCandlishFactor = As::MC_CANDLISH_FACTOR[filesType()];

    // Add zeros to empty but required variables

    // 4-circle geometry...
    if (!(*scan)["angles"]["2Theta"]["data"].isEmpty()) {
        QStringList subitemKeys = {"Omega", "Chi", "Phi"};
        for (const auto &subitemKey : subitemKeys) {
            if ((*scan)["angles"][subitemKey]["data"].isEmpty())
                scan->setData("angles", subitemKey, "0"); } }

    // Liffting counter geometry...
    if (!(*scan)["angles"]["Gamma"]["data"].isEmpty()) {
        QStringList subitemKeys = {"Nu", "Omega"};
        for (const auto &subitemKey : subitemKeys) {
            if ((*scan)["angles"][subitemKey]["data"].isEmpty())
                scan->setData("angles", subitemKey, "0"); } }

    // Fill arrays with existing single values
    QStringList itemKeys = {"angles", "conditions", "indices", "intensities"};
    for (const auto &itemKey : itemKeys) {
        QStringList subitemKeys = (*scan)[itemKey].keys();
        for (const auto &subitemKey : subitemKeys) {
            QString data = (*scan)[itemKey][subitemKey]["data"];
            if (!data.contains(" ")) {
                QStringList list;
                for (int i = 0; i < size; ++i) {
                    list.append(data); }
                scan->setData(itemKey, subitemKey, list.join(" ")); } } }

    // Add batch number. All the reflections are considered to belong to just 1st group...
    scan->setData("number", "Batch", "1");
}

/*!
Fills all the arrays with single element...
*/
// Using multi-threading: Not needed here!? It was less than 0.1s...
void As::ScanArray::fillEmptyArrays()
{
    ADEBUG_H2;

    // Futures for multi-threaded for-loop, to check later when the tasks are completed
    QList<QFuture<void> > futures;

    // For every scan in the scan array
    for (auto scan : m_scanArray) {
        // Schedule task to the global thread pool
        auto future = QtConcurrent::run(this, &As::ScanArray::fillSingleEmptyArray, scan);
        futures.append(future); }

    // Wait for all the tasks to be completed
    for (auto future : futures)
        future.waitForFinished();

    ADEBUG;
}

/*!
Sets the unpolarised neutron data based on the polarised neutron diffraction
measurement for the given \a scan using the provided \a section and \a entry.
*/
void As::ScanArray::calcUnpolData(const QString &section,
                                  const QString &entry,
                                  As::Scan *scan)
{
    bool okUp, okDown;

    const As::RealVector up   = scan->data(section, entry + COUNT_TYPES[1], &okUp);
    const As::RealVector down = scan->data(section, entry + COUNT_TYPES[2], &okDown);

    if (!okUp OR !okDown)
        return;

    if (up.size() != down.size() OR up.size() == 0)
        return;

    const As::RealVector sum  = up + down;

    scan->setData(section, entry, sum.toQString());
}

/*!
Creates the table models for the extracted scans.
*/
// Using multi-threading
void As::ScanArray::createAllExtractedTablesModels()
{
    ADEBUG;

    //for (auto scan : m_scanArray)
    //    scan->createExtractedTableModel_Slot();

    // Futures for multi-threaded for-loop, to check later when the tasks are completed
    QList<QFuture<void> > futures;

    // For every scan in the scan array
    for (auto scan : m_scanArray) {
        // Schedule task to the global thread pool
        auto future = QtConcurrent::run(scan, &As::Scan::createExtractedTableModel_Slot);
        futures.append(future); }

    // Wait for all the tasks to be completed
    for (auto future : futures)
        future.waitForFinished();

    ADEBUG;
}


