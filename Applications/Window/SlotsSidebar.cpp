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
#include <QVector>
#include <QXmlStreamWriter>

#include <QtConcurrent>

#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "ConcurrentWatcher.hpp"
#include "LineEdit.hpp"
#include "MessageWidget.hpp"
#include "TextEditor.hpp"
#include "SaveHeaders.hpp"
#include "SpinBox.hpp"
#include "SyntaxHighlighter.hpp"
#include "Plot.hpp"
#include "PreferencesDialog.hpp"
#include "ProgressDialog.hpp"
#include "TableView.hpp"
#include "VBoxLayout.hpp"

#include "Window.hpp"

//=======================
// Common sidebar widgets
//=======================

/*!
    ...
*/
void As::Window::gotoScanSlot(const int index) {
    ADEBUG_H1;
    ADEBUG << "index:" << index;

    // Update the file if nesessary. Should be at the beginning?
    gotoFileSlot(scanAt(index)->fileIndex());
    //m_scans->setFileIndex(scanAt(index)->fileIndex());
    //m_scans->setFileIndexByScanIndex(index);
    //emit currentFileIndexChangedSignal(scanAt(index)->fileIndex());

    // Set the scan index
    emit currentScanChangedSignal(index);

    //if (!scanAt(index)->m_isIndividuallyTreated) {
    if (!currentScan()->isIndividuallyTreated()) {
        currentScan()->setNeighborsRemoveType( genericScan()->neighborsRemoveType() );

        currentScan()->m_numLeftSkipPoints = genericScan()->m_numLeftSkipPoints;
        currentScan()->m_numRightSkipPoints = genericScan()->m_numRightSkipPoints;

        currentScan()->setPeakAnalysisType( genericScan()->peakAnalysisType() );
        currentScan()->setBkgDetectType( genericScan()->bkgDetectType() );
        currentScan()->setPeakFitType( genericScan()->peakFitType() );

        currentScan()->m_numLeftBkgPoints = genericScan()->m_numLeftBkgPoints;
        currentScan()->m_numRightBkgPoints = genericScan()->m_numRightBkgPoints; }

    // Treat current scan conditions
    if (genericScan()->plotType() == As::PlotType::Integrated AND currentScan()->plotType() != As::PlotType::Excluded) {
        m_scans->treatSinglePeak(index - 1); }

    //
    m_scans->setScanIndex(index);

    // Update the common widgets
    if (scanAt(index)->plotType() == As::PlotType::Excluded) {
        emit excludeScanStateChangedSignal(true); }
    else {
        emit excludeScanStateChangedSignal(false); }

    // Update the extracted tables
    emit extractedTableModelChanged(scanAt(index)->extractedTableModel());

    // Update the text widget
    m_inputTextWidget->setCursorPosition(currentScan()->scanLine());

    // Update the visualized plot
    if (m_visualizedPlotsWidget) {
        m_visualizedPlotsWidget->updateAllOnPlot(scanAt(index));
        //updateChangeScanGroup(scanAt(index));
        update_Plot_ExpDetailsGroup(scanAt(index));
        update_Plot_ExpAnglesGroup(scanAt(index));
        emit individualTreatStateChangedSignal(scanAt(index)->isIndividuallyTreated());
        update_Plot_ScanCorrectGroup(scanAt(index));
        update_Plot_PeakIntegrateGroup(scanAt(index)); }

    // Update the output table
    if (m_outputTableWidget) {
        // next to lines are done when we switch to the output table tab only
        //m_scans->createFullOutputTable(); // slows down scan change via go to
        //createFullOutputTableModelSlot(); // further slows down...
        update_OutputTable_Highlight(index - 1); } }

/*!
    ...
*/
void As::Window::updateScanSlot() {
    ADEBUG << m_scans->scanIndex();

    gotoScanSlot(m_scans->scanIndex()); }

/*!
    ...
*/
void As::Window::excludeScanSlot(const bool exclude) {
    ADEBUG << "exclude:" << exclude;

    currentScan()->setPlotType(exclude ? As::PlotType::Excluded : genericScan()->plotType());
    currentScan()->setData("number", "Excluded", exclude ? "1" : "0"); // init in preTreatData()

    updateScanSlot(); }

//================
// Text - Controls
//================

/*!
    ...
*/
void As::Window::gotoFileSlot(const int index) {
    ADEBUG << "index:" << index;

    //if (currentScan() != Q_NULLPTR AND currentScan()->fileIndex() == index)
    //    return;

    emit currentFileIndexChangedSignal(index);
    emit currentFilePathChangedSignal(m_scans->m_inputFilesContents.first[index - 1]);
    emit currentFileContentChangedSignal(m_scans->m_inputFilesContents.second[index - 1]);

    const int size = m_inputTextWidget->blockCount();
    emit linesRangeChangedSignal(1, size);
    emit linesCountChangedSignal(QString::number(size)); }

/*!
    ...
*/
void As::Window::gotoLineSlot(const int index) {
    ADEBUG << "index:" << index;

    // !?
    //connect(lineChanger, QOverload<int>::of(&As::SpinBox::valueChanged), m_inputTextWidget, TE&As::TextEditor::setCursorPosition);
    //m_inputTextWidget->setCursorPosition(index);
}

/*!
    ...
*/
void As::Window::ignoreCaseSlot(const bool ignore) {
    ADEBUG << "ignore:" << ignore;

    QSettings().setValue("TextControls/ignoreCase", ignore); }

/*!
    ...
*/
void As::Window::gotoMatchSlot(const int index) {
    ADEBUG << "index:" << index;

    if (index > 0) {
        m_inputTextWidget->setTextCursor(m_searchMatches[index - 1]); } }

/*!
    ...
*/
void As::Window::extractScansSlot() {
    ADEBUG_H3;

    // Extract data from raw input using multi-threading
    //m_scans->extractInputData();
    concurrentRun("extract", m_scans);


    // Exit from function if no scans were found
    if (m_scans->size() == 0) {
        As::MessageWidget(this, "", "Warning: No detectable scans were found.", "   OK   ", "", false).exec();
        return; }

    // Create widget if not yet created
    //if (m_extractedTableWidget == Q_NULLPTR) {}

    // Fill missing data using multi-threading
    concurrentRun("fill", m_scans);

    // Create table widget
    auto extractedTableWidget = new As::TableView;

    // Add widget to the tabs
    m_tabsWidget->addTab(extractedTableWidget, "Extracted Tables");

    // Update the table model conditions
    connect(this, &As::Window::extractedTableModelChanged, extractedTableWidget, &As::TableView::setModel);

    // Emit signal(s)
    const int size = m_scans->size();
    if (size > 0) {
        emit newScansExtractedSignal(1);
        emit scansRangeChangedSignal(1, size);
        emit scansCountChangedSignal(QString::number(size)); }

    // Set focus
    m_tabsWidget->setCurrentWidget(extractedTableWidget); }

//================
// Text - Settings
//================

/*!
    ...
*/
void As::Window::wrapTextSlot(const bool wrap) {
    ADEBUG << "wrap:" << wrap;

    QSettings().setValue("TextSettings/wrapText", wrap);

    QPlainTextEdit::LineWrapMode mode = QPlainTextEdit::NoWrap;
    if (wrap) {
        mode = QPlainTextEdit::WidgetWidth; }

    m_inputTextWidget->setLineWrapMode(mode); }

/*!
    ...
*/
void As::Window::highlightSyntaxSlot(const bool highlight) {
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
    if (highlight) {
        type.append(m_scans->m_instrumentType + " " + m_scans->m_dataType); }
    new As::SyntaxHighlighter(m_inputTextWidget->document(), type); }

/*!
    ...
*/
void As::Window::setFontSlot(const QFont& font) {
    ADEBUG << "font:" << font;

    QSettings().setValue("TextSettings/fontFamily", font.family());
    QSettings().setValue("TextSettings/fontSize", font.pointSize());

    m_inputTextWidget->setFont(font);
    m_inputTextWidget->updateLineNumberAreaFontSize(font); }

/*!
    ...
*/
void As::Window::setFontSizeSlot(const int size) {
    ADEBUG << "size:" << size;

    auto font = m_inputTextWidget->font();
    font.setPointSize(size);

    setFontSlot(font); }

/*!
    ...
*/
void As::Window::highlightCurrentScanLinesSlot(const int fileIndex) {
    //ADEBUG;

    if (currentScan() AND fileIndex == currentScan()->fileIndex()) {
        highlightScanLinesSlot(currentScanIndex()); } }

/*!
    ...
*/
void As::Window::highlightScanLinesSlot(const int scanIndex) {
    //ADEBUG << "scanIndex:" << scanIndex;
    //m_inputTextWidget->clearAllSelections();

    if (scanIndex == 0) {
        return; }

    if (scanIndex != m_scans->scanIndex()) {
        return; }

    QList<QTextEdit::ExtraSelection> selections;
    QTextEdit::ExtraSelection selection;

    // Highlighting format
    selection.format.setBackground(As::Color(As::greenLightTransparent));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);

    // Define lines to be highlighted
    selection.cursor = m_inputTextWidget->textCursor();
    bool ok;
    const QStringList lines = m_scans->at(scanIndex - 1)->data("misc", "lines", &ok).split(" ");
    for (const QString& line : lines) {
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
        if (ok) {
            m_inputTextWidget->setCursorPosition(lineIndex); } }

    // Set selections
    m_inputTextWidget->setCurrentScanSelections(selections); }

/*!
    ...
*/
void As::Window::textSearchTimerSlot() {
    // m_delayBeforeSearching was created with findField in create_Text_FindGroup
    // it allows to type more then 1 symbol before start searching of the document
    // Disconnect m_delayBeforeSearching from previous signal-slot connections
    m_delayBeforeSearching->disconnect();

    // Start timer for 1 second
    m_delayBeforeSearching->start(1000);

    // When m_delayBeforeSearching is timeout, run highlightTextFound
    // If within 1 second findField text was edited again then
    // previous signal-slot is disconnected and new will be set for 1 second
    //connect(m_delayBeforeSearching, SIGNAL(timeout()), this, SLOT(highlightFoundTextSlot()));
    connect(m_delayBeforeSearching, &QTimer::timeout, this, &As::Window::highlightFoundTextSlot); }

/*!
    ...
*/
void As::Window::highlightFoundTextSlot() {
    ADEBUG;

    // Move cursor to start
    m_inputTextWidget->moveCursor(QTextCursor::Start);

    // Read previously saved application settings
    const auto isCaseIgnored = QSettings().value("TextControls/ignoreCase", true).toBool();

    // Flags for search
    QTextDocument::FindFlags flags = 0;
    if (!isCaseIgnored) {
        flags |= QTextDocument::FindCaseSensitively; }

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
    emit matchesRangeChangedSignal(qMin(1, numMatches), numMatches);
    emit matchesCountChangedSignal(QString::number(numMatches));

    // Go to the first match
    if (numMatches > 0) {
        m_inputTextWidget->setTextCursor(m_searchMatches[0]);
        matchesIndexChangedSignal(1); }

    // Define color for the findField depends on searching result
    As::Color color;
    if (numMatches > 0) {
        color = As::blue; }
    else /* numMatches == 0 */
        if (findField->text() != "") {
            color = As::red; }
        else {
            color = As::gray; }
    findField->setStyleSheet(QString("#findField {color: %1}").arg(color.name()));

    // Set selections
    m_inputTextWidget->setFoundTextSelections(selections); }

//=================
// Table - Controls
//=================

/*!
    ...
*/
void As::Window::visualizePlotsSlot() {
    ADEBUG_H3;

    // Create widget if not yet created
    if (m_visualizedPlotsWidget == Q_NULLPTR) {
        m_visualizedPlotsWidget = new As::Plot();

        // Peak indexing using multi-threading
        //m_scans->indexPeaks();
        concurrentRun("index", m_scans);


        // Preliminary treatment using multi-threading
        // Included in "index" above!
        //m_scans->preTreatData();
        //As::ConcurrentWatcher("pretreat", m_scans, this);

        // Emit signal(s)
        ///const int size = m_scans->size();
        ///if (size > 0) {
        ///    emit newScansPlottedSignal(currentScanIndex()-1); }

        m_tabsWidget->addTab(m_visualizedPlotsWidget, "Visualized Plots"); }

    // ...
    //updateCurrentScan();
    // Set the scan index
    gotoScanSlot(currentScanIndex()); // m_scans->scanIndex()

    // Set focus
    m_tabsWidget->setCurrentWidget(m_visualizedPlotsWidget); }

//================
// Plot - Controls
//================

/*!
    ...
*/
void As::Window::treatIndividuallySlot(const bool b) {
    if (currentScan() == Q_NULLPTR) {
        return; }

    currentScan()->setIndividuallyTreated(b); }

/*!
    ...
*/
void As::Window::selectNeighborsRemoveType(int index) {
    const auto type = static_cast<As::Scan::NeighborsRemoveType>(index);

    ADEBUG << type;

    if (currentScan()->isIndividuallyTreated()) {
        currentScan()->setNeighborsRemoveType( type ); }
    else {
        genericScan()->setNeighborsRemoveType( type ); }

    updateScanSlot(); }

/*!
    ...
*/
void As::Window::setLeftSkipCountSlot(const int count) {
    ADEBUG << "LeftSkipCount:" << count;

    if (currentScan()->isIndividuallyTreated()) {
        currentScan()->m_numLeftSkipPoints = count; } // make slot in As::Scan?
    else {
        genericScan()->m_numLeftSkipPoints = count; }

    updateScanSlot();

}

/*!
    ...
*/
void As::Window::setRightSkipCountSlot(const int count) {
    ADEBUG << "RightSkipCount:" << count;

    if (currentScan()->isIndividuallyTreated()) {
        currentScan()->m_numRightSkipPoints = count; } // make slot in As::Scan?
    else {
        genericScan()->m_numRightSkipPoints = count; }

    updateScanSlot(); }


/*!
    ...
*/
void As::Window::selectPeakAnalysisType(int index) {
    const auto type = static_cast<As::Scan::PeakAnalysisType>(index);

    ADEBUG << type;

    if (currentScan()->isIndividuallyTreated()) {
        currentScan()->setPeakAnalysisType( type ); }
    else {
        genericScan()->setPeakAnalysisType( type ); }

    updateScanSlot(); }

/*!
    ...
*/
void As::Window::selectBkgDetectType(int index) {
    const auto type = static_cast<As::Scan::BkgDetectType>(index);

    ADEBUG << type;

    if (currentScan()->isIndividuallyTreated()) {
        currentScan()->setBkgDetectType( type ); }
    else {
        genericScan()->setBkgDetectType( type ); }

    updateScanSlot(); }

/*!
    ...
*/
void As::Window::selectPeakFitType(int index) {
    const auto type = static_cast<As::Scan::PeakFitType>(index);

    ADEBUG << type;

    if (currentScan()->isIndividuallyTreated()) {
        currentScan()->setPeakFitType( type ); }
    else {
        genericScan()->setPeakFitType( type ); }

    updateScanSlot(); }


/*!
    ...
*/
void As::Window::setLeftBkgCountSlot(const int count) {
    ADEBUG << "LeftBkgCount:" << count;

    if (currentScan()->isIndividuallyTreated()) {
        currentScan()->m_numLeftBkgPoints = count; } // make slot in As::Scan?
    else {
        genericScan()->m_numLeftBkgPoints = count; }

    updateScanSlot(); }

/*!
    ...
*/
void As::Window::setRightBkgCountSlot(const int count) {
    ADEBUG << "RightBkgCount:" << count;

    if (currentScan()->isIndividuallyTreated()) {
        currentScan()->m_numRightBkgPoints = count; } // make slot in As::Scan?
    else {
        genericScan()->m_numRightBkgPoints = count; }

    updateScanSlot(); }

/*!
    ...
*/
void As::Window::calcStructureFactorSlot() {
    ADEBUG_H3;

    genericScan()->setPlotType(As::PlotType::Integrated);

    updateScanSlot(); }

/*!
    ...
*/
void As::Window::showOutputSlot() {
    ADEBUG_H3;

    // Run data treatment using multi-threading
    concurrentRun("treat", m_scans);
    emit peaksTreatmentIsFinished(true);

    // Create widget if not yet created
    if (m_outputTableWidget == Q_NULLPTR) {
        m_outputTableWidget = new As::TableView();
        m_outputTableWidget->setObjectName("outputTableWidget");
        m_tabsWidget->addTab(m_outputTableWidget, "Output Table"); }

    // Create/updadate full output data table
    createFullOutputTableModelSlot();

    // Set focus
    m_tabsWidget->setCurrentWidget(m_outputTableWidget);

    ADEBUG; }

//================
// Plot - Settings
//================

/*!
    ...
*/
void As::Window::hideLegendSlot(const bool hide) {
    ADEBUG << "hide:" << hide;

    QSettings().setValue("PlotSettings/hideLegend", hide);

    // !?
    m_visualizedPlotsWidget->updateAllOnPlot(scanAt(currentScanIndex())); }

//==================
// Output - Settings
//==================

/*!
    ...
*/
void As::Window::exportExcludedSlot(const bool save) {
    ADEBUG << "save:" << save;

    QSettings().setValue("OutputSettings/exportExcluded", save); }

/*!
    ...
*/
void As::Window::alwaysSaveHeadersSlot(const bool save) {
    ADEBUG << "save:" << save;

    QSettings().setValue("OutputSettings/alwaysSaveHeaders", save); }

