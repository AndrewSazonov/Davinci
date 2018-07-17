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
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QSize>
#include <QToolBar>
#include <QWidget>

#include "Macros.hpp"

#include "LineEdit.hpp"
#include "ToolBarButton.hpp"
#include "ToolBarSpacer.hpp"
#include "UnderLabeledWidget.hpp"

#include "Window.hpp"

/*!
Creates actions, menus and tool bar
*/
void As::Window::createActionsMenusToolBar()
{
    ADEBUG;

    //===================================
    // Actions, menus and toolbar buttons
    //===================================

    //---------
    // Add menu
    //---------
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    // Add action
    QAction *openFile_Act = fileMenu->addAction(tr("&Open File(s)..."), this, &As::Window::openFile_Slot);
    openFile_Act->setToolTip(tr("Open existing file(s)."));
    openFile_Act->setShortcuts(QKeySequence::Open);
    openFile_Act->setIcon(QIcon(":/Images/Button_Open-File.svg"));

    auto openFileButton = new As::UnderLabeledWidget(new As::ToolBarButton(openFile_Act), tr("Open file"));

    // Add action
    QAction *openDir_Act = fileMenu->addAction(tr("Open &Directory..."), this, &As::Window::openDir_Slot);
    openDir_Act->setToolTip(tr("Open existing directory."));
    openDir_Act->setIcon(QIcon(":/Images/Button_Open-Folder.svg"));

    auto openDirButton = new As::UnderLabeledWidget(new As::ToolBarButton(openDir_Act), tr("Open dir"));

    // Add separator --------
    fileMenu->addSeparator();

    // Add action
    QAction *save_Act = fileMenu->addAction(tr("&Save"), this, &As::Window::aboutApp_Slot);
    save_Act->setToolTip(tr("Save the document."));
    save_Act->setShortcuts(QKeySequence::Save);
    save_Act->setEnabled(false);

    // Add action
    QAction *saveAs_Act = fileMenu->addAction(tr("Save &As..."), this, &As::Window::aboutApp_Slot);
    saveAs_Act->setToolTip(tr("Save the document with a new name."));
    saveAs_Act->setShortcuts(QKeySequence::SaveAs);
    saveAs_Act->setEnabled(false);

    // Add action
    QAction *export_Act = fileMenu->addAction(tr("&Export..."), this, &As::Window::export_Slot);
    export_Act->setToolTip(tr("Export the document."));
    export_Act->setIcon(QIcon(":/Images/Button_Export.svg"));
    export_Act->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, export_Act, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, export_Act, &QAction::setEnabled);

    auto exportButton = new As::UnderLabeledWidget(new As::ToolBarButton(export_Act), tr("Export"));
    exportButton->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, exportButton, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, exportButton, &As::UnderLabeledWidget::setEnabled);

    // Add separator --------
    fileMenu->addSeparator();

    // Add action
    QAction *reload_Act = fileMenu->addAction(tr("&Reload"), this, &As::Window::reloadFile_Slot);
    reload_Act->setToolTip(tr("Reload opened file(s)."));
    reload_Act->setIcon(QIcon(":/Images/Button_Reload.svg"));
    reload_Act->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, reload_Act, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, reload_Act, &QAction::setEnabled);

    auto reloadButton = new As::UnderLabeledWidget(new As::ToolBarButton(reload_Act), tr("Reload"));
    reloadButton->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, reloadButton, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, reloadButton, &As::UnderLabeledWidget::setEnabled);

    // Add action
    QAction *close_Act = fileMenu->addAction(tr("&Close File(s)"), this, &As::Window::closeFile_Slot);
    close_Act->setToolTip(tr("Close the open files."));
    close_Act->setShortcuts(QKeySequence::Close);
    close_Act->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, close_Act, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, close_Act, &QAction::setEnabled);

    // Add action
    QAction *exitApp_Act = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitApp_Act->setToolTip(tr("Exit the application."));
    exitApp_Act->setShortcuts(QKeySequence::Quit);
    exitApp_Act->setMenuRole(QAction::QuitRole); // not required due to the TextHeuristicRole

    //---------
    // Add menu
    //---------
    QMenu *processMenu = menuBar()->addMenu(tr("&Processing"));

    // Add action
    QAction *autoProcessing_Act = processMenu->addAction(tr("&Start auto processing"), this, &As::Window::autoProcessing_Slot);
    autoProcessing_Act->setToolTip(tr("Start data processing in auto mode."));
    autoProcessing_Act->setIcon(QIcon(":/Images/Button_Auto.svg"));
    autoProcessing_Act->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, autoProcessing_Act, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, autoProcessing_Act, &QAction::setEnabled);

    auto autoProcessingButton = new As::UnderLabeledWidget(new As::ToolBarButton(autoProcessing_Act), tr("Process"));
    autoProcessingButton->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, autoProcessingButton, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, autoProcessingButton, &As::UnderLabeledWidget::setEnabled);

    //---------
    // Add menu
    //---------
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    // Create icon with 2 states
    QIcon sidebarIcon;
    sidebarIcon.addFile(":/Images/Button_Sidebar_On.svg", QSize(), QIcon::Normal, QIcon::On);
    sidebarIcon.addFile(":/Images/Button_Sidebar_Off.svg", QSize(), QIcon::Normal, QIcon::Off);

    // Add action
    QAction *showOrHideSidebar_Act = viewMenu->addAction(tr("&Show Side Bar"));
    showOrHideSidebar_Act->setToolTip(tr("Show or hide the sidebar with options for manual data processing."));
    showOrHideSidebar_Act->setIcon(sidebarIcon);
    showOrHideSidebar_Act->setCheckable(true);
    showOrHideSidebar_Act->setChecked(false);
    showOrHideSidebar_Act->setEnabled(false);
    connect(showOrHideSidebar_Act, &QAction::toggled, this, &As::Window::showSidebar_Slot);
    connect(this, &As::Window::newFilesLoaded_Signal, showOrHideSidebar_Act, &QAction::setChecked);
    connect(this, &As::Window::oldFilesClosed_Signal, showOrHideSidebar_Act, &QAction::setChecked);
    connect(this, &As::Window::newFilesLoaded_Signal, showOrHideSidebar_Act, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, showOrHideSidebar_Act, &QAction::setEnabled);

    auto sidebarButton = new As::UnderLabeledWidget(new As::ToolBarButton(showOrHideSidebar_Act), tr("Sidebar"));
    sidebarButton->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, sidebarButton, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, sidebarButton, &As::UnderLabeledWidget::setEnabled);

    //---------
    // Add menu
    //---------
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    // Add action
    QAction *aboutApp_Act = helpMenu->addAction(tr("&About"), this, &As::Window::aboutApp_Slot);
    aboutApp_Act->setToolTip(tr("Show the application's About box."));
    aboutApp_Act->setMenuRole(QAction::AboutRole); // not required due to the TextHeuristicRole

    // Add action
    //QAction *aboutQt_Act = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    //aboutQt_Act->setToolTip(tr("Show the Qt library's About box."));
    //aboutQt_Act->setMenuRole(QAction::AboutQtRole); // not required due to the TextHeuristicRole

    // Add action
    QAction *checkUpdates_Act = helpMenu->addAction(tr("Check for Updates..."), this, &As::Window::checkApplicationUpdateNow_Slot);
    checkUpdates_Act->setToolTip(tr("Check for the application's updates now."));
    checkUpdates_Act->setMenuRole(QAction::ApplicationSpecificRole);

    // Add action
    QAction *preferences_Act = fileMenu->addAction(tr("&Preferences..."), this, &As::Window::showPreferences_Slot);
    preferences_Act->setToolTip(tr("Preferences of application."));
    preferences_Act->setShortcuts(QKeySequence::Preferences);
    preferences_Act->setMenuRole(QAction::PreferencesRole); // not required due to the TextHeuristicRole

    // Add action
    QAction *userManual_Act = helpMenu->addAction(tr("&User manual (online)"), this, &As::Window::openUserManual_Slot);
    userManual_Act->setToolTip(tr("Show online user manual in web browser."));

    // Add action
    QAction *issue_Act = helpMenu->addAction(tr("&Report issue (online)"), this, &As::Window::openIssueTracker_Slot);
    issue_Act->setToolTip(tr("Report a bug on the online issue tracker."));

    //===========================
    // Toolbar additional widgets
    //===========================

    const int width = 100;

    auto facilityType = new As::LineEdit;
    facilityType->setToolTip(tr("Neutron facility."));
    facilityType->setMinimumWidth(width);
    facilityType->setReadOnly(true);
    facilityType->setPlaceholderText(tr("Undefined"));
    facilityType->setAlignment(Qt::AlignHCenter);
    facilityType->setFocusPolicy(Qt::NoFocus);
    connect(this, &As::Window::facilityTypeChanged_Signal, facilityType, &As::LineEdit::setText);
    connect(this, &As::Window::oldFilesClosed_Signal, facilityType, &As::LineEdit::clear);

    auto facilityTypeWidget = new As::UnderLabeledWidget(facilityType, tr("Facility"));
    facilityTypeWidget->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, facilityTypeWidget, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, facilityTypeWidget, &As::UnderLabeledWidget::setEnabled);

    auto instrumentType = new As::LineEdit;
    instrumentType->setToolTip(tr("Instrument name."));
    instrumentType->setMinimumWidth(width);
    instrumentType->setReadOnly(true);
    instrumentType->setPlaceholderText(tr("Undefined"));
    instrumentType->setAlignment(Qt::AlignHCenter);
    instrumentType->setFocusPolicy(Qt::NoFocus);
    connect(this, &As::Window::instrumentTypeChanged_Signal, instrumentType, &As::LineEdit::setText);
    connect(this, &As::Window::oldFilesClosed_Signal, instrumentType, &As::LineEdit::clear);

    auto instrumentTypeWidget = new As::UnderLabeledWidget(instrumentType, tr("Instrument"));
    instrumentTypeWidget->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, instrumentTypeWidget, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, instrumentTypeWidget, &As::UnderLabeledWidget::setEnabled);

    auto dataType = new As::LineEdit;
    dataType->setToolTip(tr("Type of input data."));
    dataType->setMinimumWidth(width);
    dataType->setReadOnly(true);
    dataType->setPlaceholderText(tr("Undefined"));
    dataType->setAlignment(Qt::AlignHCenter);
    dataType->setFocusPolicy(Qt::NoFocus);
    connect(this, &As::Window::dataTypeChanged_Signal, dataType, &As::LineEdit::setText);
    connect(this, &As::Window::oldFilesClosed_Signal, dataType, &As::LineEdit::clear);

    auto dataTypeWidget = new As::UnderLabeledWidget(dataType, tr("Data type"));
    dataTypeWidget->setEnabled(false);
    connect(this, &As::Window::newFilesLoaded_Signal, dataTypeWidget, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosed_Signal, dataTypeWidget, &As::UnderLabeledWidget::setEnabled);

    //============================
    // Toolbar layout and settings
    //============================

    auto layout = new QHBoxLayout;
    layout->addWidget(openFileButton);
    layout->addWidget(openDirButton);
    layout->addWidget(reloadButton);
    layout->addWidget(new As::ToolBarSpacer);
    layout->addWidget(facilityTypeWidget);
    layout->addWidget(instrumentTypeWidget);
    layout->addWidget(dataTypeWidget);
    layout->addWidget(new As::ToolBarSpacer);
    layout->addWidget(autoProcessingButton);
    layout->addWidget(sidebarButton);
    layout->addWidget(exportButton);
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(9);

    auto toolBarInner = new QWidget;
    toolBarInner->setLayout(layout);

    auto toolBar = new QToolBar(this);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->addWidget(toolBarInner);

    addToolBar(Qt::TopToolBarArea, toolBar);

#ifdef Q_OS_OSX
    setUnifiedTitleAndToolBarOnMac(true);
#endif
}
