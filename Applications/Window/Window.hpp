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

#ifndef AS_WINDOW_HPP
#define AS_WINDOW_HPP

#include <QPointer>

#include <QMainWindow>

#include "Scan.hpp"
#include "ScanArray.hpp"

QT_BEGIN_NAMESPACE

class QAbstractItemModel;
class QAction;
class QDragEnterEvent;
class QDropEvent;
class QFont;
class QFontComboBox;
class QNetworkReply;
class QSvgWidget;
class QString;
class QStringList;
class QTabWidget;
class QTextEdit;
class QTextCursor;
class QTextStream;
class QTimer;
class QWidget;

QT_END_NAMESPACE

namespace As { //AS_BEGIN_NAMESPACE

class ComboBox;
class GroupBox;
class TextEditor;
class Plot;
class SpinBox;
class Label;
class Sidebar;
class SaveHeaders;
class TableView;
class VBoxLayout;

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window();
    ~Window();

    void writeSettings();

    void loadFiles(const QStringList &filePathList);

    const Scan *scanAt(const int index) const;
    As::Scan *currentScan() const;
    As::Scan *genericScan() const;
    int currentScanIndex() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    // Misc
    void facilityTypeChanged_Signal(const QString &text);
    void instrumentTypeChanged_Signal(const QString &text);
    void dataTypeChanged_Signal(const QString &text);

    // For all sidebar tabs
    void currentScanChanged_Signal(const int index);
    ///void currentScanUpdated_Signal(const int index);
    void excludeScanStateChanged_Signal(const bool treat);

    // For text sidebar tab
    void newFilesLoaded_Signal(const int count);
    void oldFilesClosed_Signal(const bool areClosed);
    void currentFileIndexChanged_Signal(const int index);
    void currentFilePathChanged_Signal(const QString &path);
    void currentFileContentChanged_Signal(const QString &content);
    void filesCountChanged_Signal(const QString &count);
    void filesRangeChanged_Signal(const int min, const int max);
    void linesCountChanged_Signal(const QString &count);
    void linesRangeChanged_Signal(const int min, const int max);
    void matchesCountChanged_Signal(const QString &count);
    void matchesRangeChanged_Signal(const int min, const int max);
    void matchesIndexChanged_Signal(const int index);

    // For table sidebar tab
    void newScansExtracted_Signal(const int count);
    void scansCountChanged_Signal(const QString &count);
    void scansRangeChanged_Signal(const int min, const int max);
    void extractedTableModelChanged(QAbstractItemModel *model);

    // For plot sidebar tab
    void newScansPlotted_Signal(const int count);
    void individualTreatStateChanged_Signal(const bool treat);
    void calculateButtonPressed_Signal(const bool);

    // For output sidebar tab
    void newPeaksIntegrated(const int count);

private slots:
    // Misc
    void printAppInfo_Slot();

    // Actions
    void openFile_Slot();
    void openDir_Slot();
    void reloadFile_Slot();
    void closeFile_Slot();
    void export_Slot();
    void exportImage_Slot();
    void exportOutputTable_Slot();
    void aboutApp_Slot();
    void showPreferences_Slot();
    void showSidebar_Slot(const bool show = true);
    void showOrHideSidebarBlocks_Slot(const int index);

    // Update application
    void acceptAutoUpdate_Slot();
    void rejectAutoUpdate_Slot();
    void setAutoUpdate_Slot(const bool autoUpdate);
    void checkApplicationUpdateNow_Slot(const bool hideOutput = false);
    void installUpdate_Slot();

    // For all sidebar tabs
    void gotoScan_Slot(const int index);
    void updateScan_Slot();
    void excludeScan_Slot(const bool exclude);
    void autoProcessing_Slot();

    // For text sidebar tab
    void ignoreCase_Slot(const bool ignore);
    void gotoLine_Slot(const int index);
    void gotoFile_Slot(const int index);
    void gotoMatch_Slot(const int index);
    //void textSearchTimer_Slot();
    void extractScans_Slot();
    void highlightSyntax_Slot(const bool highlight);
    void wrapText_Slot(const bool wrap);
    void setFont_Slot(const QFont &font);
    void setFontSize_Slot(const int size);
    //void highlightCurrentLine_Slot();
    void highlightCurrentScanLines_Slot(const int fileIndex);
    void highlightScanLines_Slot(const int scanIndex);
    void highlightFoundText_Slot();
    void textSearchTimer_Slot();
    //void highlightTextFoundTimer_Slot();
    //void highlightAll();

    // For table sidebar tab
    void visualizePlots_Slot();

    // For plot sidebar tab
    void treatIndividually_Slot(const bool treat);
    void selectRemoveNeighborsType_Slot(const QString &type);
    void setLeftSkipCount_Slot(const int count);
    void setRightSkipCount_Slot(const int count);
    void selectIntegrationSubType_Slot(const QString &type);
    void setLeftBkgCount_Slot(const int count);
    void setRightBkgCount_Slot(const int count);
    void calcStructureFactor_Slot();
    void showOutput_Slot();
    void hideLegend_Slot(const bool hide);

    // For output sidebar tab
    void exportExcluded_Slot(const bool save);
    void alwaysSaveHeaders_Slot(const bool save);

private:
    // Init
    void setApplicationStartCount();
    void offerAutoUpdate();
    void checkApplicationUpdate();
    void setupWindowSizeAndPosition();
    void createActionsMenusToolBar();
    void createStatusBar();
    QString createStyleSheet() const;

    // Misc
    void openFiles(const QStringList &pathList);
    QString maintainerPath();

    // Updates
    //void updateChangeScanGroup(const As::Scan &scan);
    void update_Plot_ExpDetailsGroup(const As::Scan *scan);
    void update_Plot_ExpAnglesGroup(const As::Scan *scan);
    //void update_Plot_ScanTreatGroup(const As::Scan &scan);
    void update_Plot_ScanCorrectGroup(const As::Scan *scan);
    void update_Plot_PeakIntegrateGroup(const As::Scan *scan);
    void update_OutputTable_Highlight(const int index);

    //==========
    // Variables
    //==========

    // Files or folders pathes list (for the reload action)
    QStringList m_pathList;
    // Array of experimental scans and single scan for global settings
    As::ScanArray *m_scans;
    As::Scan *m_commonScan;
    // Misc
    QFontComboBox *monospacedFonts;
    QTimer *m_delayBeforeSearching;
    QList<QTextCursor> m_searchMatches;
    QAction *m_copyTextAct;
    bool m_hideUpdateOutput;

    //========
    // Widgets
    //========

    // Drag and drop widget
    QWidget *createDragAndDropWidget();

    // Main widget: Tabs widget + Sidebar widgets
    QWidget *createDataWidget();
    QWidget *createMainWidget();
    //QWidget *m_mainWidget;

    // Tabs widget
    QTabWidget *createTabsWidget();
    QTabWidget *m_tabsWidget;
    As::TextEditor *createInputTextWidget();
    As::TextEditor *m_inputTextWidget;
    //QPointer<As::TableView> m_extractedTableWidget; //signal-slot description doesn't work with QPointer in linux gcc
    //As::TableView *m_extractedTableWidget;          //with * works in linux gcc, but pointer is not set automatically to 0...
    QPointer<As::Plot> m_visualizedPlotsWidget;
    QPointer<As::TableView> m_outputTableWidget;

    // Sidebar widgets
    QWidget *createSidebarWidget();
    QWidget *m_sidebarWidget;
    QTabWidget *createSidebarTabsWidget();
    QWidget *createSidebarTabsControlsWidget();
    QWidget *createSidebarTabsSettingsWidget();

    //===============
    // Sidebar Groups
    //===============

    As::VBoxLayout *m_sidebarControlsLayout;
    As::VBoxLayout *m_sidebarSettingsLayout;

    // For all sidebar tabs
    QWidget *m_commonControlsBlock;
    As::GroupBox *create_Common_ChangeCurrentScanGroup();

    // For text sidebar tab
    QWidget *m_textControlsBlock;
    QWidget *m_textSettingsBlock;
    As::GroupBox *create_Text_GotoGroup(const QString &objectName, const QString &title);
    As::GroupBox *create_Text_FindGroup(const QString &objectName, const QString &title);
    As::GroupBox *create_Text_ExtractGroup();
    As::GroupBox *create_Text_ViewGroup(const QString &objectName, const QString &title);
    As::GroupBox *create_Text_FontGroup(const QString &objectName, const QString &title);

    // For table sidebar tab
    QWidget *m_tableControlsBlock;
    QWidget *m_tableSettingsBlock;
    As::GroupBox *create_Table_VisualizeGroup();

    // For plot sidebar tab
    QWidget *m_plotControlsBlock;
    QWidget *m_plotSettingsBlock;
    As::GroupBox *create_Plot_ExpDetailsGroup(const QString &objectName, const QString &title);
    As::GroupBox *create_Plot_ExpAnglesGroup(const QString &objectName, const QString &title);
    As::GroupBox *create_Plot_ScanTreatGroup(const QString &objectName, const QString &title);
    As::GroupBox *create_Plot_ScanCorrectGroup(const QString &objectName, const QString &title);
    As::GroupBox *create_Plot_PeakIntegrateGroup(const QString &objectName, const QString &title);
    As::GroupBox *create_Plot_ShowOutputGroup();
    As::GroupBox *create_Plot_ViewGroup(const QString &objectName, const QString &title);
    As::ComboBox *m_removeNeighborsType;
    As::ComboBox *m_backgroundType;
    As::SpinBox *m_leftSkipCount;
    As::SpinBox *m_rightSkipCount;
    As::SpinBox *m_leftBkgCount;
    As::SpinBox *m_rightBkgCount;
    As::Label *m_nonSkipPointsCount;
    As::Label *m_peakPointsCount;

    // For output sidebar tab
    QWidget *m_outputControlsBlock;
    QWidget *m_outputSettingsBlock;
    As::GroupBox *create_Output_ExportGroup();
    As::GroupBox *create_Output_FormatGroup(const QString &objectName, const QString &title);
    void createFullOutputTableModel_Slot();

};

} //AS_END_NAMESPACE

#endif // AS_WINDOW_HPP
