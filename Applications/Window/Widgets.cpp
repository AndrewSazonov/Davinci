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

#include "ScanDatabase.hpp"

#include "Window.hpp"

/*!
...
*/
QWidget *As::Window::createDragAndDropWidget()
{
    ADEBUG;

    auto background = new QSvgWidget(":/Images/Bkg_Drag-and-Drop.svg");
    background->setFixedSize(background->sizeHint());

    auto layout = new QHBoxLayout;
    layout->addItem(new QSpacerItem(0, 0));
    layout->addWidget(background);
    layout->addItem(new QSpacerItem(0, 0));

    auto widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

/*!
Central ...
*/
QWidget *As::Window::createMainWidget()
{
    ADEBUG;

    // This is the main widget wraped into the QPointer, so no need to manually
    // set its pointer to Q_NULLPTR after deleting. Moreover, deleating of just
    // this widget deletes all the other child widgets added via layout

    auto layout = new As::HBoxLayout;
    layout->addWidget(createDataWidget());
    layout->addWidget(createSidebarWidget());


    ///connect(this, &As::Window::newFilesLoaded_Signal, this, &As::Window::createTabsWidget);

    showOrHideSidebarBlocks_Slot(0);

    /*
    if (m_mainWidget) {
        ADEBUG  << "1111111111111111111111111111111111111111111111111" << m_mainWidget;
        delete m_mainWidget;
        m_mainWidget = Q_NULLPTR; }
        */

    //auto m_mainWidget = new QWidget;
    //m_mainWidget->setObjectName("mainWidget");
    auto mainWidget = new QWidget;
    //ADEBUG  << "22222222222222222222222222222222222222222222222222" << m_mainWidget;
    //m_cleaner->add(m_mainWidget);
    mainWidget->setLayout(layout);

    return mainWidget;
}

/*!
Left ...
*/
QWidget *As::Window::createDataWidget()
{
    ADEBUG;

    auto layout = new As::HBoxLayout;
    layout->addWidget(createTabsWidget());

    auto widget = new QWidget;
    //widget->setObjectName("dataWidget");
    widget->setLayout(layout);

    //dataWidget->hide();
    //connect(this, &As::Window::newFilesLoaded_Signal, dataWidget, &QWidget::show);
    //connect(this, &As::Window::oldFilesClosed_Signal, dataWidget, &QWidget::hide);

    return widget;
}

/*!
Left widget -> Main tabs widget
*/
QTabWidget *As::Window::createTabsWidget()
{
    ADEBUG;

    //if (m_tabsWidget)
    //    delete m_tabsWidget;

    // Tabs
    m_tabsWidget = new QTabWidget;
    m_tabsWidget->setObjectName("mainTabs");
    m_tabsWidget->setTabsClosable(false);

    m_tabsWidget->addTab(createInputTextWidget(), "Input Text");

    /*
    // Create single tab by default: inputTextWidget
    m_inputTextWidget = new As::TextEditor;
    m_inputTextWidget->setObjectName("inputTextWidget");

//    connect(m_inputTextWidget, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged_Slot()));
//    connect(m_inputTextWidget, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine_Slot()));
    m_tabsWidget->addTab(m_inputTextWidget, "Input Text");
    */
    // Autohide tab-bar if only one tab exists
////    m_tabsWidget->setTabBarAutoHide(true);
    //// update currentTab if current tab is changed
    ////connect(m_tabsWidget, SIGNAL(currentChanged(int)), this, SLOT(m_tabsWidgetCurrentChanged(int)));
    // Show or hide sidebar group boxes depends on the selected tab of m_tabsWidget
//    connect(m_tabsWidget, SIGNAL(currentChanged(int)), this, SLOT(showHideSidebarGroups_Slot()));
    connect(m_tabsWidget, &QTabWidget::currentChanged, this, &As::Window::showOrHideSidebarBlocks_Slot); // Error in Linux
    //connect(m_tabsWidget, SIGNAL(currentChanged(int)), this, SLOT(showOrHideSidebarBlocks_Slot(int)));

    // Return
    return m_tabsWidget;
}

/*!
...
*/
As::TextEditor *As::Window::createInputTextWidget()
{
    ADEBUG;


    m_inputTextWidget = new As::TextEditor;

    //ADEBUG << m_inputTextWidget;
    //ADEBUG << m_copyTextAct;
    //ADEBUG << m_copyText_Act->isEnabled();
    //m_copyText_Act->setEnabled(true);

    //connect(this, &As::Window::currentFileIndexChanged_Signal, m_inputTextWidget, &As::TextEditor::clearAllSelections);
////    connect(this, &As::Window::currentFileContentChanged_Signal, m_inputTextWidget, &As::TextEditor::clearAllSelections);
    connect(this, &As::Window::currentFileContentChanged_Signal, m_inputTextWidget, &As::TextEditor::setPlainText); // Error in Linux
    //connect(this, SIGNAL(currentFileContentChanged_Signal(QString)), m_inputTextWidget, SLOT(setPlainText(QString)));

    //connect(m_copyText_Act, &QAction::triggered, m_inputTextWidget, &As::TextEditor::copy);
    //connect(m_inputTextWidget, &As::TextEditor::copyAvailable, m_copyText_Act, &QAction::setEnabled);
    //m_copyText_Act->setEnabled(true);

//    m_inputTextWidget->setObjectName("inputTextWidget");

//    connect(m_inputTextWidget, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged_Slot()));
//    connect(m_inputTextWidget, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine_Slot()));
    //m_tabsWidget->addTab(m_inputTextWidget, "Input Text");

    //connect(m_inputTextWidget, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged_Slot()));
///    connect(m_inputTextWidget, &As::TextEditor::cursorPositionChanged, this, &As::Window::highlightCurrentLine_Slot);

//    connect(m_inputTextWidget, &As::TextEditor::cursorPositionChanged, this, &As::Window::highlightCurrentLine_Slot);
//    connect(this, &As::Window::currentLineChanged_Signal, m_inputTextWidget, TE&As::TextEditor::setCursorPosition);
    //connect(this, SIGNAL(currentScanChanged_Signal(int)), m_inputTextWidget, SLOT(setCursorPosition(int)));
    ///connect(this, &As::Window::currentScanChanged_Signal, m_inputTextWidget, &As::TextEditor::setCursorPosition);

    return m_inputTextWidget;
}

