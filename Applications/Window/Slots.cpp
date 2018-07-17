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

#include <QAbstractItemModel>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QModelIndex>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTextEdit>
#include <QVariant>
#include <QXmlStreamWriter>

#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "LineEdit.hpp"
#include "MessageWidget.hpp"
#include "TextEditor.hpp"
#include "SaveHeaders.hpp"
#include "SpinBox.hpp"
#include "SyntaxHighlighter.hpp"
#include "Plot.hpp"
#include "PreferencesDialog.hpp"
#include "TableView.hpp"
#include "VBoxLayout.hpp"

#include "Window.hpp"

/*!
Open file(s) dialog.
*/
void As::Window::openFile_Slot()
{
    ADEBUG;

    QString filePathLastOpen = QSettings().value("MainWindow/filePathLastOpen", "").toString();

    QStringList filePathList = QFileDialog::getOpenFileNames(
                this,
                tr("Open data file(s)"),
                filePathLastOpen,
                "");

    openFiles(filePathList);
}

/*!
Open directory dialog.
*/
void As::Window::openDir_Slot()
{
    ADEBUG;

    QString filePathLastOpen = QSettings().value("MainWindow/filePathLastOpen", "").toString();

    QString dirPath = QFileDialog::getExistingDirectory(
                this,
                tr("Open directory"),
                filePathLastOpen,
                QFileDialog::ShowDirsOnly);

    openFiles(QStringList{dirPath}); // converted to QStringList with the single QString element
}

/*!
Closes the current files.
*/
void As::Window::closeFile_Slot()
{
    ADEBUG;

    setCentralWidget(createDragAndDropWidget()); // mainWidget is then deleted automatically

    // To disable actions and buttons. False - to use both with setEnabled and setChecked
    emit oldFilesClosed_Signal(false);
}

/*!
Reloads the current files.
*/
void As::Window::reloadFile_Slot()
{
    ADEBUG;

    closeFile_Slot();

    openFiles(m_pathList);
}

/*!
Export slot, depends on the tab selected in the main window.
*/
void As::Window::export_Slot()
{
    ADEBUG;

    auto currentTab = m_tabsWidget->currentWidget();

    //if (currentTab == m_inputTextWidget)
    //    ;

    if (currentTab == m_visualizedPlotsWidget)
        exportImage_Slot();

    if (currentTab == m_outputTableWidget)
        exportOutputTable_Slot();
}

/*!
Exports the plot as an image.
*/
void As::Window::exportImage_Slot()
{
    ADEBUG;

    //const QString path = m_scans[m_scans->scanIndex()-1].absolutePathWithBaseNameAndHkl();
    const QString path = m_scans->at(m_scans->scanIndex()-1)->absolutePathWithBaseNameAndHkl();

    QString filename = QFileDialog::getSaveFileName(
                this,
                tr("Save Image"),
                path + ".pdf",
                tr("PDF vector graphics (*.pdf);;"
                   "JPEG raster graphics (*.jpg)") );

    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    if (file.fileName().endsWith("jpg"))
        m_visualizedPlotsWidget->saveJpg(filename, 0, 0, 1.0, 100);

    else if (file.fileName().endsWith("pdf"))
        m_visualizedPlotsWidget->savePdf(filename, true, 0, 0, "", "");

    file.close();
}

/*!
Exports the output table.
*/
void As::Window::exportOutputTable_Slot()
{
    ADEBUG;

    // Update output table
    createFullOutputTableModel_Slot();

    // Define the path of the file to be exported.
    // Repetition of console.cpp part!!!!!!!!! fix
    const auto firstScan = m_scans->at(0);
    const auto lastScan = m_scans->at(m_scans->size()-1);

    const QString baseNameFirst = firstScan->baseName();
    const QString baseNameLast = lastScan->baseName();
    const QString absolutePathLast = lastScan->absolutePath();
    const QString pathWithName = FormatToPathWithName(baseNameFirst, baseNameLast, absolutePathLast);

    // Filter to distinguish between the output types
    QString format;

    // Save file dialog
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save Output"),
                pathWithName + ".csv", // default extension
                tr("General comma-separated, real (*.csv);;"
                   "ShelX with direction cosines, integer (*.hkl);;"
                   "ShelX with direction cosines, real (*.hkl);;"
                   "TBAR/D9, integer (*.tb);;"
                   "UMWEG, integer (*.obs);;"
                   "CCSL flipping ratios, integer (*.fli)"),
                &format); // can be a problem on linux: http://www.qtcentre.org/threads/21019-Determining-selected-filter-on-getSaveFileName

    // Save selected columns
    m_scans->saveSelectedOutputColumns(fileName, format);
}

/*!
Shows the application about info.
*/
void As::Window::aboutApp_Slot()
{
    ADEBUG;

    const QString title = QMessageBox::tr("About");

    const QString text = QMessageBox::tr(
                "<p align='center'><big><b>%1</b></big>"
                "<p align='center'>Version %2 (%3)<br />"
                "<a href=\"%4\" style=\"color: %5\">%4</a></p>"
                "<p align='center'>%6"
                "<p align='center'><small>%7</small></p>")
            .arg(APP_NAME)
            .arg(APP_VERSION).arg(APP_RELEASE_DATE)
            .arg(APP_URL).arg(As::Color(As::blueDarkVery).name())
            .arg(QString(APP_DESCRIPTION).replace("of ", "of<br />"))
            .arg(APP_COPYRIGHT);

    auto msgBox = new As::MessageWidget(this, title, text);
    msgBox->exec();
}

/*!
Shows the application preferences window.
*/
void As::Window::showPreferences_Slot()
{
    ADEBUG << this;

    PreferencesDialog *dialog = new PreferencesDialog(this);

    connect(dialog, &As::PreferencesDialog::checkUpdateNowClicked_Signal,
            this, &As::Window::checkApplicationUpdateNow_Slot);

    //dialog->setWindowModality(Qt::WindowModal);
    //dialog->setWindowFlags((dialog->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    //dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->exec();
    //dialog->show();
}

/*!
Runs the program in the auto mode, to quickly go through all the data treatment processes.
*/
void As::Window::autoProcessing_Slot()
{
    ADEBUG;

    extractScans_Slot();
    visualizePlots_Slot();
    showOutput_Slot();
    exportOutputTable_Slot();
}

/*!
Shows or hides the sidebar.
*/
void As::Window::showSidebar_Slot(const bool show)
{
    ADEBUG << "show:" << show;

    m_sidebarWidget->setVisible(show);
}

/*!
Shows or hide sidebar group boxes depends on the selected tab of mainTabs.
*/
void As::Window::showOrHideSidebarBlocks_Slot(const int index)
{
    ADEBUG << "index:" << index;

    // Hide all the blocks
    for (int i = 1; i < m_sidebarControlsLayout->count(); ++i) // besides the common one with index = 0
        m_sidebarControlsLayout->itemAt(i)->widget()->hide();
    for (int i = 0; i < m_sidebarSettingsLayout->count(); ++i)
        m_sidebarSettingsLayout->itemAt(i)->widget()->hide();

    // Show the required blocks only
    if (index == 0) {
        m_textControlsBlock->show();
        m_textSettingsBlock->show(); }

    else if (index == 1) {
        m_tableControlsBlock->show();
        m_tableSettingsBlock->show(); }

    else if (index == 2) {
        m_plotControlsBlock->show();
        m_plotSettingsBlock->show(); }

    else if (index == 3) {
        m_outputControlsBlock->show();
        m_outputSettingsBlock->show(); }
}

/*!
Opens online user manual in web browser.
*/
void As::Window::openUserManual_Slot()
{
    ADEBUG;

    //QString link = "http://www.google.com";
    QDesktopServices::openUrl(QUrl(USERMANUAL_URL));
}

/*!
Opens online issue tracker in web browser.
*/
void As::Window::openIssueTracker_Slot()
{
    ADEBUG;

    //QString link = "http://www.google.com";
    QDesktopServices::openUrl(QUrl(ISSUETRACKER_URL));
}

/*!
...
*/
void As::Window::acceptAutoUpdate_Slot()
{
    ADEBUG;

    setAutoUpdate_Slot(true);
}

/*!
...
*/
void As::Window::rejectAutoUpdate_Slot()
{
    ADEBUG;

    setAutoUpdate_Slot(false);
}

/*!
...
*/
void As::Window::setAutoUpdate_Slot(const bool autoUpdate)
{
    ADEBUG << "autoUpdate:" << autoUpdate;

    QSettings().setValue("Preferences/autoUpdate", autoUpdate);
}


/*!
...
*/
void As::Window::checkApplicationUpdateNow_Slot(const bool hideOutput)
{
    ADEBUG << "hideOutput:" << hideOutput;

    m_hideUpdateOutput = hideOutput;

    // Start the external maintainer tool to check for updates
    QProcess updater;
    updater.start(As::Window::maintainerPath(), QStringList("--checkupdates"));
    updater.waitForFinished();

    // Read the output
    const QByteArray data = updater.readAllStandardOutput();
    const QByteArray err = updater.readAllStandardError();
    //ADEBUG << "data:\n" << data;
    //ADEBUG << "err:\n" << err;

    // Regular expression for app version in format 1.0.12
    const QRegularExpression re("\\d+\\.\\d+\\.\\d+");
    const QRegularExpressionMatch match = re.match(QString(data));

    // Check if update is found
    const bool updateFound = match.hasMatch();
    if (!updateFound AND m_hideUpdateOutput)
        return;

    // Get new app version from the web repository
    const QString webVersion = match.captured(0);

    // User dialog settings
    const QString title = QMessageBox::tr("Update");

    const QString newVersionFound = QMessageBox::tr(
                "<p align='center'><big><b>A new version of %1 is available!</b></big></p>"
                "<p align='center'>%1 %2 is now available.<br />"
                "You are currently using version %3.</p>"
                "<p align='center'>For details, see <a href=\"%4\" style=\"color: %5\">%6</a></p>"
                "<p align='center'>Do you want to restart and install update?</p>")
            .arg(APP_NAME)
            .arg(webVersion)
            .arg(APP_VERSION)
            .arg(APP_URL).arg(As::Color(As::blueDarkVery).name())
            .arg(QString(APP_URL).remove(QRegularExpression("http.*/")));

    const QString upToDate = QMessageBox::tr(
                "<p align='center'><big><b>You are up to date!</b></big></p>"
                "<p align='center'>%1 %2 (%3) is currently the<br />newest version available.</p>")
            .arg(APP_NAME).arg(APP_VERSION).arg(APP_RELEASE_DATE);

    // Show user dialog depends on whether update is found
    if (updateFound) {
        const QString okButton = QMessageBox::tr("Install Update Now");
        const QString cancelButton = QMessageBox::tr("Remind Me Later");
        As::MessageWidget dialog(this, title, newVersionFound, okButton, cancelButton);
        connect(&dialog, &QDialog::accepted, this, &As::Window::installUpdate_Slot);
        dialog.exec(); }

    else {
        As::MessageWidget dialog(this, title, upToDate);
        dialog.exec(); }
}

/*!
...
*/
void As::Window::installUpdate_Slot()
{
    ADEBUG;

    //QStringList list = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);
    //QString dir = QDir::toNativeSeparators(QString("%1/%2").arg(list[list.size()-1]).arg(APP_NAME));

    // Start the external maintenance tool as detached process
    bool updaterStarted = QProcess::startDetached(As::Window::maintainerPath(), QStringList("--updater"));

    // Close application for the update if external updater is started
    if (updaterStarted)
        exit(0);
}


/*
// not in use
void As::Window::networkReplyFinished_Slot(QNetworkReply *reply)
{
    ADEBUG;

    const QVariant redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    //emit As::Window::applicationUrlReceived_Signal(redirectUrl.toString());
    compareAppVersions_Slot(redirectUrl.toUrl());
}
*/

