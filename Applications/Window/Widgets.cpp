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
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QObjectCleanupHandler>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QSvgWidget>
#include <QToolBar>
#include <QTimer>
#include <QUrl>

#include "Colors.hpp"
#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "ComboBox.hpp" //???
#include "HBoxLayout.hpp"
#include "LineEdit.hpp"
#include "TextEditor.hpp"
#include "Style.hpp"
#include "TableView.hpp"
#include "ToolBarButton.hpp"
#include "ToolBarSpacer.hpp"
#include "UnderLabeledWidget.hpp"
#include "Widget.hpp"

#include "ScanDict.hpp"

#include "Window.hpp"

/*!
    Returns Drag & Drop widget. That's a 1st widget opened after the program start or
    after the opened files are closed. When the new files are opened it is replaced by
    the main widget (== data widget + sidebar).
*/
QWidget* As::Window::createDragAndDropWidget() {
    ADEBUG;

    auto background = new QSvgWidget(":/Images/Bkg_Drag-and-Drop.svg");
    background->setFixedSize(background->sizeHint());

    auto layout = new QHBoxLayout;
    layout->addItem(new QSpacerItem(0, 0));
    layout->addWidget(background);
    layout->addItem(new QSpacerItem(0, 0));

    auto widget = new QWidget;
    widget->setLayout(layout);

    return widget; }

/*!
    Returns the main widget, which contains both data widget and sidebar. This widget replaces
    the drag and drop widget and is opened after the new files are opened.
*/
QWidget* As::Window::createMainWidget() {
    ADEBUG;

    auto layout = new As::HBoxLayout;
    layout->setObjectName("mainWidget layout");
    layout->addWidget(createDataWidget());
    layout->addWidget(createSidebarWidget());
    showOrHideSidebarBlocks_Slot(0);

    auto mainWidget = new As::Widget;
    mainWidget->setObjectName("mainWidget");
    mainWidget->setLayout(layout);

    return mainWidget; }

/*!
    Returns the data widget (part of main widget).
*/
QWidget* As::Window::createDataWidget() {
    ADEBUG;

    auto layout = new As::HBoxLayout;
    layout->setObjectName("dataWidget layout");
    layout->addWidget(createTabsWidget());

    auto widget = new As::Widget;
    widget->setObjectName("dataWidget");
    widget->setLayout(layout);

    return widget; }

/*!
    Returns main tabs widget of the data widget.
*/
QTabWidget* As::Window::createTabsWidget() {
    ADEBUG;

    m_tabsWidget = new QTabWidget;
    m_tabsWidget->setObjectName("mainTabs");
    m_tabsWidget->setTabsClosable(false);

    // Create single tab by default: inputTextWidget
    m_tabsWidget->addTab(createInputTextWidget(), "Input Text");

    // Show or hide sidebar group boxes depends on the selected tab of m_tabsWidget
    connect(m_tabsWidget, &QTabWidget::currentChanged, this, &As::Window::showOrHideSidebarBlocks_Slot);

    return m_tabsWidget; }

/*!
    Returns input text widget of the main tabs.
*/
As::TextEditor* As::Window::createInputTextWidget() {
    ADEBUG;

    m_inputTextWidget = new As::TextEditor;
    m_inputTextWidget->setObjectName("inputTextWidget");

    connect(this, &As::Window::currentFileContentChanged_Signal, m_inputTextWidget, &As::TextEditor::setPlainText);

    return m_inputTextWidget; }

