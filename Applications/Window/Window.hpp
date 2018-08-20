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

#ifndef AS_WINDOW_HPP
#define AS_WINDOW_HPP

#include <QPointer>

#include <QMainWindow>

#include "Scan.hpp"
#include "ScanArray.hpp"

class QAbstractItemModel;
class QAction;
class QDragEnterEvent;
class QDropEvent;
class QFont;
class QFontComboBox;
class QString;
class QStringList;
class QTabWidget;
class QTextCursor;
class QTimer;
class QWidget;
template<typename> class QList;

namespace As { //AS_BEGIN_NAMESPACE

class ComboBox;
class GroupBox;
class Label;
class Plot;
class ProgressDialog;
class SpinBox;
class Sidebar;
class SaveHeaders;
class TableView;
class TextEditor;
class VBoxLayout;

class Window : public QMainWindow {
    Q_OBJECT

  public:
    Window();
    ~Window();

    void show();
    void writeSettings();
    void loadFiles(const QStringList& filePathList);

    const Scan* scanAt(const int index) const;
    As::Scan* currentScan() const;
    As::Scan* genericScan() const;
    int currentScanIndex() const;

  protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

  signals:
    // General
    void quit();

    // For toolbar
    void facilityTypeChanged(const QString& text);
    void instrumentTypeChanged(const QString& text);
    void dataTypeChanged(const QString& text);

    // For all sidebar tabs
    void currentScanChangedSignal(const int index);
    ///void currentScanUpdatedSignal(const int index);
    void excludeScanStateChangedSignal(const bool treat);

    // For text sidebar tab
    void newFilesLoadedSignal(const int count);
    void oldFilesClosedSignal(const bool areClosed);
    void currentFileIndexChangedSignal(const int index);
    void currentFilePathChangedSignal(const QString& path);
    void currentFileContentChangedSignal(const QString& content);
    void filesCountChangedSignal(const QString& count);
    void filesRangeChangedSignal(const int min, const int max);
    void linesCountChangedSignal(const QString& count);
    void linesRangeChangedSignal(const int min, const int max);
    void matchesCountChangedSignal(const QString& count);
    void matchesRangeChangedSignal(const int min, const int max);
    void matchesIndexChangedSignal(const int index);

    // For table sidebar tab
    void newScansExtractedSignal(const int count);
    void scansCountChangedSignal(const QString& count);
    void scansRangeChangedSignal(const int min, const int max);
    void extractedTableModelChanged(QAbstractItemModel* model);

    // For plot sidebar tab
    void newScansPlottedSignal(const int count);
    void individualTreatStateChangedSignal(const bool treat);
    void calculateButtonPressedSignal(const bool);
    void peaksTreatmentIsFinished(const bool);

    // For output sidebar tab
    void newPeaksIntegrated(const int count);

  private slots:
    // Debug
    void printAppInfoSlot();

    // Actions
    void openFileSlot();
    void openDirSlot();
    void reloadFileSlot();
    void closeFileSlot();
    void exportSlot();
    void exportImageSlot();
    void exportOutputTableSlot();
    void aboutAppSlot();
    void showPreferencesSlot();
    void showSidebarSlot(const bool show = true);
    void showOrHideSidebarBlocksSlot(const int index);
    void openUserManualSlot();
    void openIssueTrackerSlot();

    // Update application
    void acceptAutoUpdateSlot();
    void rejectAutoUpdateSlot();
    void setAutoUpdateSettings(const bool autoUpdate);
    void checkApplicationUpdateNow(const bool hideOutput = false);
    void installUpdateSlot();

    // For all sidebar tabs
    void gotoScanSlot(const int index);
    void updateScanSlot();
    void excludeScanSlot(const bool exclude);
    void autoProcessingSlot();

    // For text sidebar tab
    void ignoreCaseSlot(const bool ignore);
    void gotoLineSlot(const int index);
    void gotoFileSlot(const int index);
    void gotoMatchSlot(const int index);
    //void textSearchTimerSlot();
    void extractScansSlot();
    void highlightSyntaxSlot(const bool highlight);
    void wrapTextSlot(const bool wrap);
    void setFontSlot(const QFont& font);
    void setFontSizeSlot(const int size);
    //void highlightCurrentLineSlot();
    void highlightCurrentScanLinesSlot(const int fileIndex);
    void highlightScanLinesSlot(const int scanIndex);
    void highlightFoundTextSlot();
    void textSearchTimerSlot();
    //void highlightTextFoundTimerSlot();
    //void highlightAll();

    // For table sidebar tab
    void visualizePlotsSlot();

    // For plot sidebar tab
    void treatIndividuallySlot(const bool b);

    void selectNeighborsRemoveType(int index);
    void setLeftSkipCountSlot(const int count);
    void setRightSkipCountSlot(const int count);

    void selectPeakAnalysisType(int index);
    void selectBkgDetectType(int index);
    void selectPeakFitType(int index);
    void setLeftBkgCountSlot(const int count);
    void setRightBkgCountSlot(const int count);

    void calcStructureFactorSlot();
    void showOutputSlot();
    void hideLegendSlot(const bool hide);

    // For output sidebar tab
    void exportExcludedSlot(const bool save);
    void alwaysSaveHeadersSlot(const bool save);

  private:
    // Init
    bool isFirstApplicationStart() const;
    void initMonospacedFonts() const;
    void setApplicationStartCount();
    void offerAutoUpdate();
    void checkApplicationUpdate();
    void setupWindowSizeAndPosition();
    void createActionsMenusToolBar();
    void createStatusBar();
    QString createStyleSheet() const;

    // Test
    void autoRun(const QString& path);

    // Misc
    void openFiles(const QStringList& pathList);
    QString maintainerPath();

    // Update widgets
    //void updateChangeScanGroup(const As::Scan &scan);
    void update_Plot_ExpDetailsGroup(const As::Scan* scan);
    void update_Plot_ExpAnglesGroup(const As::Scan* scan);
    //void update_Plot_ScanTreatGroup(const As::Scan &scan);
    void update_Plot_ScanCorrectGroup(const As::Scan* scan);
    void update_Plot_PeakIntegrateGroup(const As::Scan* scan);
    void update_OutputTable_Highlight(const int index);

    // Process
    void concurrentRun(const QString& type,
                       As::ScanArray* scans) const;

    //==========
    // Variables
    //==========

    // Files or folders pathes list (for the reload action)
    QStringList m_pathList;
    // Array of experimental scans and single scan
    As::ScanArray* m_scans = Q_NULLPTR;
    As::Scan* m_commonScan = Q_NULLPTR;
    // Misc
    //QFontComboBox *monospacedFonts;
    QTimer* m_delayBeforeSearching;
    QList<QTextCursor> m_searchMatches;
    QAction* m_copyTextAct;
    bool m_hideUpdateOutput;

    //========
    // Widgets
    //========

    // Drag and drop widget
    QWidget* createDragAndDropWidget();

    // Main widget: Tabs widget + Sidebar widgets
    QWidget* createDataWidget();
    QWidget* createMainWidget();
    //QWidget *m_mainWidget;

    // Tabs widget
    QTabWidget* createTabsWidget();
    QTabWidget* m_tabsWidget;
    As::TextEditor* createInputTextWidget();
    As::TextEditor* m_inputTextWidget;
    //QPointer<As::TableView> m_extractedTableWidget; //signal-slot description doesn't work with QPointer in linux gcc
    //As::TableView *m_extractedTableWidget;          //with * works in linux gcc, but pointer is not set automatically to 0...
    QPointer<As::Plot> m_visualizedPlotsWidget;
    QPointer<As::TableView> m_outputTableWidget;    // with simple pointer crashes if reopen files... fix!
    //As::TableView* m_outputTableWidget;

    // Sidebar widgets
    QWidget* createSidebarWidget();
    QWidget* m_sidebarWidget;
    QTabWidget* createSidebarTabsWidget();
    QWidget* createSidebarTabsControlsWidget();
    QWidget* createSidebarTabsSettingsWidget();
    As::ProgressDialog* m_progressDialog;

    //===============
    // Sidebar Groups
    //===============

    As::VBoxLayout* m_sidebarControlsLayout;
    As::VBoxLayout* m_sidebarSettingsLayout;

    // For all sidebar tabs
    QWidget* m_commonControlsBlock;
    As::GroupBox* create_Common_ChangeCurrentScanGroup();

    // For text sidebar tab
    QWidget* m_textControlsBlock;
    QWidget* m_textSettingsBlock;
    As::GroupBox* create_Text_GotoGroup(const QString& objectName, const QString& title);
    As::GroupBox* create_Text_FindGroup(const QString& objectName, const QString& title);
    As::GroupBox* create_Text_ExtractGroup();
    As::GroupBox* create_Text_ViewGroup(const QString& objectName, const QString& title);
    As::GroupBox* create_Text_FontGroup(const QString& objectName, const QString& title);

    // For table sidebar tab
    QWidget* m_tableControlsBlock;
    QWidget* m_tableSettingsBlock;
    As::GroupBox* create_Table_VisualizeGroup();

    // For plot sidebar tab
    QWidget* m_plotControlsBlock;
    QWidget* m_plotSettingsBlock;
    As::GroupBox* create_Plot_ExpDetailsGroup(const QString& objectName, const QString& title);
    As::GroupBox* create_Plot_ExpAnglesGroup(const QString& objectName, const QString& title);
    As::GroupBox* create_Plot_ScanTreatGroup(const QString& objectName, const QString& title);
    As::GroupBox* create_Plot_ScanCorrectGroup(const QString& objectName, const QString& title);
    As::GroupBox* create_Plot_PeakIntegrateGroup(const QString& objectName, const QString& title);
    As::GroupBox* create_Plot_ShowOutputGroup();
    As::GroupBox* create_Plot_ViewGroup(const QString& objectName, const QString& title);
    As::ComboBox* m_removeNeighborsType;
    As::ComboBox* m_backgroundType;
    As::SpinBox* m_leftSkipCount;
    As::SpinBox* m_rightSkipCount;
    As::SpinBox* m_leftBkgCount;
    As::SpinBox* m_rightBkgCount;
    As::Label* m_nonSkipPointsCount;
    As::Label* m_peakPointsCount;

    // For output sidebar tab
    QWidget* m_outputControlsBlock;
    QWidget* m_outputSettingsBlock;
    As::GroupBox* create_Output_ExportGroup();
    As::GroupBox* create_Output_FormatGroup(const QString& objectName, const QString& title);
    void createFullOutputTableModelSlot();

};

} //AS_END_NAMESPACE

#endif // AS_WINDOW_HPP
