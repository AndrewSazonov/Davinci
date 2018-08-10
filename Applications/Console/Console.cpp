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

#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
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
Constructs a console version of the application.
*/
As::Console::Console()
{
    // Create command line parser
    createCommandLineParser(qApp);

    // Print program description
    printAppDescription();

    // Set output format for qDebug() depends on the build type
    SetDebugOutputFormat(IS_DEBUG_OR_PROFILE);

    // Check if required options are provided by user
    checkRequiredOptions(QStringList{"path"});

    // Open file or dir based on the path provided in 'path' option
    openFiles(m_parser.value("path"));

    // Process the measured data using Multi-Thread
    concurrentRun("extract", m_scans);
    printMessage(QString("Number of treated files:  %1").arg(m_scans->m_inputFilesContents.first.size()));

    concurrentRun("fill", m_scans);
    concurrentRun("index", m_scans);
    concurrentRun("pretreat", m_scans); // included in "index" above!
    concurrentRun("treat", m_scans);
    printMessage(QString("Number of treated reflections:  %1").arg(m_scans->size()));

    // Output
    exportOutputTable();
    printProgramOutput();
}

/*!
Destroys the application.
*/
As::Console::~Console()
{
    ADESTROYED;
}

/*!
Returns the application description.
*/
QString As::Console::applicationDescription() const
{
    ADEBUG;

    return m_parser.applicationDescription();
}

/*!
Returns the format of the output file.
*/
// file to be exported?!
QString As::Console::outputFileFormat() const
{
    ADEBUG;

    return m_parser.value("format").isEmpty() ? "General" : m_parser.value("format");
}

/*!
Returns the extension of the output file.
*/
QString As::Console::outputFileExt() const
{
    ADEBUG;

    // sync with GUI types?!

    const QString format = outputFileFormat().toLower();
    QString ext;

    if (format.contains("shelx"))
        ext = "hkl";

    else if (format.contains("tbar"))
        ext = "tb";

    else if (format.contains("umweg"))
        ext = "obs";

    else if (format.contains("ccsl"))
        ext = "fli";

    else // default
        ext = "csv";

    return ext;
}

/*!
Returns the name of the output file.
*/
QString As::Console::outputFileName() const
{
    ADEBUG;

    const auto firstScan = m_scans->at(0);
    const auto lastScan = m_scans->at(m_scans->size()-1);

    const QString baseNameFirst = firstScan->baseName();
    const QString baseNameLast = lastScan->baseName();
    const QString absolutePathLast = lastScan->absolutePath();
    const QString pathWithName = FormatToPathWithName(baseNameFirst, baseNameLast, absolutePathLast);

    return m_parser.value("output").isEmpty() ? pathWithName : m_parser.value("output");
}

/*!
Returns the name with extension of the output file.
*/
QString As::Console::outputFileNameWithExt() const
{
    ADEBUG;

    return outputFileName() + "." + outputFileExt();
}

/*!
Process the actual command line arguments given by the user
for a given core application \a app.
*/
void As::Console::createCommandLineParser(QCoreApplication *app)
{
    ADEBUG;

    // Application description
    const QString text = QString("%1\n"
                                 "v%2 (%3)\n"
                                 "%4\n"
                                 "%5\n"
                                 "%6")
            .arg(APP_NAME)
            .arg(APP_VERSION).arg(APP_RELEASE_DATE)
            .arg(APP_URL)
            .arg(QString(APP_DESCRIPTION).replace("<br />", " "))
            .arg(APP_COPYRIGHT).replace("&copy;", "(C)");
    m_parser.setApplicationDescription(qPrintable(text));

    // Pre-defined options
    //m_parser.addVersionOption();
    m_parser.addHelpOption();

    // Add options
    m_parser.addOptions({{{"p", "path"},   "File/dir to open.", "file/dir"},
                         {{"o", "output"}, "File to save output data.", "file"},
                         {{"f", "format"}, "Output file format <type>: general, shelx, tbar, umweg, ccsl.", "type"},
                        }); // {{"d", "debug"},  "Enable debug output."}

    // Add arguments
    //parser->addPositionalArgument("urls", QObject::tr("Files to open."), "[urls...]");
    //parser->addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));
    //m_parser.addPositionalArgument("file", QObject::tr("The file(s) to open."));
    //addPositionalArgument("path", QCoreApplication::translate("main", "The path to file or directory to open.")); // what is "main"?

    // Get positional arguments
    //const QStringList args = parser.positionalArguments();
    //const QString command = args.isEmpty() ? QString() : args.first();
    //qDebug() << "is debug:" << parser.isSet("debug");
    //qDebug() << parser.value("path");

     // Link parser to application
    m_parser.process(*app);

    // Debug output format, depends on the option provided by user
    //As::SetDebugOutputFormat(m_parser.isSet("debug"));
}

/*!
Opens the file(s) according to the given file or folder \a path.
*/
void As::Console::openFiles(const QString &path)
{
    ADEBUG << path;

    QStringList filePathList;

    QFileInfo fileInfo(path);

    if (fileInfo.isFile()) {
        filePathList << path; }

    else if (fileInfo.isDir()) {
        QDir dir(path);
        for (const QFileInfo &fileInfo : dir.entryInfoList(QDir::Files)) {
            filePathList << fileInfo.absoluteFilePath(); } }

    if (filePathList.isEmpty()) {
        printMessage(QString("Cannot find file/dir '%1'.")
                     .arg(QDir::toNativeSeparators(path)));
        AEXIT; }

    loadFiles(filePathList);
}

/*!
Loads the file(s) according to the given list of file path \a filePathList.
*/
void As::Console::loadFiles(const QStringList &filePathList)
{
    ADEBUG << filePathList;

    // Create the future watcher
    //m_futureWatcher = new QFutureWatcher<void>;

    // Create the scan array
    m_scans = new As::ScanArray;

    // Read all files contents
    for (const auto &path : filePathList) {

        // Open file
        QFile file(path);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            printMessage(QString("Cannot read file '%1': %2.")
                         .arg(QDir::toNativeSeparators(path), file.errorString()));
            AEXIT; }

        // Read file content to QStringList using QTextStream
        QTextStream textStream(&file);
        textStream.setAutoDetectUnicode(true);
        textStream.setCodec("MacRoman"); // "ISO 8859-1", "UTF-8", "UTF-16", "MacRoman" (POLI)?!

        // Add file path and content to the global variable
        m_scans->m_inputFilesContents.first  << path;
        m_scans->m_inputFilesContents.second << textStream.readAll(); }

    // Detect data
    bool ok = m_scans->detectInputFileType();
    if (!ok) {
        printMessage(QString("Files of multiple types were selected for opening. "
                             "Please open the files of the same type only."));
        AEXIT; }
}

/*!
Exports the output table to disk.
*/
void As::Console::exportOutputTable() const
{
    m_scans->createFullOutputTable();

    m_scans->saveSelectedOutputColumns(outputFileNameWithExt(), outputFileFormat());
}

/*!
Checks if all the required options \a optionList are provided by the user.
*/
void As::Console::checkRequiredOptions(const QStringList &optionList) const
{
    ADEBUG << optionList;

    const QString message = "Required option '--%s' is not specified.\n"
                            "Run the program with '--help' or '-h' to see more.\n";
    for (const QString option : optionList) {
        if (m_parser.value(option).isEmpty()) {
            printMessage(message, option);
            AEXIT; } }
}

/*!
Prints the console \a message. If optional argument \a arg is provided, then
\a arg is placed inside the \a message.
*/
void As::Console::printMessage(const QString &message,
                               const QString &arg) const
{
    ADEBUG;

    if (arg.isEmpty())
        fprintf(stderr, "%s\n", qUtf8Printable(message));
    else
        fprintf(stderr, qUtf8Printable(message), qUtf8Printable(arg));
}

/*!
Prints the list of messages \a messageList.
*/
void As::Console::printMessageList(const QStringList &messageList) const
{
    ADEBUG;

    for (const QString &message : messageList)
        printMessage(message);
}

/*!
Prints the application description.
*/
void As::Console::printAppDescription() const
{
    ADEBUG;

    printMessageList(QStringList{applicationDescription(), ""});
}

/*!
Prints the console output when the programm is finished.
*/
void As::Console::printProgramOutput() const
{
    ADEBUG;

    const QStringList messageList = {
        //QString("Number of treated files:  %1").arg(m_scans->m_inputFilesContents.first.size()),
        //QString("Number of treated reflections:  %1").arg(m_scans->size()),
        QString("Output file:  %1").arg(outputFileNameWithExt()),
        "",
        "The program is finished successfully." };

    printMessageList(messageList);
}

void As::Console::concurrentRun(const QString &type,
                               As::ScanArray *scans) const
{
    As::ConcurrentWatcher watcher;
    watcher.startComputation(type, scans);
}
