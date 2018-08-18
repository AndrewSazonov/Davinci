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

#include <QSettings>
#include <QString>

#include "Colors.hpp"
#include "Constants.hpp"
#include "Macros.hpp"

#include "Style.hpp"

/*!
    \class As::Style

    \brief The Style is a singleton class that contains the settings for the
    program user interface.

    \inmodule Main
*/

/*!
    Constructs a style.
*/
As::Style::Style() {
    initStyle(); }

/*!
    Destroys the style.
*/
As::Style::~Style() {
    ADESTROYED; }

/*!
    Returns the only instance of the Style class as QString.
*/
QString& As::Style::ToQString() {
    static As::Style instance;
    return instance.m_style; }

/*!
    Starts the new style part named \a block.
*/
void As::Style::startBlock(const QString& block) {
    if (!m_style.isEmpty()) {
        closeBlock(); }
    m_style += block + "{"; }

/*!
    Closes the \a block.
*/
void As::Style::closeBlock() {
    m_style += "}"; }

/*!
    Appends \a property with \a value to the last created block.
*/
void As::Style::add(const QString& property,
                    const QString& value) {
    m_style += QString("%1 %2;").arg(property).arg(value); }

/*!
    Initializes the style.
*/
void As::Style::initStyle() {

    //==========
    // QGroupBox
    //==========

    startBlock("QGroupBox");
    add("color:", As::Color(As::grayDarkVery).name());
    add("font-weight:", "bold");
    add("border-bottom-style:", "solid");
    add("border-bottom-width:", "1px");
    add("border-bottom-color:", As::Color(As::grayLightVery).name());
    add("border-radius:", "0px");
    add("padding-top:", "12px");
    add("padding-bottom:", "2px");
    add("padding-left:", "2px");
    add("padding-right:", "2px");
    add("margin-top:", "12px");
    add("border-top-style:", "solid");

    startBlock("QGroupBox::title");
    add("subcontrol-origin:", "margin");
    add("subcontrol-position:", "top left");
    add("padding-left:", "9px");
    add("padding-right:", "9px");
    add("margin-top:", "12px");

    startBlock("QGroupBox[isHideable=false]");
    add("padding-top:", "-8px");

    startBlock("QGroupBox[isLast=true]");
    add("border-bottom-style:", "none");

    startBlock("QGroupBox::indicator:unchecked");
    add("image:", "url(:/Images/TriangleArrow-Right.svg)");

    startBlock("QGroupBox::indicator:checked");
    add("image:", "url(:/Images/TriangleArrow-Down.svg)");

    //==========
    // QCheckBox
    //==========

    startBlock("QCheckBox");
    add("spacing:", "8px");
    add("padding-top:", "4px");
    add("padding-bottom:", "4px");
    add("margin-right:", "9px");

    startBlock("QCheckBox::indicator");
    add("background-color:", As::Color(As::white).name());
    add("border-style:", "solid");
    add("border-width:", "1px");
    add("border-color:", As::Color(As::grayLightVery).name());
    add("border-radius:", "2px");
    add("margin-left:", "2px");
    add("width:", "13px");
    add("height:", "13px");
    add("padding:", "0px");

    startBlock("QCheckBox::indicator:checked");
    add("image:", "url(:/Images/Checkmark.svg)");

    //==========
    // QComboBox
    //==========

    startBlock("QComboBox");
    add("color:", As::Color(As::black).name());
    add("background-color:", As::Color(As::white).name());
    add("border-style:", "solid");
    add("border-width:", "1px");
    add("border-color:", As::Color(As::grayLightVery).name());
    add("border-radius:", "4px");

    startBlock("QComboBox:disabled");
    add("color:", As::Color(As::gray).name());

    startBlock("QComboBox QAbstractItemView");
    add("background-color:", As::Color(As::white).name());
    add("selection-background-color:", As::Color(As::blue).name());
    add("padding-top:", "1px");
    add("padding-bottom:", "1px");

    startBlock("QComboBox QAbstractItemView::item");
    add("margin-top:", "1px");
    add("margin-bottom:", "1px");

    startBlock("QComboBox::drop-down");
    add("border-style:", "none");

    startBlock("QComboBox::down-arrow");
    add("image:", "url(:/Images/Arrows-UpDown.svg)");
    add("width:", "9px");

    //==========
    // QLineEdit
    //==========

    startBlock("QLineEdit");
    add("background-color:", As::Color(As::white).name());
    add("color:", As::Color(As::blue).name());
    add("border-style:", "solid");
    add("border-width:", "1px");
    add("border-color:", As::Color(As::grayLightVery).name());
    add("border-radius:", "0px");

    startBlock("QLineEdit:focus");
    add("border-color:", As::Color(As::blueLight).name());

    //startBlock("QLineEdit:read-only");
    //add("color:", As::Color(As::blueDark).name());

    startBlock("QLineEdit:disabled");
    add("background-color:", As::Color(As::grayLightUltra).name());
    add("color:", As::Color(As::grayLight).name());

    //===============================
    // QPlainTextEdit, As::TextEditor
    //===============================

    startBlock("QPlainTextEdit");
    add("background-color:", As::Color(As::white).name());
    add("selection-background-color:", As::Color(As::orangeLight).name());
    add("selection-color:", As::Color(As::black).name());
    add("margin:", "0px");
    add("padding:", "0px");
    add("border-style:", "none");

    //=============
    // QProgressBar
    //=============

    startBlock("QProgressBar:horizontal");
    add("background-color:", As::Color(As::grayLightUltra).name());
    add("border-color:", As::Color(As::grayLight).name());
    add("border-style:", "none");
    //add("border-style:", "solid");
    //add("border-width:", "1px");
    //add("border-radius:", "2px");
    add("padding:", "0px");
    add("margin:", "0px");

    startBlock("QProgressBar::chunk:horizontal");
    add("background-color:", As::Color(As::blue).name());

    //============================================================
    // QProgressDialog is defined via QMessageBox and QProgressBar
    //============================================================

    //============
    // QPushButton
    //============

    startBlock("QPushButton");
    add("color:", As::Color(As::white).name());
    add("background-color:", As::Color(As::blue).name());
    add("border-color:", As::Color(As::blueDark).name());
    add("border-style:", "solid");
    add("border-width:", "1px");
    add("border-radius:", "4px");
    add("padding-left:", "14px");
    add("padding-right:", "14px");
    add("padding-top:", "3px");
    add("padding-bottom:", "3px");

    startBlock("QPushButton:pressed");
    add("background-color:", As::Color(As::blueDark).name());
    add("border-color:", As::Color(As::blueDarkVery).name());

    startBlock("QPushButton:default");
    add("background-color:", As::Color(As::blue).name());
    add("border-color:", As::Color(As::blueDark).name());

    startBlock("QPushButton:default:pressed");
    add("background-color:", As::Color(As::blueDark).name());
    add("border-color:", As::Color(As::blueDarkVery).name());

    startBlock("QPushButton:focus");
    add("background-color:", As::Color(As::blue).name());
    add("border-color:", As::Color(As::blueDark).name());
    add("outline:", "none");

    startBlock("QPushButton:focus:pressed");
    add("background-color:", As::Color(As::blueDark).name());
    add("border-color:", As::Color(As::blueDarkVery).name());

    startBlock("QPushButton:disabled");
    add("background-color:", As::Color(As::grayLight).name());
    add("border-color:", As::Color(As::gray).name());

    //startBlock("#sidebarTabsWidget QPushButton"); // for sidebarTabs only to prevent changes in QDialog

    startBlock("#sidebarTabsWidget #findNextButton");
    add("border-left-color:", As::Color(As::grayLightVery).name());
    add("border-top-left-radius:", "0px");
    add("border-bottom-left-radius:", "0px");
    add("image:", "url(:/Images/arrow_right.png)");

    startBlock("#sidebarTabsWidget #findPreviousButton");
    add("margin-right:", "-1px");
    add("border-top-right-radius:", "0px");
    add("border-bottom-right-radius:", "0px");
    add("image:", "url(:/Images/arrow_left.png)");

    //=========
    // QSpinBox
    //=========

    startBlock("QSpinBox");
    add("color:", As::Color(As::blue).name());
    add("selection-color:", As::Color(As::white).name());
    add("background-color:", As::Color(As::white).name());
    add("selection-background-color:", As::Color(As::blue).name());
    add("border-style:", "solid");
    add("border-width:", "1px");
    add("border-color:", As::Color(As::grayLightVery).name());
    add("border-radius:", "4px");
    add("padding-left:", "2px");
    add("padding-right:", "2px");

    startBlock("QSpinBox:focus");
    add("border-color:", As::Color(As::blueLight).name());

    startBlock("QSpinBox:disabled");
    add("color:", As::Color(As::gray).name());

    startBlock("QSpinBox:read-only");
    add("color:", As::Color(As::gray).name());

    startBlock("QSpinBox::up-button, QSpinBox::down-button");
    add("background-color:", "transparent");
    add("border-style:", "none");
    add("height:", "22px");
    add("width:", "24px");
    add("subcontrol-origin:", "border");

    startBlock("QSpinBox::up-button");
    add("margin-right:", "1px");
    add("subcontrol-position:", "right");
    add("border-left-style:", "solid");
    add("border-left-width:", "1px");
    add("border-left-color:", As::Color(As::grayLightVery).name());
    add("border-top-right-radius:", "4px");
    add("border-bottom-right-radius:", "4px");

    startBlock("QSpinBox::down-button");
    add("margin-left:", "1px");
    add("subcontrol-position:", "left");
    add("border-right-style:", "solid");
    add("border-right-width:", "1px");
    add("border-right-color:", As::Color(As::grayLightVery).name());
    add("border-top-left-radius:", "4px");
    add("border-bottom-left-radius:", "4px");

    startBlock("QSpinBox::up-button:pressed, QSpinBox::down-button:pressed");
    add("background-color:", As::Color(As::grayLightUltra).name());

    startBlock("QSpinBox::up-arrow");
    add("image:", "url(:/Images/TriangleArrow-Right.svg)");
    add("width:", "17px");

    startBlock("QSpinBox::down-arrow");
    add("image:", "url(:/Images/TriangleArrow-Left.svg)");
    add("width:", "17px");

    //===========
    // QStatusBar
    //===========

    startBlock("QStatusBar");
    add("color:", As::Color(As::grayDark).name());

    //===========
    // QTableView
    //===========

    startBlock("QTableView");
    add("background-color:", As::Color(As::white).name());
    add("alternate-background-color:", As::Color(As::blueLightUltra).name());
    add("selection-color:", As::Color(As::white).name());
    add("selection-background-color:", As::Color(As::blue).name());
    add("gridline-color:", As::Color(As::grayLightVery).name());
    add("border-style:", "none");
    add("margin:", "0px");
    add("padding:", "0px");

    startBlock("QTableView QHeaderView");
    add("background-color:", As::Color(As::white).name());

    startBlock("QTableView QTableCornerButton::section");
    add("background-color:", As::Color(As::grayLightUltra).name());
    add("border-style:", "none");
    add("border-bottom-style:", "solid");
    add("border-bottom-width:", "1px");
    add("border-bottom-color:", As::Color(As::grayLightVery).name());
    add("border-right-style:", "solid");
    add("border-right-width:", "1px");
    add("border-right-color:", As::Color(As::grayLightVery).name());
    add("image:", "url(:/Images/table_corner-button.png)");

    startBlock("QTableView QHeaderView::section");
    add("color:", As::Color(As::grayDark).name());
    add("background-color:", As::Color(As::grayLightUltra).name());
    add("border-style:", "none");
    add("border-bottom-style:", "solid");
    add("border-bottom-width:", "1px");
    add("border-bottom-color:", As::Color(As::grayLightVery).name());
    add("border-right-style:", "solid");
    add("border-right-width:", "1px");
    add("border-right-color:", As::Color(As::grayLightVery).name());
    add("padding-left:", "9px");
    add("padding-right:", "9px");

    //===========
    // QTabWidget
    //===========

    //QSettings().setValue("Sidebar/width", APP_SIDEBAR_WIDTH);
    //const auto APP_SIDEBAR_WIDTH = QSettings().value("Sidebar/width").toInt();
    //const auto TAB_WIDTH = QString::number(0.5 * APP_SIDEBAR_WIDTH - 1) + "px";

    startBlock("QTabWidget::pane");
    add("background-color:", As::Color(As::grayLightUltra).name());
    add("border-top: 1px solid", As::Color(As::grayLightVery).name());

    startBlock("QTabWidget::tab-bar");
    add("alignment:", "left");

    startBlock("QTabBar::tab");
    add("min-width:", "149px");
    add("max-width:", "149px");
    add("min-height:", "26px");
    add("max-height:", "26px");
    add("margin-left:", "-1px");
    add("border-style:", "none");

    startBlock("QTabBar::tab:selected");
    add("color:", As::Color(As::white).name());
    add("background-color:", As::Color(As::blue).name());
    add("border-left: 1px solid", As::Color(As::blue).name());
    add("border-right: 1px solid", As::Color(As::blue).name());  //add("border-top-style:", "none");

    startBlock("QTabBar::tab:!selected");
    add("color:", As::Color(As::grayDarkVery).name());
    add("background-color:", As::Color(As::grayLightUltra).name());
    add("border-left: 1px solid", As::Color(As::grayLightVery).name());
    add("border-right: 1px solid", As::Color(As::grayLightVery).name());

    startBlock("#sidebarTabsWidget QTabBar::tab"); /// How else?!
    add("border-style:", "none");
    add("border-left: 1px solid", As::Color(As::grayLightVery).name());
    add("margin-left:", "0px");

    //=========
    // QToolBar
    //=========

    startBlock("QToolBar QLabel");
    add("color:", As::Color(As::grayDarkUltra).name());

    startBlock("QToolBar QLabel:disabled");
    add("color:", As::Color(As::gray).name());

    startBlock("QToolBar QToolButton");
    add("background-color:", As::Color(As::white).name());
    add("border: 1px solid", As::Color(As::grayLightVery).name());
    add("border-radius:", "4px");

    startBlock("QToolBar QToolButton:pressed");
    add("background-color:", As::Color(As::grayLightVery).name());

    startBlock("QToolBar QToolButton:disabled");
    add("background-color:", As::Color(As::grayLightUltra).name());

    //=========================
    // QWidget (via objectName)
    //=========================

    startBlock("#lineNumberArea");
    add("border-right: 1px solid", As::Color(As::grayLightVery).name());
    add("color:", As::Color(As::grayLight).name());
    add("background-color:", As::Color(As::grayLightUltra).name());

    startBlock("#sidebarTabsWidget::pane");
    add("border-left: 1px solid", As::Color(As::grayLightVery).name());
    add("color:", As::Color(As::blue).name());

    startBlock("#gotoScanCount");
    add("color:", As::Color(As::blue).name());

    startBlock("#gotoFileCount");
    add("color:", As::Color(As::blue).name());

    startBlock("#gotoLineCount");
    add("color:", As::Color(As::blue).name());

    startBlock("#gotoMatchCount");
    add("color:", As::Color(As::blue).name());

    startBlock("#nonSkipPointsCount");
    add("color:", As::Color(As::blue).name());

    startBlock("#nonSkipPointsCount:disabled");
    add("color:", As::Color(As::gray).name());

    startBlock("#peakPointsCount");
    add("color:", As::Color(As::blue).name());

    startBlock("#peakPointsCount:disabled");
    add("color:", As::Color(As::gray).name());

    //=====================
    // Close the last block
    //=====================

    closeBlock(); }
