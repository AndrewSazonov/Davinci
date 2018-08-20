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
    Creates actions, menus and tool bar for the application user interface.
*/
void As::Window::createActionsMenusToolBar() {
    ADEBUG;

    //===================================
    // Actions, menus and toolbar buttons
    //===================================

    //---------
    // Add menu
    //---------
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    // Add action
    QAction* openFileAct = fileMenu->addAction(tr("&Open File(s)..."),
                                               this, &As::Window::openFileSlot);
    openFileAct->setToolTip(tr("Open existing file(s)."));
    openFileAct->setShortcuts(QKeySequence::Open);
    openFileAct->setIcon(QIcon(":/Images/Button_Open-File.svg"));

    auto openFileButton = new As::UnderLabeledWidget(new As::ToolBarButton(openFileAct),
                                                     tr("Open file"));

    // Add action
    QAction* openDirAct = fileMenu->addAction(tr("Open &Directory..."),
                                              this, &As::Window::openDirSlot);
    openDirAct->setToolTip(tr("Open existing directory."));
    openDirAct->setIcon(QIcon(":/Images/Button_Open-Folder.svg"));

    auto openDirButton = new As::UnderLabeledWidget(new As::ToolBarButton(openDirAct),
                                                    tr("Open dir"));

    // Add separator --------
    fileMenu->addSeparator();

    // Add action
    QAction* saveAct = fileMenu->addAction(tr("&Save"),
                                           this, &As::Window::aboutAppSlot);
    saveAct->setToolTip(tr("Save the document."));
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setEnabled(false);

    // Add action
    QAction* saveAsAct = fileMenu->addAction(tr("Save &As..."), this, &As::Window::aboutAppSlot);
    saveAsAct->setToolTip(tr("Save the document with a new name."));
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setEnabled(false);

    // Add action
    QAction* exportAct = fileMenu->addAction(tr("&Export..."), this, &As::Window::exportSlot);
    exportAct->setToolTip(tr("Export the document."));
    exportAct->setIcon(QIcon(":/Images/Button_Export.svg"));
    exportAct->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, exportAct, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, exportAct, &QAction::setEnabled);

    auto exportButton = new As::UnderLabeledWidget(new As::ToolBarButton(exportAct), tr("Export"));
    exportButton->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, exportButton, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, exportButton, &As::UnderLabeledWidget::setEnabled);

    // Add separator --------
    fileMenu->addSeparator();

    // Add action
    QAction* reloadAct = fileMenu->addAction(tr("&Reload"), this, &As::Window::reloadFileSlot);
    reloadAct->setToolTip(tr("Reload opened file(s)."));
    reloadAct->setIcon(QIcon(":/Images/Button_Reload.svg"));
    reloadAct->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, reloadAct, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, reloadAct, &QAction::setEnabled);

    auto reloadButton = new As::UnderLabeledWidget(new As::ToolBarButton(reloadAct), tr("Reload"));
    reloadButton->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, reloadButton, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, reloadButton, &As::UnderLabeledWidget::setEnabled);

    // Add action
    QAction* closeAct = fileMenu->addAction(tr("&Close File(s)"), this, &As::Window::closeFileSlot);
    closeAct->setToolTip(tr("Close the open files."));
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, closeAct, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, closeAct, &QAction::setEnabled);

    // Add action
    QAction* exitAppAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAppAct->setToolTip(tr("Exit the application."));
    exitAppAct->setShortcuts(QKeySequence::Quit);
    exitAppAct->setMenuRole(QAction::QuitRole); // not required due to the TextHeuristicRole

    //---------
    // Add menu
    //---------
    QMenu* processMenu = menuBar()->addMenu(tr("&Processing"));

    // Add action
    QAction* autoProcessingAct = processMenu->addAction(tr("&Start auto processing"), this, &As::Window::autoProcessingSlot);
    autoProcessingAct->setToolTip(tr("Start data processing in auto mode."));
    autoProcessingAct->setIcon(QIcon(":/Images/Button_Auto.svg"));
    autoProcessingAct->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, autoProcessingAct, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, autoProcessingAct, &QAction::setEnabled);

    auto autoProcessingButton = new As::UnderLabeledWidget(new As::ToolBarButton(autoProcessingAct), tr("Process"));
    autoProcessingButton->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, autoProcessingButton, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, autoProcessingButton, &As::UnderLabeledWidget::setEnabled);

    //---------
    // Add menu
    //---------
    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

    // Create icon with 2 states
    QIcon sidebarIcon;
    sidebarIcon.addFile(":/Images/Button_Sidebar_On.svg", QSize(), QIcon::Normal, QIcon::On);
    sidebarIcon.addFile(":/Images/Button_Sidebar_Off.svg", QSize(), QIcon::Normal, QIcon::Off);

    // Add action
    QAction* showOrHideSidebarAct = viewMenu->addAction(tr("&Show Side Bar"));
    showOrHideSidebarAct->setToolTip(tr("Show or hide the sidebar with options for manual data processing."));
    showOrHideSidebarAct->setIcon(sidebarIcon);
    showOrHideSidebarAct->setCheckable(true);
    showOrHideSidebarAct->setChecked(false);
    showOrHideSidebarAct->setEnabled(false);
    connect(showOrHideSidebarAct, &QAction::toggled, this, &As::Window::showSidebarSlot);
    connect(this, &As::Window::newFilesLoadedSignal, showOrHideSidebarAct, &QAction::setChecked);
    connect(this, &As::Window::oldFilesClosedSignal, showOrHideSidebarAct, &QAction::setChecked);
    connect(this, &As::Window::newFilesLoadedSignal, showOrHideSidebarAct, &QAction::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, showOrHideSidebarAct, &QAction::setEnabled);

    auto sidebarButton = new As::UnderLabeledWidget(new As::ToolBarButton(showOrHideSidebarAct), tr("Sidebar"));
    sidebarButton->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, sidebarButton, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, sidebarButton, &As::UnderLabeledWidget::setEnabled);

    //---------
    // Add menu
    //---------
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

    // Add action
    QAction* aboutAppAct = helpMenu->addAction(tr("&About"), this, &As::Window::aboutAppSlot);
    aboutAppAct->setToolTip(tr("Show the application's About box."));
    aboutAppAct->setMenuRole(QAction::AboutRole); // not required due to the TextHeuristicRole

    // Add action
    //QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    //aboutQtAct->setToolTip(tr("Show the Qt library's About box."));
    //aboutQtAct->setMenuRole(QAction::AboutQtRole); // not required due to the TextHeuristicRole

    // Add action
    QAction* checkUpdatesAct = helpMenu->addAction(tr("Check for Updates..."), this, &As::Window::checkApplicationUpdateNow);
    checkUpdatesAct->setToolTip(tr("Check for the application's updates now."));
    checkUpdatesAct->setMenuRole(QAction::ApplicationSpecificRole);

    // Add action
    QAction* preferencesAct = fileMenu->addAction(tr("&Preferences..."), this, &As::Window::showPreferencesSlot);
    preferencesAct->setToolTip(tr("Preferences of application."));
    preferencesAct->setShortcuts(QKeySequence::Preferences);
    preferencesAct->setMenuRole(QAction::PreferencesRole); // not required due to the TextHeuristicRole

    // Add action
    QAction* userManualAct = helpMenu->addAction(tr("&User manual (online)"), this, &As::Window::openUserManualSlot);
    userManualAct->setToolTip(tr("Show online user manual in web browser."));

    // Add action
    QAction* issueAct = helpMenu->addAction(tr("&Report issue (online)"), this, &As::Window::openIssueTrackerSlot);
    issueAct->setToolTip(tr("Report a bug on the online issue tracker."));

    //===========================
    // Toolbar additional widgets
    //===========================

    const int height = 28;
    const int width = 100;

    auto facilityType = new As::LineEdit;
    facilityType->setToolTip(tr("Neutron facility."));
    facilityType->setMinimumHeight(height);
    facilityType->setMinimumWidth(width);
    facilityType->setReadOnly(true);
    facilityType->setPlaceholderText(tr("Undefined"));
    facilityType->setAlignment(Qt::AlignHCenter);
    facilityType->setFocusPolicy(Qt::NoFocus);
    connect(this, &As::Window::facilityTypeChanged, facilityType, &As::LineEdit::setText);
    connect(this, &As::Window::oldFilesClosedSignal, facilityType, &As::LineEdit::clear);

    auto facilityTypeWidget = new As::UnderLabeledWidget(facilityType, tr("Facility"));
    facilityTypeWidget->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, facilityTypeWidget, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, facilityTypeWidget, &As::UnderLabeledWidget::setEnabled);

    auto instrumentType = new As::LineEdit;
    instrumentType->setToolTip(tr("Instrument name."));
    instrumentType->setMinimumHeight(height);
    instrumentType->setMinimumWidth(width);
    instrumentType->setReadOnly(true);
    instrumentType->setPlaceholderText(tr("Undefined"));
    instrumentType->setAlignment(Qt::AlignHCenter);
    instrumentType->setFocusPolicy(Qt::NoFocus);
    connect(this, &As::Window::instrumentTypeChanged, instrumentType, &As::LineEdit::setText);
    connect(this, &As::Window::oldFilesClosedSignal, instrumentType, &As::LineEdit::clear);

    auto instrumentTypeWidget = new As::UnderLabeledWidget(instrumentType, tr("Instrument"));
    instrumentTypeWidget->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, instrumentTypeWidget, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, instrumentTypeWidget, &As::UnderLabeledWidget::setEnabled);

    auto dataType = new As::LineEdit;
    dataType->setToolTip(tr("Type of input data."));
    dataType->setMinimumHeight(height);
    dataType->setMinimumWidth(width);
    dataType->setReadOnly(true);
    dataType->setPlaceholderText(tr("Undefined"));
    dataType->setAlignment(Qt::AlignHCenter);
    dataType->setFocusPolicy(Qt::NoFocus);
    connect(this, &As::Window::dataTypeChanged, dataType, &As::LineEdit::setText);
    connect(this, &As::Window::oldFilesClosedSignal, dataType, &As::LineEdit::clear);

    auto dataTypeWidget = new As::UnderLabeledWidget(dataType, tr("Data type"));
    dataTypeWidget->setEnabled(false);
    connect(this, &As::Window::newFilesLoadedSignal, dataTypeWidget, &As::UnderLabeledWidget::setEnabled);
    connect(this, &As::Window::oldFilesClosedSignal, dataTypeWidget, &As::UnderLabeledWidget::setEnabled);

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
