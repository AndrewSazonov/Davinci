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
#include <QDebug>
#include <QFontComboBox>
#include <QListView>
#include <QSettings>
#include <QStandardItemModel>
#include <QTabWidget>
#include <QTimer>

#include "Colors.hpp"
//#include "Constants.hpp"
#include "Macros.hpp"

#include "ComboBox.hpp"
#include "FontComboBox.hpp"
#include "CheckBox.hpp"
#include "GroupBox.hpp"
#include "HBoxLayout.hpp"
#include "Label.hpp"
#include "LabelQuatroBlock.hpp"
#include "LabelTripleBlock.hpp"
#include "LineEdit.hpp"
#include "PushButton.hpp"
#include "Scan.hpp"
#include "ScanDatabase.hpp"
#include "SpinBox.hpp"
#include "SpinBoxSingleBlock.hpp"
#include "SpinBoxDoubleBlock.hpp"
#include "TableView.hpp"
#include "TextEditor.hpp"
#include "VBoxLayout.hpp"

#include "Window.hpp"

// https://doc-snapshots.qt.io/qtcreator-extending/core-sidebar.html#SideBar
// https://doc-snapshots.qt.io/qtcreator-extending/core-sidebar-members.html

/*!
Returns the widget 'Sidebar'.
*/
QWidget *As::Window::createSidebarWidget()
{
    ADEBUG;

    //QSettings settings;
    const int SIDEBAR_WIDTH = QSettings().value("Sidebar/width").toInt();
    //QSettings().setValue("Sidebar/width",  SIDEBAR_WIDTH);

    auto layout = new As::HBoxLayout;
    layout->addWidget(createSidebarTabsWidget());

    m_sidebarWidget = new QWidget;
    m_sidebarWidget->setLayout(layout);
    m_sidebarWidget->setFixedWidth(SIDEBAR_WIDTH);
    m_sidebarWidget->setVisible(false);

    return m_sidebarWidget;
}

/*!
Returns the widget 'Sidebar - Tabs'.
*/
QTabWidget *As::Window::createSidebarTabsWidget()
{
    ADEBUG;

    auto widget = new QTabWidget;
    widget->setTabsClosable(false);
    widget->setObjectName("sidebarTabsWidget"); // for Style.cpp
    widget->addTab(createSidebarTabsControlsWidget(), tr("Controls"));
    widget->addTab(createSidebarTabsSettingsWidget(), tr("Settings"));

    return widget;
}

/*!
Returns the widget 'Sidebar - Tabs - Controls'.
*/
QWidget *As::Window::createSidebarTabsControlsWidget()
{
    ADEBUG;

    auto commonLayout = new As::VBoxLayout;
    commonLayout->addWidget(create_Common_ChangeCurrentScanGroup());
    m_commonControlsBlock = new QWidget;
    m_commonControlsBlock->setLayout(commonLayout);

    auto textLayout = new As::VBoxLayout;
    textLayout->addWidget(create_Text_GotoGroup("TextGotoGroup", tr("Go to")));
    textLayout->addWidget(create_Text_FindGroup("TextFindGroup", tr("Find")));
    textLayout->addWidget(create_Text_ExtractGroup());
    m_textControlsBlock = new QWidget;
    m_textControlsBlock->setLayout(textLayout);

    auto tableLayout = new As::VBoxLayout;
    tableLayout->addWidget(create_Table_VisualizeGroup());
    m_tableControlsBlock = new QWidget;
    m_tableControlsBlock->setLayout(tableLayout);

    auto plotLayout = new As::VBoxLayout;
    plotLayout->addWidget(create_Plot_ExpDetailsGroup("PlotExpDetailsGroup", tr("Experimental details")));
    plotLayout->addWidget(create_Plot_ExpAnglesGroup("PlotExpAnglesGroup", tr("Experimental angles")));
    plotLayout->addWidget(create_Plot_ScanTreatGroup("PlotScanTreatGroup", tr("Scan treatment")));
    plotLayout->addWidget(create_Plot_ScanCorrectGroup("PlotScanCorrectGroup", tr("Scan correction")));
    plotLayout->addWidget(create_Plot_PeakIntegrateGroup("PlotPeakIntegrateGroup", tr("Peak integration")));
    plotLayout->addWidget(create_Plot_ShowOutputGroup());
    m_plotControlsBlock = new QWidget;
    m_plotControlsBlock->setLayout(plotLayout);

    auto outputLayout = new As::VBoxLayout;
    outputLayout->addWidget(create_Output_ExportGroup());
    m_outputControlsBlock = new QWidget;
    m_outputControlsBlock->setLayout(outputLayout);

    m_sidebarControlsLayout = new As::VBoxLayout;
    m_sidebarControlsLayout->addWidget(m_commonControlsBlock);
    m_sidebarControlsLayout->addWidget(m_textControlsBlock);
    m_sidebarControlsLayout->addWidget(m_tableControlsBlock);
    m_sidebarControlsLayout->addWidget(m_plotControlsBlock);
    m_sidebarControlsLayout->addWidget(m_outputControlsBlock);

    auto widget = new QWidget;
    widget->setLayout(m_sidebarControlsLayout);

    return widget;
}

/*!
Returns the widget 'Sidebar - Tabs - Settings'.
*/
QWidget *As::Window::createSidebarTabsSettingsWidget()
{
    ADEBUG;

    auto textLayout = new As::VBoxLayout;
    textLayout->addWidget(create_Text_ViewGroup("TextViewGroup", tr("View")));
    textLayout->addWidget(create_Text_FontGroup("TextFontGroup", tr("Font")));
    m_textSettingsBlock = new QWidget;
    m_textSettingsBlock->setLayout(textLayout);

    auto tableLayout = new As::VBoxLayout;
    m_tableSettingsBlock = new QWidget;
    m_tableSettingsBlock->setLayout(tableLayout);

    auto plotLayout = new As::VBoxLayout;
    plotLayout->addWidget(create_Plot_ViewGroup("PlotViewGroup", tr("View")));
    m_plotSettingsBlock = new QWidget;
    m_plotSettingsBlock->setLayout(plotLayout);

    auto outputLayout = new As::VBoxLayout;
    outputLayout->addWidget(create_Output_FormatGroup("OutputFormatGroup", tr("Format")));
    m_outputSettingsBlock = new QWidget;
    m_outputSettingsBlock->setLayout(outputLayout);

    m_sidebarSettingsLayout = new As::VBoxLayout;
    m_sidebarSettingsLayout->addWidget(m_textSettingsBlock);
    m_sidebarSettingsLayout->addWidget(m_tableSettingsBlock);
    m_sidebarSettingsLayout->addWidget(m_plotSettingsBlock);
    m_sidebarSettingsLayout->addWidget(m_outputSettingsBlock);

    auto widget = new QWidget;
    widget->setLayout(m_sidebarSettingsLayout);

    return widget;
}

/*!
Returns the group 'Common Controls - Change Current Scan'.
*/
As::GroupBox *As::Window::create_Common_ChangeCurrentScanGroup()
{
    ADEBUG;

    // Relatives: Go to scan changer
    auto scanChanger = new As::SpinBox;

    auto scanText = new As::Label;
    scanText->setText("0");
    scanText->setStyleSheet(QString("color: %1").arg(As::Color(As::blue).name()));

    auto scanBlock = new As::SpinBoxSingleBlock(scanChanger, scanText, tr("scans"));
    scanBlock->setToolTip(tr("Go to another scan."));
    //scanBlock->setEnabled(false);

    //connect(this, &As::Window::newScansExtracted_Signal, scanBlock, &As::SpinBoxSingleBlock::setEnabled);
    ///connect(this, &As::Window::newScansExtracted_Signal, scanChanger, QOverload<int>::of(&As::SpinBox::valueChanged));
    connect(this, &As::Window::scansRangeChanged_Signal, scanChanger, &As::SpinBox::setRange);
    connect(this, &As::Window::scansCountChanged_Signal, scanText, &As::Label::setText);
    ///connect(this, &As::Window::newScansPlotted_Signal, scanChanger, QOverload<int>::of(&As::SpinBox::valueChanged));
    ///connect(this, &As::Window::newPeaksIntegrated, scanChanger, QOverload<int>::of(&As::SpinBox::valueChanged));
    /////connect(scanChanger, QOverload<int>::of(&As::SpinBox::valueChanged), &m_scans, &As::ScanArray::setScanIndex); // slot emits scanIndexChanged
    /////connect(&m_scans, &As::ScanArray::scanIndexChanged, this, &As::Window::gotoScan_Slot);
    /////connect(&m_scans, &As::ScanArray::scanIndexChanged, this, &As::Window::highlightScanLines_Slot);
    //connect(scanChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::gotoScan_Slot);
    //CONNECT_SBINT(scanChanger, &As::SpinBox::valueChanged, this, &As::Window::gotoScan_Slot);
    connect(scanChanger, SB_INT&As::SpinBox::valueChanged, this, &As::Window::gotoScan_Slot);

    //-connect(&m_scans, &As::ScanArray::scanIndexChanged, this, &As::Window::highlightScanLines_Slot);
//    connect(scanChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::highlightScanLines_Slot);
//    connect(scanChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::highlightFoundText_Slot);

    // Relatives: Exclude current scan
    auto excludeScan = new As::CheckBox(tr("Exclude current scan"));
    excludeScan->setToolTip(tr("Exclude current scan from the treatment process and output."));
    excludeScan->setChecked(false);
    connect(excludeScan, &As::CheckBox::toggled, this, &As::Window::excludeScan_Slot);
    connect(this, &As::Window::excludeScanStateChanged_Signal, excludeScan, &As::CheckBox::setCheckedSilently);
    ///connect(this, &As::Window::excludeScanStateChanged_Signal, this, &As::Window::createFullOutputTableModel_Slot);
    connect(excludeScan, &As::CheckBox::stateChanged, this, &As::Window::createFullOutputTableModel_Slot);

    auto layout = new QVBoxLayout;
    layout->addWidget(scanBlock);
    layout->addWidget(excludeScan);

    auto group = new As::GroupBox;
    group->setLayout(layout);
    group->hide();
    connect(this, &As::Window::newScansExtracted_Signal, group, &As::GroupBox::show);

    return group;
}

/*!
Returns the group 'Text - Go to'.
*/
As::GroupBox *As::Window::create_Text_GotoGroup(const QString &objectName,
                                                const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    // Relatives: Go to file changer
    auto fileChanger = new As::SpinBox;
    auto fileText = new As::Label;
    fileText->setText("0");
    fileText->setStyleSheet(QString("color: %1").arg(As::Color(As::blue).name()));
    auto fileBlock = new As::SpinBoxSingleBlock(fileChanger, fileText, tr("files"));
    fileBlock->setToolTip(tr("Go to another file."));
    //fileBlock->setEnabled(false);
    //connect(this, &As::Window::newFilesLoaded_Signal, fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged));
    connect(this, &As::Window::filesRangeChanged_Signal, fileChanger, &As::SpinBox::setRange);
    connect(this, &As::Window::filesCountChanged_Signal, fileText, &As::Label::setText);
    /////connect(fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged), &m_scans, &As::ScanArray::setFileIndex); // slot emits fileIndexChanged
    /////connect(&m_scans, &As::ScanArray::fileIndexChanged, this, &As::Window::gotoFile_Slot);
    /////connect(&m_scans, &As::ScanArray::fileIndexChanged, fileChanger, SB&As::SpinBox::setValueSilently);
    //-//connect(fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::gotoFile_Slot);
    //connect(fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::gotoFile_Slot);

    //connect(fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::gotoFile_Slot);
    //connect(fileChanger, SIGNAL(valueChanged(int)), this, SLOT(gotoFile_Slot(int)));
    //QObject::connect<void(QSpinBox::*)(int)>(fileChanger, &As::SpinBox::valueChanged, this, &As::Window::gotoFile_Slot);
    //CONNECT_SBINT(fileChanger, &As::SpinBox::valueChanged, this, &As::Window::gotoFile_Slot);

    connect(fileChanger, SB_INT&As::SpinBox::valueChanged, this, &As::Window::gotoFile_Slot);


//#define SB (void(QSpinBox::*)(int))
//#define SB (void(QSpinBox::*)(int))

    //connect(spinBox, QOverload<int>::of(&As::SpinBox::valueChanged),
    //      [=](int i){ /* ... */ });


    //+//+//connect(m_scans, &As::ScanArray::fileIndexChanged, fileChanger, SB&As::SpinBox::setValueSilently);
    //connect(m_scans, &As::ScanArray::fileIndexChanged, fileChanger, &As::SpinBox::setValueSilently);
    connect(m_scans, &As::ScanArray::fileIndexChanged, fileChanger, SB_INT&As::SpinBox::setValueSilently);
    //connect(m_scans, SIGNAL(fileIndexChanged(int)), fileChanger, SLOT(setValueSilently(int)));

    //connect(&m_scans, &As::ScanArray::fileIndexChanged, this, &As::Window::gotoFile_Slot);
    //connect(fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged), m_inputTextWidget, &As::TextEditor::clearAllSelections);
    //connect(fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::highlightCurrentScanLines_Slot);
    //connect(fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::highlightFoundText_Slot);
    //CONNECT_SBINT(fileChanger, &As::SpinBox::valueChanged, this, &As::Window::highlightCurrentScanLines_Slot);
    //CONNECT_SBINT(fileChanger, &As::SpinBox::valueChanged, this, &As::Window::highlightFoundText_Slot);
    connect(fileChanger, SB_INT&As::SpinBox::valueChanged, this, &As::Window::highlightCurrentScanLines_Slot);
    connect(fileChanger, SB_INT&As::SpinBox::valueChanged, this, &As::Window::highlightFoundText_Slot);



    // Relatives
    auto lineChanger = new As::SpinBox;
    auto lineText = new As::Label;
    lineText->setText("0");
    lineText->setStyleSheet(QString("color: %1").arg(As::Color(As::blue).name()));
    auto lineBlock = new As::SpinBoxSingleBlock(lineChanger, lineText, tr("lines"));
    lineBlock->setToolTip(tr("Go to another line in the file."));
    connect(this, &As::Window::linesRangeChanged_Signal, lineChanger, &As::SpinBox::setRange);
    connect(this, &As::Window::linesCountChanged_Signal, lineText, &As::Label::setText);
    ///connect(lineChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::gotoLine_Slot);
    //connect(lineChanger, QOverload<int>::of(&As::SpinBox::valueChanged), m_inputTextWidget, TE&As::TextEditor::setCursorPosition);
    //connect(lineChanger, QOverload<int>::of(&As::SpinBox::valueChanged), m_inputTextWidget, &As::TextEditor::setCursorPosition);
    //CONNECT_SBINT(lineChanger, &As::SpinBox::valueChanged, m_inputTextWidget, &As::TextEditor::setCursorPosition); // Error in Linux
    //connect(lineChanger, SIGNAL(valueChanged(int)), m_inputTextWidget, SLOT(setCursorPosition(int)));
    //connect(m_inputTextWidget, TE_INT&As::TextEditor::cursorPositionChanged, lineChanger, SB_INT&As::SpinBox::setValueSilently);
    //connect(m_inputTextWidget, &As::TextEditor::cursorPositionChanged, lineChanger, &As::SpinBox::setValueSilently); // Error in Linux
    //connect(m_inputTextWidget, SIGNAL(cursorPositionChanged(int)), lineChanger, SLOT(setValueSilently(int)));
    //connect(m_inputTextWidget, TE&As::TextEditor::cursorPositionChanged, lineChanger, SB&As::SpinBox::setValue);
    connect(lineChanger, SB_INT&As::SpinBox::valueChanged, m_inputTextWidget, &As::TextEditor::setCursorPosition);
    connect(m_inputTextWidget, TE_INT&As::TextEditor::cursorPositionChanged, lineChanger, SB_INT&As::SpinBox::setValueSilently);

    // Layout
    auto layout = new QVBoxLayout;
    layout->addWidget(fileBlock);
    layout->addWidget(lineBlock);

    // Settings
    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);

    return group;
}

/*!
Returns the group 'Text - Controls - Find'.
*/
As::GroupBox *As::Window::create_Text_FindGroup(const QString &objectName,
                                              const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    const auto isCaseIgnored = QSettings().value("TextControls/ignoreCase", true).toBool();

    auto ignoreCase = new As::CheckBox(tr("Ignore case"));
    ignoreCase->setToolTip(tr("Ignore case in the search."));
    ignoreCase->setChecked(isCaseIgnored);
    connect(ignoreCase, &As::CheckBox::toggled, this, &As::Window::ignoreCase_Slot);
    connect(ignoreCase, &As::CheckBox::toggled, this, &As::Window::highlightFoundText_Slot);

    auto matchChanger = new As::SpinBox;
    auto matchText = new As::Label;
    matchText->setText("0");
    matchText->setStyleSheet(QString("color: %1").arg(As::Color(As::blue).name()));
    auto matchBlock = new As::SpinBoxSingleBlock(matchChanger, matchText, tr("matches"));
    matchBlock->setToolTip(tr("The total number of matches in the search."));
    connect(this, &As::Window::matchesRangeChanged_Signal, matchChanger, &As::SpinBox::setRange);
    connect(this, &As::Window::matchesCountChanged_Signal, matchText, &As::Label::setText);
    connect(this, &As::Window::matchesIndexChanged_Signal, matchChanger, &As::SpinBox::setValueSilently);
    //connect(matchChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::gotoMatch_Slot);
    //CONNECT_SBINT(matchChanger, &As::SpinBox::valueChanged, this, &As::Window::gotoMatch_Slot);
    connect(matchChanger, SB_INT&As::SpinBox::valueChanged, this, &As::Window::gotoMatch_Slot);

    auto findField = new As::LineEdit;
    findField->setObjectName("findField"); // required in the highlightFoundText_Slot()
    findField->setToolTip(tr("???"));
    findField->setPlaceholderText(tr("Find"));
    findField->setStyleSheet(QString("color: %1").arg(As::Color(As::gray).name()));

    // Timer for start of the search
    m_delayBeforeSearching = new QTimer(this); // doesn't work without 'this'
    m_delayBeforeSearching->setSingleShot(true);
    connect(findField, &As::LineEdit::textEdited, this, &As::Window::textSearchTimer_Slot);

    auto layout = new QVBoxLayout;
    layout->addWidget(ignoreCase);
    layout->addWidget(findField);
    layout->addWidget(matchBlock);

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);

    return group;
}

/*!
Returns the group 'Text - Controls - Extract'.
*/
As::GroupBox *As::Window::create_Text_ExtractGroup()
{
    ADEBUG;

    auto button = new As::PushButton(tr("Extract data"));
    button->setToolTip(tr("???"));
    connect(button, &As::PushButton::clicked, this, &As::Window::extractScans_Slot);
    connect(button, &As::PushButton::clicked, button, &As::PushButton::setEnabled);

    auto layout = new QVBoxLayout;
    layout->addWidget(button);

    auto group = new As::GroupBox;
    group->setLayout(layout);
    group->setProperty("isLast", true);

    return group;
}

/*!
Returns the group 'Text - Settings - View'.
*/
As::GroupBox *As::Window::create_Text_ViewGroup(const QString &objectName,
                                              const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    // Read previously saved application settings
    const bool isSyntaxHighlighted = QSettings().value("TextSettings/highlightSyntax", true).toBool();
    const bool isTextWraped        = QSettings().value("TextSettings/wrapText",       false).toBool();

    auto highlightSyntax = new As::CheckBox(tr("Highlight syntax"));
    highlightSyntax->setToolTip(tr("Highlight syntax for input files."));
    highlightSyntax->setChecked(isSyntaxHighlighted);
    connect(highlightSyntax, &As::CheckBox::toggled, this, &As::Window::highlightSyntax_Slot);

    auto wrapText = new As::CheckBox(tr("Wrap text"));
    wrapText->setToolTip(tr("Wrap lines to the width of the window."));
    wrapText->setChecked(isTextWraped);
    connect(wrapText, &As::CheckBox::toggled, this, &As::Window::wrapText_Slot);

    auto layout = new QVBoxLayout;
    layout->addWidget(highlightSyntax);
    layout->addWidget(wrapText);

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);

    return group;
}

/*!
Returns the group 'Text - Settings - Font'.
*/
As::GroupBox *As::Window::create_Text_FontGroup(const QString &objectName,
                                                const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    // Default font settings
    const int defaultSize = 13;
    QString defaultFamily;
#ifdef Q_OS_OSX
    defaultFamily = "Monaco";
#endif
    ADEBUG << "default font" << defaultFamily << defaultSize;

    // Read previously saved application settings
    const QString family = QSettings().value("TextSettings/fontFamily", defaultFamily).toString();
    const int size = QSettings().value("TextSettings/fontSize", defaultSize).toInt();

    //auto font = monospacedFonts;
    auto font = new As::FontComboBox;
    font->setFontFilters(QFontComboBox::MonospacedFonts); // slow!?
    font->setToolTip(tr("Choose the font family."));
    connect(font, &As::FontComboBox::currentFontChanged, this, &As::Window::setFont_Slot);
    font->setCurrentFont(QFont(family, size)); // after connect!

    auto fontSize = new As::SpinBox;
    fontSize->setToolTip(tr("Set the font size."));
    fontSize->setRange(1, 32);
    fontSize->setSuffix(" pt");
    fontSize->setValue(size);
    connect(fontSize, qOverload<int>(&As::SpinBox::valueChanged), this, &As::Window::setFontSize_Slot);

    auto layout = new QHBoxLayout;
    layout->addWidget(font);
    layout->addWidget(fontSize);

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);

    return group;
}

/*!
Returns the group 'Table - Controls - Visualize'.
*/
As::GroupBox *As::Window::create_Table_VisualizeGroup()
{
    ADEBUG;

    auto button = new As::PushButton(tr("Visualize extracted data"));
    button->setToolTip(tr("???"));
    connect(button, &As::PushButton::clicked, this, &As::Window::visualizePlots_Slot);
    connect(button, &As::PushButton::clicked, button, &As::PushButton::setEnabled);

    auto layout = new QVBoxLayout;
    layout->addWidget(button);

    auto group = new As::GroupBox;
    group->setLayout(layout);
    group->setProperty("isLast", true);

    return group;
}

/*!
Returns the group 'Plot - Controls - Experimental details'.
*/
As::GroupBox *As::Window::create_Plot_ExpDetailsGroup(const QString &objectName,
                                                      const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    auto layout = new QVBoxLayout;
    layout->setSpacing(UI_VERTICAL_SPACE_ROWS);

    QStringList itemKeys = {"conditions"};
    for (const auto &itemKey : itemKeys) {
        QStringList subitemKeys = SCAN_DATABASE[itemKey].keys();
        for (const auto &subitemKey : subitemKeys) {
            // Label
            auto label = new QLabel(subitemKey);

            // Data
            auto data = new QLabel("");
            data->setObjectName(itemKey + subitemKey + "Data");
            data->setStyleSheet(QString("QLabel {color: %1}").arg(As::Color(As::blue).name()));

            // Units
            auto units = new QLabel(SCAN_DATABASE[itemKey][subitemKey]["units"]);
            units->setStyleSheet(QString("QLabel {color: %1}").arg(As::Color(As::grayLight).name()));

            // Widget (Label + Data + Units)
            auto widget = new As::LabelTripleBlock(label, data, units);
            widget->setObjectName(itemKey + subitemKey + "Widget");
            widget->hide();

            // Layout
            layout->addWidget(widget); } }

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);

    return group;
}

/*!
Returns the group 'Plot - Controls - Experimental angles'.
*/
As::GroupBox *As::Window::create_Plot_ExpAnglesGroup(const QString &objectName,
                                                     const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    auto layout = new QGridLayout;
    layout->setSpacing(UI_VERTICAL_SPACE_ROWS);

    auto header = new As::LabelQuatroBlock(new QLabel(""),
                                           new QLabel(tr("Value")),
                                           new QLabel(tr("Range")),
                                           new QLabel(tr("Step")));
    layout->addWidget(header);

    QStringList itemKeys = {"angles"};
    for (const auto &itemKey : itemKeys) {
        QStringList subitemKeys = SCAN_DATABASE[itemKey].keys();
        for (const auto &subitemKey : subitemKeys) {
            // Label
            auto label = new QLabel(subitemKey);

            // Value
            auto value = new QLabel("");
            value->setObjectName(itemKey + subitemKey + "Value");
            value->setStyleSheet(QString("QLabel {color: %1}").arg(As::Color(As::blue).name()));

            // Range
            auto range = new QLabel("");
            range->setObjectName(itemKey + subitemKey + "Range");
            range->setStyleSheet(QString("QLabel {color: %1}").arg(As::Color(As::grayLight).name()));

            // Step
            auto step = new QLabel("");
            step->setObjectName(itemKey + subitemKey + "Step");
            step->setStyleSheet(QString("QLabel {color: %1}").arg(As::Color(As::grayLight).name()));

            // Widget (Label + Value + Range + Step)
            auto widget = new As::LabelQuatroBlock(label, value, range, step);
            widget->setObjectName(itemKey + subitemKey + "Widget");
            widget->hide();

            // Layout
            layout->addWidget(widget); } }

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);

    return group;
}

/*!
Returns the group 'Plot - Controls - Scan treatment'.
*/
As::GroupBox *As::Window::create_Plot_ScanTreatGroup(const QString &objectName,
                                                     const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    auto individualTreat = new As::CheckBox(tr("Individual treatment of current scan"));
    individualTreat->setToolTip(tr("Treat current scan with independent settings"));
    connect(individualTreat, &As::CheckBox::clicked, this, &As::Window::treatIndividually_Slot);
    connect(this, &As::Window::individualTreatStateChanged_Signal, individualTreat, &As::CheckBox::setCheckedSilently);

    auto layout = new QVBoxLayout;
    layout->addWidget(individualTreat);

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);
    group->setHidden(true);
    connect(this, &As::Window::calculateButtonPressed_Signal, group, &As::GroupBox::setHidden);

    return group;
}

/*!
Returns the group 'Plot - Controls - Scan correction'.
*/
As::GroupBox *As::Window::create_Plot_ScanCorrectGroup(const QString &objectName,
                                                       const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    // Relatives: Remove neighbors type
    //auto removeNeighborsType = new As::ComboBox;
    m_removeNeighborsType = new As::ComboBox;
    //removeNeighborsType->setObjectName("removeNeighborsType");
    m_removeNeighborsType->setToolTip(tr("???"));
    m_removeNeighborsType->addItem(tr("Manually remove neighbors"));  // !!! make as m_backgroundType->addItems(As::Scan::BKG_TYPES);
    m_removeNeighborsType->addItem(tr("Automatically remove neighbors"));
    //connect(removeNeighborsType, SIGNAL(currentTextChanged(QString)), obj, SLOT(removeNeighborsTypeSelected(QString)));
    connect(m_removeNeighborsType, &As::ComboBox::currentTextChanged, this, &As::Window::selectRemoveNeighborsType_Slot); // Error in Linux
    //connect(m_removeNeighborsType, SIGNAL(currentTextChanged(QString)), this, SLOT(selectRemoveNeighborsType_Slot(QString)));

    // Relatives: Numbers of points to be skipped in the scan
    m_leftSkipCount = new As::SpinBox;
    //leftChanger->setObjectName("leftSkipPointsChanger");
    //connect(m_leftSkipCount, SIGNAL(valueChanged(int)), this, SLOT(setLeftSkipCount_Slot(int)));
    //connect(m_leftSkipCount, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::setLeftSkipCount_Slot);
    //CONNECT_SBINT(m_leftSkipCount, &As::SpinBox::valueChanged, this, &As::Window::setLeftSkipCount_Slot);
    connect(m_leftSkipCount, SB_INT&As::SpinBox::valueChanged, this, &As::Window::setLeftSkipCount_Slot);

    m_nonSkipPointsCount = new As::Label;
    m_nonSkipPointsCount->setObjectName("nonSkipPointsCount");

    m_rightSkipCount = new As::SpinBox;
    //rightChanger->setObjectName("rightSkipPointsChanger");
    //connect(rightChanger, SIGNAL(valueChanged(int)), obj, SLOT(rightSkipPointsChanged(int)));
    //connect(m_rightSkipCount, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::setRightSkipCount_Slot);
    //CONNECT_SBINT(m_rightSkipCount, &As::SpinBox::valueChanged, this, &As::Window::setRightSkipCount_Slot);
    connect(m_rightSkipCount, SB_INT&As::SpinBox::valueChanged, this, &As::Window::setRightSkipCount_Slot);

    auto skipRemainSkip = new As::SpinBoxDoubleBlock(m_leftSkipCount, m_nonSkipPointsCount, m_rightSkipCount,
                                                     tr("left skip"), tr("non-skip"), tr("right skip"));
    skipRemainSkip->setToolTip(tr("???"));

    auto layout = new QVBoxLayout;
    layout->addWidget(m_removeNeighborsType);
    layout->addWidget(skipRemainSkip);

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);
    group->setHidden(true);
    connect(this, &As::Window::calculateButtonPressed_Signal, group, &As::GroupBox::setHidden);

    return group;
}

/*!
Returns the group 'Plot - Controls - Peak integration'.
*/
As::GroupBox *As::Window::create_Plot_PeakIntegrateGroup(const QString &objectName,
                                                       const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    // Relatives: Peak integration type
    auto integrationType = new As::ComboBox;
    //integrationType->setObjectName("integrationType");
    integrationType->setToolTip(tr("Select peak integration type"));
    integrationType->addItem(tr("Conventional peak integration"));
    ///integrationType->addItem(tr("Peak profile fitting")); // Hide until it is implemented !!!
    //connect(integrationType, SIGNAL(currentTextChanged(QString)), obj, SLOT(integrationTypeSelected(QString)));

    // Relatives: Background type
    //auto backgroundType = new As::ComboBox;
    m_backgroundType = new As::ComboBox;
    //backgroundType->setObjectName("backgroundType");
    m_backgroundType->setToolTip(tr("Select algorithm for background estimation"));
    m_backgroundType->addItems(As::Scan::BKG_TYPES);
    //connect(backgroundType, SIGNAL(currentTextChanged(QString)), obj, SLOT(integrationSubTypeSelected(QString)));
    connect(m_backgroundType, &As::ComboBox::currentTextChanged, this, &As::Window::selectIntegrationSubType_Slot); // Error in Linux
    //connect(m_backgroundType, &QComboBox::currentTextChanged, this, &As::Window::selectIntegrationSubType_Slot); // Error in Linux
    //connect(m_backgroundType, SIGNAL(currentTextChanged(QString)), this, SLOT(selectIntegrationSubType_Slot(QString)));

    // Relatives: Numbers of background points in the scan
    //auto bkgLeft = new As::SpinBox;
    m_leftBkgCount = new As::SpinBox;
    //bkgLeft->setObjectName("leftBkgPointsChanger");
    //connect(bkgLeft, SIGNAL(valueChanged(int)), obj, SLOT(leftBkgPointsChanged(int)));
    //connect(m_leftBkgCount, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::setLeftBkgCount_Slot);
    //CONNECT_SBINT(m_leftBkgCount, &As::SpinBox::valueChanged, this, &As::Window::setLeftBkgCount_Slot);
    connect(m_leftBkgCount, SB_INT&As::SpinBox::valueChanged, this, &As::Window::setLeftBkgCount_Slot);

    //auto peak = new As::Label;
    m_peakPointsCount = new As::Label;
    m_peakPointsCount->setObjectName("peakPointsCount");

    m_rightBkgCount = new As::SpinBox;
    //connect(m_rightBkgCount, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::setRightBkgCount_Slot);
    //CONNECT_SBINT(m_rightBkgCount, &As::SpinBox::valueChanged, this, &As::Window::setRightBkgCount_Slot);
    connect(m_rightBkgCount, SB_INT&As::SpinBox::valueChanged, this, &As::Window::setRightBkgCount_Slot);

    //connect(bkgRight, SIGNAL(valueChanged(int)), obj, SLOT(rightBkgPointsChanged(int)));
    auto bkgPeakBkg = new As::SpinBoxDoubleBlock(m_leftBkgCount, m_peakPointsCount, m_rightBkgCount,
                                                 tr("left background"), tr("peak"), tr("right background"));
    //bkgPeakBkg->setObjectName("bkgPeakBkg");
    bkgPeakBkg->setToolTip(tr("???"));

    // Relatives: Fit type
    auto fitType = new As::ComboBox;
    //fitType->setObjectName("fitType");
    fitType->setToolTip(tr("Select function for the peak fitting"));
    fitType->addItems(As::Scan::FIT_TYPES);
    fitType->hide(); //hide until implemented
    //connect(fitType, SIGNAL(currentTextChanged(QString)), obj, SLOT(integrationSubTypeSelected(QString)));

    /*
    // Relatives: Integrate button
    auto calcButton = new As::PushButton(tr("Calculate structure factors"));
    //integrateButton->setObjectName("integrateButton");
    calcButton->setToolTip(tr("???"));
    //connect(integrateButton, SIGNAL(clicked()), obj, SLOT(integrateButtonClicked()));
    connect(calcButton, &As::PushButton::clicked, this, &As::Window::calcStructureFactor_Slot);
    connect(calcButton, &As::PushButton::clicked, calcButton, &As::PushButton::setEnabled);
    connect(calcButton, &As::PushButton::clicked, this, &As::Window::calculateButtonPressed_Signal);
    */

    auto layout = new QVBoxLayout;
    layout->addWidget(integrationType);
    layout->addWidget(m_backgroundType);
    layout->addWidget(bkgPeakBkg);
    layout->addWidget(fitType);
    //layout->addWidget(calcButton);

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);
    group->setHidden(true);
    connect(this, &As::Window::calculateButtonPressed_Signal, group, &As::GroupBox::setHidden);

    return group;
}

/*!
Returns the group 'Plot - Controls - Output'.
*/
As::GroupBox *As::Window::create_Plot_ShowOutputGroup()
{
    ADEBUG;

    auto calcButton = new As::PushButton(tr("Calculate structure factors"));
    calcButton->setToolTip(tr("???"));
    connect(calcButton, &As::PushButton::clicked, this, &As::Window::calcStructureFactor_Slot);
    connect(calcButton, &As::PushButton::clicked, calcButton, &As::PushButton::setEnabled);
    connect(calcButton, &As::PushButton::clicked, this, &As::Window::calculateButtonPressed_Signal);

    auto outputButton = new As::PushButton(tr("Show output table"));
    outputButton->setToolTip(tr("???"));
    outputButton->setDisabled(true);
    connect(outputButton, &As::PushButton::clicked, this, &As::Window::showOutput_Slot);
    connect(this, &As::Window::calculateButtonPressed_Signal, outputButton, &As::PushButton::setDisabled);

    auto layout = new QVBoxLayout;
    layout->addWidget(calcButton);
    layout->addWidget(outputButton);

    auto group = new As::GroupBox;
    group->setLayout(layout);
    group->setProperty("isLast", true);

    return group;
}

/*!
Returns the group 'Plot - Settings - View'.
*/
As::GroupBox *As::Window::create_Plot_ViewGroup(const QString &objectName,
                                                const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    auto isLegendHidden  = QSettings().value("PlotSettings/hideLegend", false).toBool();

    auto hideLegend = new As::CheckBox(tr("Hide legend"));
    hideLegend->setToolTip(tr("Hide legend on the plot."));
    hideLegend->setChecked(isLegendHidden);
    connect(hideLegend, &As::CheckBox::toggled, this, &As::Window::hideLegend_Slot);

    auto layout = new QVBoxLayout;
    layout->addWidget(hideLegend);

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);

    return group;
}

/*!
Returns the group 'Output - Controls - Export'.
*/
As::GroupBox *As::Window::create_Output_ExportGroup()
{
    ADEBUG;

    auto button = new As::PushButton(tr("Export output data"));
    button->setToolTip(tr("???"));
    connect(button, &As::PushButton::clicked, this, &As::Window::export_Slot);

    auto layout = new QVBoxLayout;
    layout->addWidget(button);

    auto group = new As::GroupBox;
    group->setLayout(layout);
    group->setProperty("isLast", true);

    return group;
}

/*!
Returns the group 'Output - Settings - Format'.
*/
As::GroupBox *As::Window::create_Output_FormatGroup(const QString &objectName,
                                                    const QString &title)
{
    ADEBUG << "objectName and title:" << objectName << title;

    // names do not correspond?!
    auto isExcludedSaved = QSettings().value("OutputSettings/exportExcluded", false).toBool();
    auto isAlwaysHeadersSaved = QSettings().value("OutputSettings/alwaysSaveHeaders", false).toBool();

    auto exportExcluded = new As::CheckBox(tr("Export excluded scans"));
    exportExcluded->setToolTip(tr("Export excluded scans to the output files."));
    exportExcluded->setChecked(isExcludedSaved);
    connect(exportExcluded, &As::CheckBox::toggled, this, &As::Window::exportExcluded_Slot);

    auto alwaysSaveHeaders = new As::CheckBox(tr("Always save headers"));
    alwaysSaveHeaders->setToolTip(tr("Always save headers in the output files."));
    alwaysSaveHeaders->setChecked(isAlwaysHeadersSaved);
    connect(alwaysSaveHeaders, &As::CheckBox::toggled, this, &As::Window::alwaysSaveHeaders_Slot);

    auto layout = new QVBoxLayout;
    layout->addWidget(exportExcluded);
    layout->addWidget(alwaysSaveHeaders);

    auto group = new As::GroupBox(objectName, title);
    group->setLayout(layout);

    return group;
}

/*!
Shows or hide sidebar group boxes depends on the selected tab of mainTabs.
*/
/*
void As::Window::showOrHideSidebarBlocks_Slot333(const int index)
{
    ADEBUG << "index:" << index;

    // Show the required blocks only
    if (index == 3) {
        m_textControlsBlock->show();
        m_textSettingsBlock->show();
    }

}
*/

/*!
Returns the table model for the program output.
*/
// called when switch to any tab in m_tabsWidget. change to m_outputTableWidget only !?
void As::Window::createFullOutputTableModel_Slot()
{
    ADEBUG;

    // Exit if no m_outputTableWidget exist yet
    if (!m_outputTableWidget)
        return;

    // Create full output table for every scan
    m_scans->createFullOutputTable();

    // Number of columns and rows for the required table
    const int columnCount = m_scans->m_outputTableHeaders.size();
    const int rowCount = m_scans->size();

    // Create model
    auto tableModel = new QStandardItemModel;
    tableModel->setRowCount(rowCount);
    tableModel->setColumnCount(columnCount);

    // Set headers
    for (int iColumn = 0; iColumn < columnCount; ++iColumn)
        tableModel->setHorizontalHeaderItem(iColumn, new QStandardItem(m_scans->m_outputTableHeaders[iColumn]));

    // Set values
    for (int iRow = 0; iRow < rowCount; ++iRow) {
        for (int iColumn = 0; iColumn < columnCount; ++iColumn) {
            tableModel->setItem(iRow, iColumn, new QStandardItem(m_scans->m_outputTableData[iRow][iColumn]));
            tableModel->item(iRow, iColumn)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); } }

    // Set model
    ADEBUG;
    m_outputTableWidget->setModel(tableModel);
    ADEBUG;

    // Highlight row with current scan data
    update_OutputTable_Highlight(currentScanIndex()-1);

    ADEBUG;
}

