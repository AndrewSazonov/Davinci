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

//=======================
// Common sidebar widgets
//=======================

/*!
...
*/
void As::Window::gotoScan_Slot(const int index)
{
    ADEBUG_H1;
    ADEBUG << "index:" << index;

    // Update the file if nesessary. Should be at the beginning?
    gotoFile_Slot(scanAt(index)->fileIndex());
    //m_scans->setFileIndex(scanAt(index)->fileIndex());
    //m_scans->setFileIndexByScanIndex(index);
    //emit currentFileIndexChanged_Signal(scanAt(index)->fileIndex());

    // Set the scan index
    emit currentScanChanged_Signal(index);

    //if (!scanAt(index)->m_isIndividuallyTreated) {
    if (!currentScan()->m_isIndividuallyTreated) {
        currentScan()->m_removeNeighborsType = genericScan()->m_removeNeighborsType;

        currentScan()->m_numLeftSkipPoints = genericScan()->m_numLeftSkipPoints;
        currentScan()->m_numRightSkipPoints = genericScan()->m_numRightSkipPoints;

        currentScan()->m_integrationType = genericScan()->m_integrationType;
        currentScan()->m_integrationSubType = genericScan()->m_integrationSubType;

        currentScan()->m_numLeftBkgPoints = genericScan()->m_numLeftBkgPoints;
        currentScan()->m_numRightBkgPoints = genericScan()->m_numRightBkgPoints;
    }

    // Treat current scan conditions
    if (genericScan()->plotType() == As::PlotType::Integrated AND currentScan()->plotType() != As::PlotType::Excluded)
        m_scans->treatSingle(index-1);

    //
    m_scans->setScanIndex(index);

    // Update the common widgets
    if (scanAt(index)->plotType() == As::PlotType::Excluded)
        emit excludeScanStateChanged_Signal(true);
    else
        emit excludeScanStateChanged_Signal(false);

    // Update the extracted tables
    emit extractedTableModelChanged(scanAt(index)->m_tableModel);

    // Update the text widget
    m_inputTextWidget->setCursorPosition(currentScan()->scanLine());

    // Update the visualized plot
    if (m_visualizedPlotsWidget) {
        m_visualizedPlotsWidget->updateAllOnPlot(scanAt(index));
        //updateChangeScanGroup(scanAt(index));
        update_Plot_ExpDetailsGroup(scanAt(index));
        update_Plot_ExpAnglesGroup(scanAt(index));
        emit individualTreatStateChanged_Signal(scanAt(index)->m_isIndividuallyTreated);
        update_Plot_ScanCorrectGroup(scanAt(index));
        update_Plot_PeakIntegrateGroup(scanAt(index)); }

    // Update the output table
    if (m_outputTableWidget) {
        // next to lines are done when we switch to the output table tab only
        //m_scans->createFullOutputTable(); // slows down scan change via go to
        //createFullOutputTableModel_Slot(); // further slows down...
        update_OutputTable_Highlight(index-1); }
}

/*!
...
*/
void As::Window::updateScan_Slot()
{
    ADEBUG << m_scans->scanIndex();

    gotoScan_Slot(m_scans->scanIndex());
}

/*!
...
*/
void As::Window::excludeScan_Slot(const bool exclude)
{
    ADEBUG << "exclude:" << exclude;

    currentScan()->setPlotType(exclude ? As::PlotType::Excluded : genericScan()->plotType());
    currentScan()->setData("number", "Excluded", exclude ? "1" : "0"); // init in preTreatData()

    updateScan_Slot();
}

//================
// Text - Controls
//================

/*!
...
*/
void As::Window::gotoFile_Slot(const int index)
{
    ADEBUG << "index:" << index;

    //if (currentScan() != Q_NULLPTR AND currentScan()->fileIndex() == index)
    //    return;

    emit currentFileIndexChanged_Signal(index);
    emit currentFilePathChanged_Signal(m_scans->m_inputFilesContents.first[index-1]);
    emit currentFileContentChanged_Signal(m_scans->m_inputFilesContents.second[index-1]);

    const int size = m_inputTextWidget->blockCount();
    emit linesRangeChanged_Signal(1, size);
    emit linesCountChanged_Signal(QString::number(size));
}

/*!
...
*/
void As::Window::gotoLine_Slot(const int index)
{
    ADEBUG << "index:" << index;

    // !?
    //connect(lineChanger, QOverload<int>::of(&As::SpinBox::valueChanged), m_inputTextWidget, TE&As::TextEditor::setCursorPosition);
    //m_inputTextWidget->setCursorPosition(index);
}

/*!
...
*/
void As::Window::ignoreCase_Slot(const bool ignore)
{
    ADEBUG << "ignore:" << ignore;

    QSettings().setValue("TextControls/ignoreCase", ignore);
}

/*!
...
*/
void As::Window::gotoMatch_Slot(const int index)
{
    ADEBUG << "index:" << index;

    if (index > 0)
        m_inputTextWidget->setTextCursor(m_searchMatches[index-1]);
}

/*!
...
*/
void As::Window::extractScans_Slot()
{
    ADEBUG_H3;

    // Extract data from raw input
    m_scans->extractInputData();

    // Exit from function if no scans were found
    if (m_scans->size() == 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("No scans are found");
        msgBox.exec();
        return; }

    // Create widget if not yet created
    //if (m_extractedTableWidget == Q_NULLPTR) {}

    // Create table widget
    auto extractedTableWidget = new As::TableView;

    // Add widget to the tabs
    m_tabsWidget->addTab(extractedTableWidget, "Extracted Tables");

    // Fill data
    m_scans->fillEmptyArrays();

    // Ceate extracted data table
    m_scans->createAllExtractedTablesModels();

    // Update the table model conditions
    connect(this, &As::Window::extractedTableModelChanged, extractedTableWidget, &As::TableView::setModel);

    // Emit signal(s)
    const int size = m_scans->size();
    if (size > 0) {
        emit newScansExtracted_Signal(1);
        emit scansRangeChanged_Signal(1, size);
        emit scansCountChanged_Signal(QString::number(size)); }

    // Set focus
    m_tabsWidget->setCurrentWidget(extractedTableWidget);
}

//================
// Text - Settings
//================

/*!
...
*/
void As::Window::wrapText_Slot(const bool wrap)
{
    ADEBUG << "wrap:" << wrap;

    QSettings().setValue("TextSettings/wrapText", wrap);

    QPlainTextEdit::LineWrapMode mode = QPlainTextEdit::NoWrap;
    if (wrap)
        mode = QPlainTextEdit::WidgetWidth;

    m_inputTextWidget->setLineWrapMode(mode);
}

/*!
...
*/
void As::Window::highlightSyntax_Slot(const bool highlight)
{
    ADEBUG << "highlight:" << highlight;

    QSettings().setValue("TextSettings/highlightSyntax", highlight);

    /*
    tobeHighligthed = checked;
    QString type("");

    if (tobeHighligthed)
        type.append(inputFileType);
    //else
    //    type.append("");
    */

    // Highlight syntax
    QString type("");
    if (highlight)
        type.append(m_scans->m_instrumentType + " " + m_scans->m_dataType);
    new As::SyntaxHighlighter(m_inputTextWidget->document(), type);
}

/*!
...
*/
void As::Window::setFont_Slot(const QFont &font)
{
    ADEBUG << "font:" << font;

    QSettings().setValue("TextSettings/fontFamily", font.family());
    QSettings().setValue("TextSettings/fontSize", font.pointSize());

    m_inputTextWidget->setFont(font);
    m_inputTextWidget->updateLineNumberAreaFontSize(font);
}

/*!
...
*/
void As::Window::setFontSize_Slot(const int size)
{
    ADEBUG << "size:" << size;

    auto font = m_inputTextWidget->font();
    font.setPointSize(size);

    setFont_Slot(font);
}

/*!
...
*/
void As::Window::highlightCurrentScanLines_Slot(const int fileIndex)
{
    //ADEBUG;

    if (currentScan() AND fileIndex == currentScan()->fileIndex())
        highlightScanLines_Slot(currentScanIndex());
}

/*!
...
*/
void As::Window::highlightScanLines_Slot(const int scanIndex)
{
    //ADEBUG << "scanIndex:" << scanIndex;
    //m_inputTextWidget->clearAllSelections();

    if (scanIndex == 0)
        return;

    if (scanIndex != m_scans->scanIndex())
        return;

    QList<QTextEdit::ExtraSelection> selections;
    QTextEdit::ExtraSelection selection;

    // Highlighting format
    selection.format.setBackground(As::Color(As::greenLightTransparent));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);

    // Define lines to be highlighted
    selection.cursor = m_inputTextWidget->textCursor();
    bool ok;
    const QStringList lines = m_scans->at(scanIndex-1)->data("misc", "lines", &ok).split(" ");
    for (const QString &line : lines) {
        bool ok;
        const int num = line.toInt(&ok);
        if (ok) {
            const int pos = m_inputTextWidget->document()->findBlockByLineNumber(num).position();
            selection.cursor.setPosition(pos);
            selections.append(selection); } }

    // Go to the line just before the current scan in mainTextEdit
    if (lines.size() > 0) {
        bool ok;
        const int lineIndex = lines[0].toInt(&ok);
        ///ADEBUG << "???" << lineIndex;
        if (ok)
            m_inputTextWidget->setCursorPosition(lineIndex); }

    // Set selections
    m_inputTextWidget->setCurrentScanSelections(selections);
}

/*!
...
*/
void As::Window::textSearchTimer_Slot()
{
    // m_delayBeforeSearching was created with findField in create_Text_FindGroup
    // it allows to type more then 1 symbol before start searching of the document
    // Disconnect m_delayBeforeSearching from previous signal-slot connections
    m_delayBeforeSearching->disconnect();

    // Start timer for 1 second
    m_delayBeforeSearching->start(1000);

    // When m_delayBeforeSearching is timeout, run highlightTextFound
    // If within 1 second findField text was edited again then
    // previous signal-slot is disconnected and new will be set for 1 second
    //connect(m_delayBeforeSearching, SIGNAL(timeout()), this, SLOT(highlightFoundText_Slot()));
    connect(m_delayBeforeSearching, &QTimer::timeout, this, &As::Window::highlightFoundText_Slot);
}

/*!
...
*/
void As::Window::highlightFoundText_Slot()
{
    ADEBUG;

    // Move cursor to start
    m_inputTextWidget->moveCursor(QTextCursor::Start);

    // Read previously saved application settings
    const auto isCaseIgnored = QSettings().value("TextControls/ignoreCase", true).toBool();

    // Flags for search
    QTextDocument::FindFlags flags = 0;
    if (!isCaseIgnored)
        flags |= QTextDocument::FindCaseSensitively;

    // Make alias to find field widget
    auto findField = findChild<As::LineEdit*>("findField");

    // Text for search
    const QString text = findField->text();

    // Highlighting format
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(As::Color(As::yellowLightVery));
    selection.format.setForeground(As::Color(As::grayLight));

    // Find all positions
    QList<QTextEdit::ExtraSelection> selections;
    m_searchMatches.clear();
    while (m_inputTextWidget->find(text, flags)) {
        selection.cursor = m_inputTextWidget->textCursor();
        selections.append(selection); // this slows down the searching. need to highlight only the visible block of text!?
        m_searchMatches.append(selection.cursor); }

    // Emit signal(s)
    const int numMatches = m_searchMatches.count();
    emit matchesRangeChanged_Signal(qMin(1, numMatches), numMatches);
    emit matchesCountChanged_Signal(QString::number(numMatches));

    // Go to the first match
    if (numMatches > 0) {
        m_inputTextWidget->setTextCursor(m_searchMatches[0]);
        matchesIndexChanged_Signal(1);}

    // Define color for the findField depends on searching result
    As::Color color;
    if (numMatches > 0)
        color = As::blue;
    else /* numMatches == 0 */
        if (findField->text() != "")
            color = As::red;
        else
            color = As::gray;
    findField->setStyleSheet(QString("#findField {color: %1}").arg(color.name()));

    // Set selections
    m_inputTextWidget->setFoundTextSelections(selections);
}

//=================
// Table - Controls
//=================

/*!
...
*/
void As::Window::visualizePlots_Slot()
{
    ADEBUG_H3;

    // Create widget if not yet created
    if (m_visualizedPlotsWidget == Q_NULLPTR) {
        m_visualizedPlotsWidget = new As::Plot();
        m_tabsWidget->addTab(m_visualizedPlotsWidget, "Visualized Plots");

        // Index
        m_scans->indexPeaks();
        m_scans->calcDirectionCosines();

        // Preliminary Treat
        m_scans->preTreatData();

        // Emit signal(s)
        ///const int size = m_scans->size();
        ///if (size > 0) {
        ///    emit newScansPlotted_Signal(currentScanIndex()-1); }
        }

    // ...
    //updateCurrentScan();
    // Set the scan index
    gotoScan_Slot(currentScanIndex()); // m_scans->scanIndex()

    // Set focus
    m_tabsWidget->setCurrentWidget(m_visualizedPlotsWidget);
}

//================
// Plot - Controls
//================

/*!
...
*/
void As::Window::treatIndividually_Slot(const bool treat)
{
    ADEBUG << "treat:" << treat;

    currentScan()->m_isIndividuallyTreated = treat; // make slot in As::Scan!
}

/*!
...
*/
void As::Window::selectRemoveNeighborsType_Slot(const QString &type)
{
    ADEBUG << "Remove neighbors type:" << type;

    if (currentScan()->m_isIndividuallyTreated)
        currentScan()->m_removeNeighborsType = type; // make slot in As::Scan?
    else
        genericScan()->m_removeNeighborsType = type;

    updateScan_Slot();
}

/*!
...
*/
void As::Window::setLeftSkipCount_Slot(const int count)
{
    ADEBUG << "LeftSkipCount:" << count;

    if (currentScan()->m_isIndividuallyTreated)
        currentScan()->m_numLeftSkipPoints = count; // make slot in As::Scan?
    else
        genericScan()->m_numLeftSkipPoints = count;

    updateScan_Slot();

}

/*!
...
*/
void As::Window::setRightSkipCount_Slot(const int count)
{
    ADEBUG << "RightSkipCount:" << count;

    if (currentScan()->m_isIndividuallyTreated)
        currentScan()->m_numRightSkipPoints = count; // make slot in As::Scan?
    else
        genericScan()->m_numRightSkipPoints = count;

    updateScan_Slot();
}

/*!
...
*/
void As::Window::selectIntegrationSubType_Slot(const QString &type)
{
    ADEBUG << "Integration sub type:" << type;

    if (currentScan()->m_isIndividuallyTreated)
        currentScan()->m_integrationSubType = type; // make slot in As::Scan?
    else
        genericScan()->m_integrationSubType = type;

    //currentScan()->m_integrationSubType = genericScan()->m_integrationSubType;

    /*
    currentScan()->m_integrationSubType = type; // make slot in As::Scan?

    if (!currentScan()->m_isIndividuallyTreated) {
        genericScan()->m_integrationSubType = type;

        currentScan()->m_numLeftSkipPoints = genericScan()->m_numLeftSkipPoints;
        currentScan()->m_numRightSkipPoints = genericScan()->m_numRightSkipPoints;
        currentScan()->m_numLeftBkgPoints = genericScan()->m_numLeftBkgPoints;
        currentScan()->m_removeNeighborsType = genericScan()->m_removeNeighborsType;
        currentScan()->m_integrationType = genericScan()->m_integrationType;
        currentScan()->m_integrationSubType = genericScan()->m_integrationSubType;
    }
    */

    updateScan_Slot();
}

/*!
...
*/
void As::Window::setLeftBkgCount_Slot(const int count)
{
    ADEBUG << "LeftBkgCount:" << count;

    //if (currentScan()->m_integrationSubType == "Automatically detect background")
    //    return;

    if (currentScan()->m_isIndividuallyTreated)
        currentScan()->m_numLeftBkgPoints = count; // make slot in As::Scan?
    else
        genericScan()->m_numLeftBkgPoints = count;

    updateScan_Slot();
}

/*!
...
*/
void As::Window::setRightBkgCount_Slot(const int count)
{
    ADEBUG << "RightBkgCount:" << count;

    //if (currentScan()->m_integrationSubType == "Automatically detect background")
    //    return;

    if (currentScan()->m_isIndividuallyTreated)
        currentScan()->m_numRightBkgPoints = count; // make slot in As::Scan?
    else
        genericScan()->m_numRightBkgPoints = count;

    updateScan_Slot();
}

/*!
...
*/
void As::Window::calcStructureFactor_Slot()
{
    ADEBUG_H3;

    genericScan()->setPlotType(As::PlotType::Integrated);

    updateScan_Slot();
}

/*!
...
*/
void As::Window::showOutput_Slot()
{
    ADEBUG_H3;

    // Create widget if not yet created
    if (m_outputTableWidget == Q_NULLPTR) {
        m_outputTableWidget = new As::TableView();
        m_tabsWidget->addTab(m_outputTableWidget, "Output Table");

        // Treat all scans
        m_scans->treatData();

        // Emit signal(s)
        //const int size = m_scans->size();
        //if (size > 0) {
        //    emit newPeaksIntegrated(1);
        //}
        }

    // Create/updadate full output data table
    createFullOutputTableModel_Slot();

    // Set focus
    m_tabsWidget->setCurrentWidget(m_outputTableWidget);

    ADEBUG;
}

//================
// Plot - Settings
//================

/*!
...
*/
void As::Window::hideLegend_Slot(const bool hide)
{
    ADEBUG << "hide:" << hide;

    QSettings().setValue("PlotSettings/hideLegend", hide);

    // !?
    m_visualizedPlotsWidget->updateAllOnPlot(scanAt(currentScanIndex()));
}

//==================
// Output - Settings
//==================

/*!
...
*/
void As::Window::exportExcluded_Slot(const bool save)
{
    ADEBUG << "save:" << save;

    QSettings().setValue("OutputSettings/exportExcluded", save);
}

/*!
...
*/
void As::Window::alwaysSaveHeaders_Slot(const bool save)
{
    ADEBUG << "save:" << save;

    QSettings().setValue("OutputSettings/alwaysSaveHeaders", save);
}

