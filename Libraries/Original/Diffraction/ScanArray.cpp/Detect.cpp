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

#include "Constants.hpp"
#include "Macros.hpp"

#include "ScanArray.hpp"

/*!
    Returns true if the input file type is detected.
*/
bool As::ScanArray::detectInputFilesType() {
    ADEBUG;

    QList<QStringList> filesAsListOfStringLists;

    for (const QString fileAsString : m_inputFilesContents.second) {
        filesAsListOfStringLists << fileAsString.split("\n"); }

    // Go through all the files to get the list of all the opened file types
    QList<As::InputFileType> detectedTypes;

    for (const QStringList& fileAsListOfStrings : filesAsListOfStringLists) {
        As::InputFileType type = As::InputFileType(0);

        for (const QString& str : fileAsListOfStrings) {

            // Check the file content line by line
            if (str.startsWith("### NICOS data file")) {
                type = As::InputFileType::NICOS_DAT;
                break; }

            else if (str.endsWith("4-CIRCLE DIFFRACTOMETER CONTROL PROGRAM") || str.endsWith("Protocol ON")) {
                type = As::InputFileType::HEIDI_LOG;
                break; }

            else if (str.endsWith("Rev HEIDI/FRM2")) {
                type = As::InputFileType::HEIDI_DAT;
                break; }

            else if (str.startsWith("  => Now executing the cmd GEO")) {
                type = As::InputFileType::POLI_LOG;
                break; }

            else if (str.contains("<manip>6T2</manip>")) {
                type = As::InputFileType::S6T2_DAT;
                break; } }

        detectedTypes << type; }

    // Get the size of the detectedTypes list converted to set and back to list in order to remove duplicates
    int size = detectedTypes.toSet().toList().size();

    // Set m_inputFilesType depends on the size of the detectedTypes list
    if (size == 0) {
        setInputFileType(As::InputFileType(0));
        return true; }

    else if (size == 1) {
        setInputFileType(detectedTypes[0]);
        return true; }

    else {
        setInputFileType(As::InputFileType(0));
        return false; } }

/*!
    Sets the input file type to be \a type.
*/
void As::ScanArray::setInputFileType(const As::InputFileType type) {
    ADEBUG << "type:" << type;

    if (m_inputFilesType == type) {
        return; }

    m_inputFilesType = type;

    switch (m_inputFilesType) {

        case As::InputFileType::UNKNOWN_FILE:
            m_facilityType = "Unknown";
            m_instrumentType = "Unknown";
            m_dataType = "Unknown";
            break;

        case As::InputFileType::HEIDI_DAT:
            m_facilityType = "MLZ/FRMII";
            m_instrumentType = "HEiDi";
            m_dataType = "DIF4 dat";
            break;

        case As::InputFileType::HEIDI_LOG:
            m_facilityType = "MLZ/FRMII";
            m_instrumentType = "HEiDi";
            m_dataType = "DIF4 log";
            break;

        case As::InputFileType::NICOS_DAT:
            m_facilityType = "MLZ/FRMII";
            m_instrumentType = "POLI";
            m_dataType = "NICOS dat";
            break;

        case As::InputFileType::POLI_LOG:
            m_facilityType = "MLZ/FRMII";
            m_instrumentType = "POLI";
            m_dataType = "IgorPro log";
            break;

        case As::InputFileType::S6T2_DAT:
            m_facilityType = "LLB";
            m_instrumentType = "6T2";
            m_dataType = "NEW xml";
            break; }

    emit facilityTypeChanged(m_facilityType);
    emit instrumentTypeChanged(m_instrumentType);
    emit dataTypeChanged(m_dataType); }




