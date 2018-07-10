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

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontComboBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QToolBar>
#include <QTimer>
#include <QVariant>
#include <QUrl>

#include "Colors.hpp"
#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "FontComboBox.hpp"
#include "LineEdit.hpp"
#include "MessageWidget.hpp"
#include "Style.hpp"
#include "SyntaxHighlighter.hpp"
#include "TableView.hpp"
#include "TextEditor.hpp"
#include "ToolBarButton.hpp"
#include "ToolBarSpacer.hpp"
#include "UnderLabeledWidget.hpp"

#include "Scan.hpp"
#include "ScanArray.hpp"
#include "ScanDatabase.hpp"

#include "Window.hpp"


#include "RealMatrix9.hpp"
#include "RealVector.hpp"
#include <qmath.h>

/*!
Constructs the main program window.
*/
As::Window::Window()
{
    // Start program timer
    As::g_elapsedTimer.start();

    // Print application info
    printAppInfo_Slot();

    // Set output format for qDebug(), etc. if debug-build mode
    //qInstallMessageHandler(0);
    //qInstallMessageHandler(As::DetailedMessageOutput);
    //SetDebugOutputFormat(1);
    SetDebugOutputFormat(IS_DEBUG);

    // Set program GUI style
    setStyleSheet(createStyleSheet());

    // Method setFontFilters(QFontComboBox::MonospacedFonts) takes some time.
    // Variable monospacedFonts is requred to shift that time delay from the
    // files opening time to the program opening time.
    monospacedFonts = new As::FontComboBox;
    monospacedFonts->setFontFilters(QFontComboBox::MonospacedFonts);

    // Create action, menues and tool bar
    createActionsMenusToolBar();
    //createStatusBar();

    // To be able to receive media dropped on a widget
    setAcceptDrops(true);

    // Initial central widget before new files are loaded
    setCentralWidget(createDragAndDropWidget());
    //setCentralWidget(createMainWidget());

    // Window size and position
    setupWindowSizeAndPosition();

    // Update windows title when needed
    connect(this, &As::Window::currentFilePathChanged_Signal, this, &As::Window::setWindowTitle);

    // Update count of application start
    setApplicationStartCount();

    // Show the main application window
    show();

    // Offer to chose the automatic update
    offerAutoUpdate();

    // Check for update, if required
    checkApplicationUpdate();
}

/*!
Destructs the main window.
*/
As::Window::~Window()
{
    ADEBUG;

    writeSettings();
}

/*!
Writes application setting to disk.
*/
void As::Window::writeSettings()
{
    ADEBUG;

    QSettings().setValue("MainWindow/size", size());
    QSettings().setValue("MainWindow/position", pos());
    //if (m_sidebarWidget)
    //    settings.setValue("SideBar/isVisible", m_sidebarWidget->isVisible());
}

/*!
Loads file(s).
*/
void As::Window::loadFiles(const QStringList &filePathList)
{
    ADEBUG;

    // Get file path of the last opened file
    QFileInfo fileLastOpen(filePathList[filePathList.size() - 1]);

    // Save settings
    QSettings().setValue("MainWindow/filePathLastOpen", fileLastOpen.absolutePath());

    // Create or re-create the scan array
    if (m_scans != Q_NULLPTR) {
        delete m_scans;
        m_scans = Q_NULLPTR; }
    m_scans = new As::ScanArray;

    // Signal-slot connections for the scans array
    //connect(this, SIGNAL(currentFileIndexChanged_Signal(int)), m_scans, SLOT(setModel(int)));
    connect(this, &As::Window::currentFileIndexChanged_Signal, m_scans, &As::ScanArray::setFileIndex);
    connect(this, &As::Window::currentScanChanged_Signal, m_scans, &As::ScanArray::setScanIndex);
    connect(m_scans, &As::ScanArray::scanIndexChanged, this, &As::Window::highlightScanLines_Slot);
    connect(m_scans, &As::ScanArray::scanIndexChanged, this, &As::Window::highlightFoundText_Slot);
    connect(m_scans, &As::ScanArray::facilityTypeChanged_Signal, this, &As::Window::facilityTypeChanged_Signal);
    connect(m_scans, &As::ScanArray::instrumentTypeChanged_Signal, this, &As::Window::instrumentTypeChanged_Signal);
    connect(m_scans, &As::ScanArray::dataTypeChanged_Signal, this, &As::Window::dataTypeChanged_Signal);

    // Create or re-create the common scan
    if (m_commonScan != Q_NULLPTR) {
        delete m_commonScan;
        m_commonScan = Q_NULLPTR; }
    m_commonScan = new As::Scan;

    // Save contents of the old files
    auto oldInputFilesContents = m_scans->m_inputFilesContents;

    // Clear global vars
    m_scans->m_inputFilesContents.first.clear();
    m_scans->m_inputFilesContents.second.clear();
    //m_scans->clear();

    // Create or re-create main widget
    setCentralWidget(createMainWidget()); // dragAndDropWidget is then deleted automatically

    // Read all files contents
    for (const auto &path : filePathList) {
        // Open file
        QFile file(path);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this,
                                 tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(path), file.errorString()));
            return; }
        // Read file content to QStringList using QTextStream
        QTextStream textStream(&file);
        textStream.setAutoDetectUnicode(true);
        //textStream.setCodec("MacRoman"); // "ISO 8859-1", "UTF-8", "UTF-16", "MacRoman" (POLI)
        // Add file path and content to the global variable
        m_scans->m_inputFilesContents.first  << path;
        m_scans->m_inputFilesContents.second << textStream.readAll(); }

    // To disable actions and buttons. False - to use both with setEnabled and setChecked
    emit oldFilesClosed_Signal(false);

    // Detect data
    bool ok = m_scans->detectInputFileType();
    if (!ok) {
        auto msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText("Files of multiple types were selected for opening."
                        "Please open the files of the same type only.");
        msgBox->exec();
        m_scans->m_inputFilesContents = oldInputFilesContents;
        openFile_Slot(); }

    // Emit signal(s)
    const int size = m_scans->m_inputFilesContents.second.size();
    if (size > 0) {
        emit newFilesLoaded_Signal(1);
        emit filesRangeChanged_Signal(1, size);
        emit filesCountChanged_Signal(QString::number(size)); }

    // Highlight syntax
    // Use enum InputFileType here and in SyntaxHighlighter!?
    const QString type = m_scans->m_instrumentType + " " + m_scans->m_dataType;
    new As::SyntaxHighlighter(m_inputTextWidget->document(), type);
}

/*!
Drag event. This event is called when the mouse enters the widgets area
during a drag/drop operation.
*/
void As::Window::dragEnterEvent(QDragEnterEvent *event)
{
    ADEBUG;

    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

/*!
Drop event. This event is called when the drop operation is initiated at the widget.
*/
void As::Window::dropEvent(QDropEvent *event)
{
    ADEBUG;

    if (event->mimeData()->hasUrls()) {

        // Extract the local paths of the files
        QStringList pathList;
        for (const QUrl &url : event->mimeData()->urls())
            pathList << url.toLocalFile();

        // Open file(s)
        openFiles(pathList); }
}

/*!
Opens file(s).
*/
void As::Window::openFiles(const QStringList &pathList)
{
    ADEBUG;

    if (pathList.isEmpty())
        return;

    m_pathList = pathList;

    QStringList filePathList;

    for (const QString &path : pathList) {
        QFileInfo fileInfo(path);

        if (fileInfo.isFile()) {
            filePathList << path; }

        else if (fileInfo.isDir()) {
            QDir dir(path);
            for (const QFileInfo &fileInfo : dir.entryInfoList(QDir::Files)) {
                filePathList << fileInfo.absoluteFilePath(); } } }

    // Load file(s)
    if (!filePathList.isEmpty())
        loadFiles(filePathList);
}

/*!
Prints the application information.
*/
void As::Window::printAppInfo_Slot()
{
    ADEBUG << "";
    ADEBUG << " - Application:  " << APP_NAME;
    ADEBUG << " - Version:      " << APP_VERSION;
    ADEBUG << " - Release date: " << APP_RELEASE_DATE;
    ADEBUG << " - Product url:  " << APP_URL;
    ADEBUG << "";
}

/*!
Updates the number of the application start.
*/
void As::Window::setApplicationStartCount()
{
    ADEBUG;

    const QString key = "Preferences/ApplicationStartCount";
    const int defaultValue = 0;

    const int count = QSettings().value(key, defaultValue).toInt();
    QSettings().setValue(key, count + 1);
}

/*!
Offers to chose an automatic update, when program starts for the first time.
*/
void As::Window::offerAutoUpdate()
{
    ADEBUG;

    const int count = QSettings().value("Preferences/ApplicationStartCount", 0).toInt();

    // Return if the program run not for the 1st time
    if (count != 0)
        return;

    // Opens user dialog window

    const QString title = QMessageBox::tr("Update");

    const QString text = QMessageBox::tr(
                "<p align='center'><big><b>Check for updates automatically?</b></big></p>"
                "<p align='center'>Should %1 automatically check for updates?<br />"
                "You can always change this setting or check for<br />"
                "updates manually from the program menu.")
            .arg(APP_NAME);

    const QString okButton = QMessageBox::tr("Check Automatically");
    const QString cancelButton = QMessageBox::tr("Don't Check");

    As::MessageWidget dialog(this, title, text, okButton, cancelButton);
    connect(&dialog, &QDialog::accepted, this, &As::Window::acceptAutoUpdate_Slot);
    connect(&dialog, &QDialog::rejected, this, &As::Window::rejectAutoUpdate_Slot);

    dialog.exec();

    // Tried to get rid of the maximize window button in the dialog box on macOS, but failed

    // 1st way
    //Qt::WindowFlags flags = 0;
    //flags |= Qt::WindowTitleHint;

    // 2nd way
    //Qt::WindowFlags flags = dialog.windowFlags();
    //dialog.setWindowFlags(flags & ~Qt::WindowMaximizeButtonHint);

    // 3rd way: Window or Dialog + required parts (Title, Close button, etc.)
    // The CustomizeWindowHint flag is used to enable customization of the window controls.
    //dialog.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
    //dialog.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
}

/*!
Checks if application updates are available.
*/
void As::Window::checkApplicationUpdate()
{
    ADEBUG;

    const bool autoUpdate = QSettings().value("Preferences/autoUpdate", true).toBool();

    if (autoUpdate) {
        const bool hideOutput = true;
        checkApplicationUpdateNow_Slot(hideOutput); }
}

/*!
Resizes main program window and move it to the screen center.
*/
void As::Window::setupWindowSizeAndPosition()
{
    ADEBUG;

    // Read saved window size
    QSize windowRect = QSettings().value("MainWindow/size", QSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT)).toSize();

    // Relative widget to have screen dimensions
    QDesktopWidget *desktop = QApplication::desktop();
    QRect desktopRect = desktop->screenGeometry(this); // main screen
    //QRect desktopRect = desktop->screenGeometry(desktop->screenNumber(QCursor::pos())); // screen with cursor

    // Correct window size to be not larger than screen size
    windowRect.setWidth(qMin(windowRect.width(), desktopRect.width()));
    windowRect.setHeight(qMin(windowRect.height(), desktopRect.height()));

    // Calculate window position to be in the screen center
    const int left = (desktopRect.width() - windowRect.width()) / 2;
    const int top = (desktopRect.height() - windowRect.height()) / 2;

    // Resize main window
    resize(windowRect.width(), windowRect.height());

    // Read saved window position
    QPoint windowPos = QSettings().value("MainWindow/position", QPoint(left, top)).toPoint();

    // Move main window
    move(windowPos.x(), windowPos.y());
}

/*!
Creates status bar.
*/
void As::Window::createStatusBar()
{
    ADEBUG;

    statusBar()->showMessage(tr("Ready"));
}

/*!
Returns the style for the main window as QString.
*/
QString As::Window::createStyleSheet() const
{
    ADEBUG;

    return As::Style().toQString();
}

/*!
Returns the scan at the position \a index.
*/
const As::Scan *As::Window::scanAt(const int index) const
{
    return m_scans->at(index - 1);
}

/*!
Returns a pointer to the current scan.
*/
As::Scan *As::Window::currentScan() const
{
    if (m_scans->scanIndex() == 0)
        return Q_NULLPTR;

    return m_scans->at(m_scans->scanIndex() - 1);
}

/*!
Returns a pointer to the generic scan which have the common settings.
*/
As::Scan *As::Window::genericScan() const
{
    return m_commonScan;
}

/*!
Returns an index of the current scan
*/
// move it to ScanArray?!
int As::Window::currentScanIndex() const
{
    if (m_scans)
        return m_scans->scanIndex();

    return -1;
}

/*!
Returns a file path to the external application maintenance tool.
*/
QString As::Window::maintainerPath()
{
    ADEBUG;

    QString appPath = QApplication::applicationFilePath();
    const QRegularExpression appName(QString("%1(?![/\\\\])").arg(APP_NAME)); // match 'APP_NAME' which is not followed by '/' or '\'
    return appPath.replace(appName, MAINTAINER_NAME);
}


// you don't need to use Q_DECLARE_METATYPE for classes derived from QObject.
// http://lists.qt-project.org/pipermail/interest/2016-March/021132.html
// http://lists.qt-project.org/pipermail/interest/2016-March/021137.html

/*
https://stackoverflow.com/questions/4954140/how-to-redirect-qdebug-qwarning-qcritical-etc-output
http://www.qtcentre.org/threads/19534-redirect-qDebug()-to-file
https://forum.qt.io/topic/50936/qt-5-4-sending-qdebug-to-a-file-40-aka-logging-41-solved/19
https://gist.github.com/ajithbh/9383850
http://clauandr.sdf-eu.org/blog/2013/03/19/how-to-redirect-qdebug-qwarning-qcritical-output/
https://www.ics.com/blog/whats-new-qt-560-logging-syslog-and-journald
*/


