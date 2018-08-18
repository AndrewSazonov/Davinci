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

#include <QDateTime>
#include <QPointer>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QtMath>

#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "RealVector.hpp"
#include "Scan.hpp"
#include "StringParser.hpp"

#include "ScanArray.hpp"

/*!
    ...
*/
void As::ScanArray::extractDataFromFile(const int index) {
    //ADEBUG_H2 << index;

    const QString& filePath    = m_inputFilesContents.first[index];
    const QString& fileContent = m_inputFilesContents.second[index];

    switch (m_inputFilesType) {

        case UNKNOWN_FILE:
            ADEBUG << "Not implemented yet"; break;

        case HEIDI_DAT:
            extractHeidiData(index, filePath, fileContent);
            break;

        case HEIDI_LOG:
            extractHeidiLog(index, filePath, fileContent);
            break;

        case NICOS_DAT:
            extractNicosData(index, filePath, fileContent);
            break;

        case POLI_LOG:
            extractPoliLog(index, filePath, fileContent);
            break;

        case S6T2_DAT:
            extract6t2Data(index, filePath, m_inputFilesContents.second[index]); // as file content to be modified
            break;

        default:
            ADEBUG << "Not implemented yet"; break; }

}

/*!
    Extracts the scans from the single HEIDI data file.
*/
void As::ScanArray::extractHeidiData(const int fileIndex,
                                     const QString& filePath,
                                     const QString& fileContent) {
    ADEBUG;

    // Get file content as a list of strings
    const QStringList file = fileContent.split("\n");

    // Mixed groups
    const int nHeaderLines = 7;

    for (int i = nHeaderLines; i < file.size(); ++i) {
        if (!file[i].isEmpty()) {

            // Variables

            auto scan = new As::Scan; // scan to be added to the scan array
            As::StringParser string;  // string parser
            QStringList list = file[i].split(" ", QString::SkipEmptyParts); // split every line

            // Set common data for all the scans in the scan array

            // Get the index of the file which contains the current scan
            scan->setFileIndex(fileIndex + 1);

            // Set the absolute file path of the file which contains the scan
            scan->setAbsoluteFilePath(filePath);

            // Read and set the conditions and orientation groups
            scan->setData("conditions", "Wavelength", string.parseNumberNearText("Wave=", "txt:num", file));
            scan->setData("orientation", "matrix", string.parseString("Omat=", "num", file, 0, 3));

            // Set individual data for every scan in the scan array

            // Indices group
            const int iH = 0; scan->setData("indices", "H", list[iH]);
            const int iK = 1; scan->setData("indices", "K", list[iK]);
            const int iL = 2; scan->setData("indices", "L", list[iL]);

            // Psi angle
            const int iPsi = 6; scan->setData("angles", "Psi", list[iPsi]);

            // Conditions group
            const int iTimePerStep = 8; scan->setData("conditions", "Time/step", QString::number(list[iTimePerStep].toDouble() / 10));
            const int iTemperature = 10; scan->setData("conditions", "Temperature", list[iTemperature]);

            // Supplementary data
            const int iValuesPerBlock = 7;
            const int nValuesPerBlock = list[iValuesPerBlock].toInt();

            // Read scan step size
            if (i + 1 < file.size()) {
                list = file[i + 1].split(" ", QString::SkipEmptyParts);
                const int iScanStep = 3;

                if (list.size() > iScanStep) {
                    bool ok;
                    scan->setScanStep(list[iScanStep].toDouble(&ok)); } }

            // Define scan angle name
            scan->setScanAngle("Omega");

            // Scandata group. Make own headers, as file has no any
            scan->setData("scandata", "headers", "Detector Monitor");

            // Make a header map between the possible internal names and headers created above
            QList<QStringList> headerMap;
            headerMap.append({"intensities", "Detector", "Detector" });
            headerMap.append({"intensities", "Monitor",  "Monitor" });

            // Define position of the data to be read
            const int nValuesPerLine = 16;
            const int nCharsPerValue = 5;
            const int nBlocksPerData = 2;
            const int nLinesPerData = qCeil(static_cast<qreal>(nValuesPerBlock) / nValuesPerLine * nBlocksPerData);
            const int nLinesToSkip = 2;
            const int iEnd = i + nLinesToSkip + nLinesPerData - 1;

            if (iEnd < file.size()) {
                scan->setData("scandata", "data", string.parseString(i, "raw", file, nLinesToSkip, nLinesPerData));
                scan->setData("scandata", "data", string.splitText(nCharsPerValue, nBlocksPerData));

                // Read and set data from the scan table created above according to the header map
                extractDataFromTable(scan, headerMap);

                // Append scan line numbers
                QStringList lines;

                for (int k = i + nLinesToSkip; k <= iEnd; ++k) {
                    lines << QString::number(k); }

                scan->setData("misc", "lines", lines.join(" "));

                // Append single scan to the scan array
                appendScan(scan); }

            i = iEnd; } } }

/*!
    Extracts the scans from the single HEIDI log file.
*/
void As::ScanArray::extractHeidiLog(const int fileIndex,
                                    const QString& filePath,
                                    const QString& fileContent) {
    ADEBUG;

    // Make a header map between the possible internal names and HEIDI log parameter names
    QList<QStringList> headerMap;
    headerMap.append({"angles",      "2Theta",          "2Theta" });
    headerMap.append({"angles",      "Omega",           "Omega" });
    headerMap.append({"angles",      "Phi",             "Phi" });
    headerMap.append({"angles",      "Chi",             "Chi" });
    headerMap.append({"intensities", "Detector",        "Idet" });
    headerMap.append({"intensities", "Monitor",         "Imon" });

    const QRegExp re("[^-.0-9]");     // regular expression for the splitting: all characters but "-", ".", "0123456789"
    QStringList list;
    QString str;

    QString wavelength = "1";
    QString timePerStep = "1";
    QString matrix;

    // Get file content as a list of strings
    const QStringList file = fileContent.split("\n");

    // Go through every line
    for (int i = 0; i < file.size(); ++i) {

        // Read line
        str = file[i];

        // Get wavelength
        // both cases: "Wavelength [0.79350] ?" and "Wavelength [0.79350] ? 1.169"
        if (str.simplified().startsWith("Wavelength")) {
            list = str.split(re, QString::SkipEmptyParts);
            wavelength = list[list.size() - 1]; }

        // Get time per step
        // both cases: "Time/step :   1.00 sec ?" and "Time/step :   1.00 sec ? 2"
        if (str.simplified().startsWith("Time/step :")) {
            list = str.split(re, QString::SkipEmptyParts);
            timePerStep = list[list.size() - 1]; }

        // Get orientation matrix: 1st type (after ro, rc, ol, pm commands)
        if (str.simplified().startsWith("Orienting")) {
            matrix = str.replace(re, " ");
            str = file[++i];
            matrix += str.replace(re, " ");
            str = file[++i];
            matrix += str.replace(re, " "); }

        // Get orientation matrix: 2nd type (after mr command)
        if (str.simplified().startsWith("Refined orienting matrix")) {
            str = file[++i];
            matrix = str;
            str = file[++i];
            matrix += str;
            str = file[++i];
            matrix += str; }

        // Get scan data (after ss command)
        if (str.simplified().startsWith("Scan centre =")) {

            // Variables
            auto scan = new As::Scan; // scan to be added to the scan array

            // Set parameters of the scan defined extracted above
            scan->setData("conditions", "Wavelength", wavelength);
            scan->setData("conditions", "Time/step", timePerStep);
            scan->setData("orientation", "matrix", matrix);

            // Set scan angles
            list = str.split(re, QString::SkipEmptyParts);
            scan->setData("angles", "2Theta", list[0]);
            scan->setData("angles", "Omega", list[1]);
            scan->setData("angles", "Chi", list[2]);
            scan->setData("angles", "Phi", list[3]);

            // Read data headers
            str = file[++i];
            str = file[++i];
            scan->setData("scandata", "headers", str.section('|', 0, 0));

            // Read data table
            str = file[++i];
            str = file[++i];
            QString data;
            QString lines;

            while (!str.simplified().startsWith("Centre at point") AND !str.simplified().startsWith("#")) {
                QRegularExpression re("[|+]");

                if (str.contains(re)) {
                    data.append(str.section(re, 0, 0) + "\n");
                    lines.append(QString::number(i) + " "); }

                str = file[++i]; }

            scan->setData("scandata", "data", data);
            scan->setData("misc", "lines", lines);

            // Get the index of the file which contains the current scan
            scan->setFileIndex(fileIndex + 1);

            // Set the absolute file path of the file which contains the scan
            scan->setAbsoluteFilePath(filePath);

            // Read and set data from the scan table created above according to the header map
            extractDataFromTable(scan, headerMap);

            // Define scan angle name
            scan->findAndSetScanAngle();

            // Append single scan to the scan array
            appendScan(scan); } } }

/*!
    Extracts the scan from the single NICOS data file.
*/
void As::ScanArray::extractNicosData(const int fileIndex,
                                     const QString& filePath,
                                     const QString& fileContent) {
    // Make a header map between the possible internal names and NICOS parameter names
    QList<QStringList> headerMap;
    headerMap.append({"angles",        "Chi1",          "chi1" });
    headerMap.append({"angles",        "Chi2",          "chi2" });
    headerMap.append({"angles",        "Gamma",         "gamma" });
    headerMap.append({"angles",        "2Theta",        "twotheta" });
    headerMap.append({"angles",        "Nu",            "liftingctr" });
    headerMap.append({"angles",        "Omega",         "omega|sth" });
    headerMap.append({"angles",        "Psi",           "psi_virtual" });
    headerMap.append({"indices",       "H",             "h" });
    headerMap.append({"indices",       "K",             "k" });
    headerMap.append({"indices",       "L",             "l" });
    headerMap.append({"conditions",    "Temperature",   "Ts" });
    headerMap.append({"conditions",    "Magnetic field", "B" });
    headerMap.append({"conditions",    "Electric field", "fug" });
    headerMap.append({"conditions",    "Wavelength",    "wavelength" });
    // Append additional possible header names to the header map (apriori not single numbers in the scan table)
    headerMap.append({"conditions",    "Time/step",     "timer" });
    headerMap.append({"conditions",    "Time/step(+)",  "timer_up" });
    headerMap.append({"conditions",    "Time/step(-)",  "timer_dn" });
    headerMap.append({"intensities",   "Detector",      "ctr1" });
    headerMap.append({"intensities",   "Detector(+)",   "ctr1_up" });
    headerMap.append({"intensities",   "Detector(-)",   "ctr1_dn" });
    headerMap.append({"intensities",   "Monitor1",      "mon1" });
    headerMap.append({"intensities",   "Monitor1(+)",   "mon1_up" });
    headerMap.append({"intensities",   "Monitor1(-)",   "mon1_dn" });
    headerMap.append({"intensities",   "Monitor2",      "mon2" });
    headerMap.append({"intensities",   "Monitor2(+)",   "mon2_up" });
    headerMap.append({"intensities",   "Monitor2(-)",   "mon2_dn" });
    headerMap.append({"polarisation",  "Pin",           "Pin" });
    headerMap.append({"polarisation",  "Pout",          "Pout" });
    headerMap.append({"polarisation",  "Fin",           "Fin" });
    headerMap.append({"polarisation",  "Fout",          "Fout" });

    // Variables
    const QStringList file = fileContent.split("\n"); // every file content as a list of strings
    auto scan = new As::Scan; // scan to be added to the scan array
    //QPointer<As::Scan> scan = new As::Scan;
    As::StringParser string; // string parser

    // Set the index of the file which contains the current scan
    scan->setFileIndex(fileIndex + 1);

    // Set the absolute file path of the file which contains the scan
    scan->setAbsoluteFilePath(filePath);

    // Read (single numbers if any) and set data for the NICOS variables according to the map created above
    // slow. 30 headerMap lines x 350 files = 10000 iterations
    for (const QStringList& row : headerMap) {
        for (const QString& name : row[2].split("|")) {
            const QString value = string.parseString(" " + name + "_value :", "txt", file).split(" ")[0];
            scan->setData(row[0], row[1], value); } }

    // Read and set other data (both single and not single numbers)
    scan->setData("conditions",  "Date & Time",    string.parseString("### NICOS data file",      "date", file));
    scan->setData("conditions",  "Absolute index", string.parseString("number",                   "num",  file));
    scan->setData("orientation", "matrix",         string.parseString("Sample_rmat",              "num",  file));
    scan->setData("orientation", "matrix",         string.parseString("Sample_ubmatrix",          "num",  file));
    scan->setData("scandata",    "data",           string.parseString("### Scan data", "### End", "mult", file, 3));
    scan->setData("scandata",    "headers",        string.parseString("### Scan data",            "txt",  file, 1));

    // Read and set data from the scan table created above according to the header map
    extractDataFromTable(scan, headerMap);

    // Select appropriate data for the monitor
    As::RealVector monitor1 = scan->data("intensities", "Monitor1");
    As::RealVector monitor2 = scan->data("intensities", "Monitor2");
    As::RealVector monitor1up = scan->data("intensities", "Monitor1(+)");
    As::RealVector monitor1down = scan->data("intensities", "Monitor1(-)");
    As::RealVector monitor2up = scan->data("intensities", "Monitor2(+)");
    As::RealVector monitor2down = scan->data("intensities", "Monitor2(-)");

    //
    if (monitor1.isZero()) {
        scan->setData("intensities", "Monitor", monitor2.toQString()); }

    else {
        scan->setData("intensities", "Monitor", monitor1.toQString()); }

    //
    if (!monitor1up.isZero()) {
        scan->setData("intensities", "Monitor1(+)", monitor1up.toQString());
        scan->setData("intensities", "Monitor(+)", monitor1up.toQString()); }

    if (!monitor1down.isZero()) {
        scan->setData("intensities", "Monitor1(-)", monitor1down.toQString());
        scan->setData("intensities", "Monitor(-)", monitor1down.toQString()); }

    //
    if (!monitor2up.isZero()) {
        scan->setData("intensities", "Monitor2(+)", monitor2up.toQString()); }

    if (!monitor2down.isZero()) {
        scan->setData("intensities", "Monitor2(-)", monitor2down.toQString()); }

    // Define scan angle name
    scan->findAndSetScanAngle();

    // Append single scan to the scan array
    appendScan(scan); }

/*!
    Extracts the scans from the POLI Igor Pro log file using \a filesAsListOfStrings.
*/
void As::ScanArray::extractPoliLog(const int,
                                   const QString&,
                                   const QString&) {}

/*!
    Extracts the scans from the single 6T2 xml data file.
*/
// UB matrix is also present in the 6T2/5C2 file! Add reading routine!
void As::ScanArray::extract6t2Data(const int fileIndex,
                                   const QString& filePath,
                                   QString& fileContent) {
    ADEBUG;

    // Make a header map between the possible internal names and 6T2 xml parameter names
    QList<QStringList> headerMap;
    headerMap.append({"indices",     "H",               "h" });
    headerMap.append({"indices",     "K",               "k" });
    headerMap.append({"indices",     "L",               "l" });
    headerMap.append({"angles",      "Gamma",           "Gamma" });
    headerMap.append({"angles",      "2Theta",          "theta2" });
    headerMap.append({"angles",      "Nu",              "Nu" });
    headerMap.append({"angles",      "Omega",           "Omega" });
    headerMap.append({"angles",      "Phi",             "Phi" });
    headerMap.append({"angles",      "Chi",             "Chi" });
    headerMap.append({"conditions",  "Wavelength",      "wavelength" });
    headerMap.append({"conditions",  "Temperature",     "Temperature" });
    headerMap.append({"conditions",  "Magnetic field",  "magneticField" });
    headerMap.append({"conditions",  "Time/step",       "totaltimecount" });
    headerMap.append({"conditions",  "Time/step(+)",    "timeUp" });
    headerMap.append({"conditions",  "Time/step(-)",    "timeDown" });
    headerMap.append({"intensities", "Detector",        "counter" });
    headerMap.append({"intensities", "Detector(+)",     "counterUp" });
    headerMap.append({"intensities", "Detector(-)",     "counterDown" });
    headerMap.append({"intensities", "Monitor",         "totalmonitorcount" });
    headerMap.append({"intensities", "Monitor(+)",      "MonitorUpCount" });
    headerMap.append({"intensities", "Monitor(-)",      "MonitorDownCount" });

    // Variables
    auto scan = new As::Scan; // scan to be added to the scan array

    // Get the index of the file which contains the current scan
    scan->setFileIndex(fileIndex + 1);

    // Set the absolute file path of the file which contains the scan
    scan->setAbsoluteFilePath(filePath);

    // Feed file content to the xml reader
    QXmlStreamReader xmlReader(fileContent);

    // Check every tag
    while (!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartElement) {

            // Compare tag content with list of possible names
            for (const QStringList& row : headerMap) {
                if (xmlReader.name() == row[2]) {
                    xmlReader.readNext();

                    // Append the tag content to the respective scan component
                    bool ok;
                    qreal d = xmlReader.text().toDouble(&ok);

                    if (!ok) {
                        d = qQNaN(); }

                    const QString s = QString("%1").arg(d, 0, 'f', 3);
                    scan->appendData(row[0], row[1], s); } } } }

    // Re-init xml reader
    xmlReader.clear();
    xmlReader.addData(fileContent);

    // Create xml writer to format the input text
    QString formattedFile;
    QXmlStreamWriter xmlWriter(&formattedFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.setAutoFormattingIndent(1);

    // Write every tag to xml writer
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();

        if (!xmlReader.isWhitespace()) {
            xmlWriter.writeCurrentToken(xmlReader); } }

    fileContent = formattedFile;

    // Define scan angle name
    //scan->setScanAngle("Omega");
    scan->findAndSetScanAngle();

    // Append single scan to the scan array
    appendScan(scan); }

/*!
    Extracts the data from the generic table created in the previous
    Nicos extraction step for the given \a scan and according to the
    provided list of headers \a headerMap.
*/
void As::ScanArray::extractDataFromTable(As::Scan* scan,
                                         QList<QStringList>& headerMap) {

    // Make 2D map of the actually measured data from the single data string
    const QStringList dataList = scan->data("scandata", "data").split("\n");
    QList<QStringList> dataMap;

    for (const QString& string : dataList) {
        dataMap << string.split(QRegExp("\\s"), QString::SkipEmptyParts); }


    // Exit from function if no data were found
    if (dataMap.size() == 1 AND dataMap[0].size() == 1) {
        return; }

    // Check if data measured according to the header, and fill the respective array
    const QStringList headerList = scan->data("scandata", "headers").split(QRegExp("\\s"), QString::SkipEmptyParts);

    for (int i = 0; i < headerMap.size(); ++i) {
        for (const QString& name : headerMap[i][2].split("|")) {
            const QRegularExpression re(name + "(_.*){0,1}"); // to catch, e.g., both 'sth' and 'sth_jvm2' cases
            const int row = headerList.indexOf(re);

            if (row != -1) {
                QStringList data;

                for (const QStringList& dataList : dataMap) {
                    if (dataList.size() > row) {
                        data << dataList[row]; } }

                scan->setData(headerMap[i][0], headerMap[i][1], data.join(" ")); } } } }

/*!
    Appends the \a scan to the scan array.
*/
void As::ScanArray::appendScan(As::Scan* scan) {
    ADEBUG << scan;

    if (scan->numPoints() < As::ScanDict::MIN_DATA_POINTS) {
        delete scan;
        return; }

    QStringList itemKeys = {"angles", "indices" };

    for (const auto& itemKey : itemKeys) {
        QStringList subitemKeys = (*scan)[itemKey].keys();

        for (const auto& subitemKey : subitemKeys) {
            // Check if there is any not-empty angle or hkl and...
            if (!scan->data(itemKey, subitemKey).isEmpty() AND !scan->scanAngle().isEmpty()) {
                append(scan);
                return; } } }

    // not correctly measured scan - deallocate memory
    delete scan; }
