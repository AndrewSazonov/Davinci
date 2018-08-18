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
void As::ScanArray::fillMissingDataArray(const int index) {
    auto scan = at(index);

    // Define the total measured intensities and times based on up and down polarized measurements
    calcUnpolData("intensities", "Detector",  scan);
    calcUnpolData("intensities", "Monitor",   scan);
    calcUnpolData("conditions",  "Time/step", scan);

    // Set some common parameters
    scan->setData("conditions", "Points count", QString::number(scan->numPoints()));

    // Set McCandlish factor depends on the instrument
    scan->setMcCandlishFactor( As::ScanDict::MC_CANDLISH_FACTOR[ m_inputFilesType ] );

    // Add zeros to empty but required variables depends on the instrument geometry
    QStringList elements;

    if (!scan->data("angles", "2Theta").isEmpty()) {            // 4-circle geometry
        elements = QStringList({"Omega", "Chi", "Phi" }); }

    if (!scan->data("angles", "Gamma").isEmpty()) {             // Liffting counter geometry
        elements = QStringList({"Nu", "Omega" }); }

    for (const auto& element : elements) {
        if (scan->data("angles", element).isEmpty()) {
            scan->setData("angles", element, "0"); } }

    // Fill arrays with existing single values
    const QStringList groups = { "angles", "conditions", "indices", "intensities" };

    for (const auto& group : groups) {
        const QStringList elements = (*scan)[group].keys();

        for (const auto& element : elements) {
            const QString data = scan->data(group, element);

            if (!data.contains(" ")) {
                QStringList list;

                for (int i = 0; i < scan->numPoints(); ++i) {
                    list.append(data); }

                scan->setData(group, element, list.join(" ")); } } }

    // All the reflections are considered to belong to just 1st group...
    scan->setData("number", "Batch", "1");

    scan->createExtractedTableModel(); }

/*!
    Sets the unpolarised neutron data based on the polarised neutron diffraction
    measurement for the given \a scan using the provided \a group and \a element.
*/
void As::ScanArray::calcUnpolData(const QString& group,
                                  const QString& element,
                                  As::Scan* scan) {
    bool okUp, okDown;

    const QString typeUp   = As::ScanDict::BEAM_TYPES[As::ScanDict::POLARISED_UP];
    const QString typeDown = As::ScanDict::BEAM_TYPES[As::ScanDict::POLARISED_DOWN];

    ADEBUG << (*scan)["intensities"]["Detector(+)"]["data"];
    ADEBUG << scan->data("intensities", "Detector(+)");
    ADEBUG << scan->data(group, element + typeUp);
    ADEBUG << scan->data(group, element + typeUp, &okUp) << okUp;

    const As::RealVector up   = scan->data(group, element + typeUp, &okUp);
    const As::RealVector down = scan->data(group, element + typeDown, &okDown);
    ADEBUG << up;

    if (!okUp OR !okDown) {
        return; }

    if (up.size() != down.size() OR up.size() == 0) {
        return; }

    //const As::RealVector sum = up + down; // operator+ was modified for As::RealArray
    As::RealVector sum;
    for (int i = 0; i < up.size(); ++i) {
        sum.append(up[i] + down[i]); }

    scan->setData(group, element, sum.toQString()); }
