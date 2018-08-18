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

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
//#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QToolBar>
#include <QTimer>
#include <QUrl>

#include "Colors.hpp"
#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "ComboBox.hpp" //???
#include "HBoxLayout.hpp"
#include "Label.hpp"
#include "LabelQuatroBlock.hpp"
#include "LabelTripleBlock.hpp"
#include "LineEdit.hpp"
#include "RealVector.hpp"
#include "SpinBox.hpp"
#include "Style.hpp"
#include "TableView.hpp"
#include "TextEditor.hpp"
#include "ToolBarButton.hpp"
#include "ToolBarSpacer.hpp"
#include "UnderLabeledWidget.hpp"

#include "Scan.hpp"
#include "ScanDict.hpp"

#include "Window.hpp"

/*!
    Updates the group 'Plot - Experimental details'
*/
void As::Window::update_Plot_ExpDetailsGroup(const As::Scan* scan) {
    ADEBUG;

    const QString group = "conditions";

    // First, hide all the lines
    for (const auto& element : As::Scan::Properties[group].keys()) {
        auto widget = findChild<As::LabelQuatroBlock*>(group + element + "Widget");
        widget->hide(); }

    // Now, show only the required lines
    for (const auto& element : (*scan)[group].keys()) {
        const QString data = scan->data(group, element);
        if (!data.isEmpty()) {
            auto widget = findChild<As::LabelTripleBlock*>(group + element + "Widget");
            widget->show();
            auto label = findChild<QLabel*>(group + element + "Data");
            label->setText(scan->printDataRange(group, element)); } } }

/*!
    Updates the group 'Plot - Experimental angles'
*/
void As::Window::update_Plot_ExpAnglesGroup(const As::Scan* scan) {
    ADEBUG;

    const QString group = "angles";

    // First, hide all the lines
    for (const auto& element : As::Scan::Properties[group].keys()) {
        auto widget = findChild<As::LabelQuatroBlock*>(group + element + "Widget");
        widget->hide(); }

    // Now, show only the required lines
    for (const auto& element : (*scan)[group].keys()) {
        const QString data = scan->data(group, element);
        if (!data.isEmpty()) {
            auto widget = findChild<As::LabelQuatroBlock*>(group + element + "Widget");
            widget->show();

            // Update the value, range and step
            const As::RealVector vector(data);

            auto value = findChild<QLabel*>(group + element + "Value");
            value->setText(QString::number(vector.mean(), 'f', 2));
            auto range = findChild<QLabel*>(group + element + "Range");
            range->setText(QString::number(vector.range(), 'f', 2));
            auto step = findChild<QLabel*>(group + element + "Step");
            step->setText(QString::number(vector.step(), 'f', 2));

            // Modify color of non-zero ranges and steps
            QString color = As::Color(As::grayLight).name();
            if (vector.step() != 0.) {
                color = As::Color(As::blue).name(); }

            // Change style
            range->setStyleSheet(QString("QLabel {color: %1}").arg(color));
            step->setStyleSheet(QString("QLabel {color: %1}").arg(color)); } } }

/*!
    Updates the group 'Plot - Scan corrections'
*/
void As::Window::update_Plot_ScanCorrectGroup(const As::Scan* scan) {
    ADEBUG;

    // Enable or disable left and right skip counts spin boxes
    const bool b = (scan->neighborsRemoveType() == As::Scan::AutoNeighborsRemove);
    m_leftSkipCount->setDisabled(b);
    m_rightSkipCount->setDisabled(b);

    // Set remove neighbor type
    m_removeNeighborsType->setCurrentText(As::Scan::NeighborsRemoveTypeDict[scan->neighborsRemoveType()]);

    // Set number of non-skipped points
    m_nonSkipPointsCount->setText(QString::number(scan->m_numNonSkipPoints)); // chose Count or Num everywhere?!

    // Calc max numbers of left and right points to be skipped for spin boxes' ranges
    int numBkgPoints = scan->m_numLeftBkgPoints + scan->m_numRightBkgPoints;
    int numLeftSkipPoints = scan->m_numLeftSkipPoints;
    int numRightSkipPoints = scan->m_numRightSkipPoints;
    int numPoints = scan->numPoints();
    int maxNumLeftSkipPoints = numPoints - numBkgPoints - numRightSkipPoints - 1;
    int maxNumRightSkipPoints = numPoints - numBkgPoints - numLeftSkipPoints - 1;

    // Set left and right background maximum and current values
    m_leftSkipCount->setMaximumSilently(maxNumLeftSkipPoints);
    m_leftSkipCount->setValueSilently(numLeftSkipPoints);
    m_rightSkipCount->setMaximumSilently(maxNumRightSkipPoints);
    m_rightSkipCount->setValueSilently(numRightSkipPoints); }

/*!
    Updates the group 'Plot - Peak integration'
*/
void As::Window::update_Plot_PeakIntegrateGroup(const Scan* scan) {
    ADEBUG;

    // Enable or disable left and right background counts spin boxes
    const bool b = (scan->bkgDetectType() == As::Scan::AutoBkgDetect);
    m_leftBkgCount->setDisabled(b);
    m_rightBkgCount->setDisabled(b);

    // Set background type
    m_backgroundType->setCurrentText( As::Scan::BkgDetectTypeDict[ scan->bkgDetectType() ] );

    // Set number of peak points
    m_peakPointsCount->setText(QString::number(scan->m_numPeakPoints));

    // Calc max numbers of left and right background points for spin boxes' ranges
    int numSkipPoints = scan->m_numLeftSkipPoints + scan->m_numRightSkipPoints;
    int numLeftBkgPoints = scan->m_numLeftBkgPoints;
    int numRightBkgPoints = scan->m_numRightBkgPoints;
    int numPoints = scan->numPoints();
    int maxNumLeftBkgPoints = numPoints - numSkipPoints - numRightBkgPoints - 1;
    int maxNumRightBkgPoints = numPoints - numSkipPoints - numLeftBkgPoints - 1;

    // Set left and right background maximum and current values
    m_leftBkgCount->setMaximumSilently(maxNumLeftBkgPoints);
    m_leftBkgCount->setValueSilently(numLeftBkgPoints);
    m_rightBkgCount->setMaximumSilently(maxNumRightBkgPoints);
    m_rightBkgCount->setValueSilently(numRightBkgPoints);

    ADEBUG; }

/*!
    Updates the output table highlighting
*/
void As::Window::update_OutputTable_Highlight(const int index) {
    ADEBUG << index;

    if (m_outputTableWidget) {
        m_outputTableWidget->selectRow(index); }

    ADEBUG; }
