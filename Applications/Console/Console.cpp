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

#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMetaObject>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include <QtConcurrent>

#include "Functions.hpp"
#include "Macros.hpp"

#include "ConcurrentWatcher.hpp"
#include "Scan.hpp"
#include "ScanArray.hpp"

#include "Console.hpp"

/*!
    Constructs a console version of the application with command line parser.
*/
As::Console::Console(QObject* parent)
    : QObject(parent) {
    As::SetDebugOutputFormat(IS_DEBUG_OR_PROFILE);
    createCommandLineParser(qApp); }

/*!
    Destroys the console.
*/
As::Console::~Console() {
    ADESTROYED; }

/*!
    Starts the data processing.
*/
void As::Console::run() {
    printAppDescription();

    if (!checkRequiredOptionsAreProvided(QStringList{ "path" }))
        return;
    if (!setOutputFileExt()) {
        return; }
    if (!openFiles()) {
        return; }
    if (!detectInputFilesType()) {
        return; }

    // Process the data using Multi-Thread (concurrentRun)
    concurrentRun("extract", &m_scans);
    printMessage(QString("Number of treated files:  %1").arg(m_scans.m_inputFilesContents.first.size()));
    concurrentRun("fill", &m_scans);
    concurrentRun("index", &m_scans);
    concurrentRun("treat", &m_scans);
    printMessage(QString("Number of treated reflections:  %1").arg(m_scans.size()));

    exportOutputTable();
    printProgramOutput(); }

/*!
    Returns the application description.
*/
QString As::Console::applicationDescription() const {
    return m_parser.applicationDescription(); }

/*!
    Returns the format of the output file. Default value: "General".
*/
QString As::Console::outputFileFormat() const {
    return m_parser.value("format").isEmpty() ? "General" : m_parser.value("format"); }

/*!
    Sets the extension of the output file. Default value: "csv"
*/
// sync with GUI types?!
bool As::Console::setOutputFileExt() {

    const QString format = outputFileFormat().toLower();

    if (format.isEmpty()) {
        m_outputFileExt = "csv"; }

    else if (format.contains("general")) {
        m_outputFileExt = "csv"; }

    else if (format.contains("shelx")) {
        m_outputFileExt = "hkl"; }

    else if (format.contains("tbar")) {
        m_outputFileExt = "tb"; }

    else if (format.contains("umweg")) {
        m_outputFileExt = "obs"; }

    else if (format.contains("ccsl")) {
        m_outputFileExt = "fli"; }

    else {
        printMessage(QString("Unknown output file format '%1'").arg(format));
        printMessage("Run the program with '--help' or '-h' to see more.");
        return false; }

    return true; }

/*!
    Returns the extension of the output file.
*/
QString As::Console::outputFileExt() const {
    return m_outputFileExt; }

/*!
    Returns the name of the output file.
*/
QString As::Console::outputFileName() const {
    const auto firstScan = m_scans.at(0);
    const auto lastScan = m_scans.at( m_scans.size() - 1 );

    const QString baseNameFirst = firstScan->baseName();
    const QString baseNameLast = lastScan->baseName();
    const QString absolutePathLast = lastScan->absolutePath();
    const QString pathWithName = FormatToPathWithName(baseNameFirst, baseNameLast, absolutePathLast);

    return m_parser.value("output").isEmpty() ? pathWithName : m_parser.value("output"); }

/*!
    Returns the name with extension of the output file.
*/
QString As::Console::outputFileNameWithExt() const {
    return outputFileName() + "." + outputFileExt(); }

/*!
    Process the actual command line arguments given by the user
    for a given core application \a app.
*/
void As::Console::createCommandLineParser(QCoreApplication* app) {
    const QString text = QString("%1 v%2 (%3)\n"
                                 "%4\n"
                                 "%5\n"
                                 "%6")
                         .arg(APP_NAME).arg(APP_VERSION).arg(APP_RELEASE_DATE)
                         .arg(APP_URL)
                         .arg(APP_DESCRIPTION)
                         .arg(APP_COPYRIGHT);
    m_parser.setApplicationDescription(qPrintable(text));

    m_parser.addHelpOption();
    m_parser.addOptions({{{"p", "path" },   "File/dir to open.", "file/dir" },
        {{"o", "output" }, "File to save output data.", "file" },
        {{"f", "format" }, "Output file format <type>: general, shelx, tbar, umweg, ccsl.", "type" }, });

    // Link parser to application
    m_parser.process(*app); }

/*!
    Opens the file(s) according to the given file or folder \a path.
*/
bool As::Console::openFiles() {

    const QString& path = m_parser.value("path");
    const QFileInfo fileInfo(path);
    QStringList filePathList;

    if (fileInfo.isFile()) {
        filePathList << path; }

    else if (fileInfo.isDir()) {
        QDir dir(path);
        for (const QFileInfo& fileInfo : dir.entryInfoList(QDir::Files)) {
            filePathList << fileInfo.absoluteFilePath(); } }

    if (filePathList.isEmpty()) {
        printMessage(QString("Cannot find file/dir '%1'.")
                     .arg(QDir::toNativeSeparators(path)));
        return false; }

    if (!loadData(filePathList)) {
        return false; }

    return true; }

/*!
    Loads the file(s) according to the given list of file path \a filePathList.
*/
bool As::Console::loadData(const QStringList& filePathList) {
    for (const auto& path : filePathList) {
        QFile file(path);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            printMessage(QString("Cannot read file '%1': %2.")
                         .arg(QDir::toNativeSeparators(path), file.errorString()));
            return false; }

        // Read file content to QStringList using QTextStream
        QTextStream textStream(&file);
        textStream.setAutoDetectUnicode(true);
        textStream.setCodec("MacRoman"); // "ISO 8859-1", "UTF-8", "UTF-16", "MacRoman" (POLI)?!

        // Add file path and content to the member variable
        m_scans.m_inputFilesContents.first  << path;
        m_scans.m_inputFilesContents.second << textStream.readAll(); }

    return true; }

/*!
    Detects a type of the input files.
*/
bool As::Console::detectInputFilesType() {
    if (!m_scans.detectInputFilesType()) {
        printMessage("Files of multiple types were selected for opening. "
                     "Please open the files of the same type only.");
        return false; }
    return true; }

/*!
    Exports the output table to disk.
*/
void As::Console::exportOutputTable() {
    m_scans.createFullOutputTable();
    m_scans.saveSelectedOutputColumns(outputFileNameWithExt(), outputFileFormat()); }

/*!
    Checks if all the required options \a optionList are provided by the user.
*/
bool As::Console::checkRequiredOptionsAreProvided(const QStringList& optionList) const {
    for (const QString& option : optionList) {
        if (m_parser.value(option).isEmpty()) {
            printMessage("Required option '--%s' is not specified.\n", option);
            printMessage("Run the program with '--help' or '-h' to see more.");
            return false; } }
    return true; }

/*!
    Prints the console \a message. If optional argument \a arg is provided, then
    \a arg is placed inside the \a message.
*/
void As::Console::printMessage(const QString& message,
                               const QString& arg) const {
    if (arg.isEmpty()) {
        fprintf(stderr, "%s\n", qUtf8Printable(message)); }
    else {
        fprintf(stderr, qUtf8Printable(message), qUtf8Printable(arg)); } }

/*!
    Prints the list of messages \a messageList.
*/
void As::Console::printMessageList(const QStringList& messageList) const {
    for (const QString& message : messageList) {
        printMessage(message); } }

/*!
    Prints the application description.
*/
void As::Console::printAppDescription() const {
    printMessageList(QStringList{ applicationDescription(), "" }); }

/*!
    Prints the console output when the programm is finished.
*/
void As::Console::printProgramOutput() const {
    const QStringList messageList = {
        QString("Output file:  %1").arg(outputFileNameWithExt()),
        "",
        "The program is finished successfully." };

    printMessageList(messageList); }

/*!
    Starts parallel computation of type \a type on the scan array \a scans.
*/
void As::Console::concurrentRun(const QString& type,
                                As::ScanArray* scans) const {
    As::ConcurrentWatcher watcher;
    watcher.startComputation(type, scans); }
