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

#ifndef AS_DIFFRACTION_SCAN_HPP
#define AS_DIFFRACTION_SCAN_HPP

#include <QObject>
#include <QStandardItemModel>

#include "Constants.hpp"

class QString;
class QStringList;
template <class Key, class T> class QMap;

namespace As { //AS_BEGIN_NAMESPACE

class RealVector;

class Scan : public QObject {
    Q_OBJECT

  public:

    // constructor and destructor

    Scan(QObject* parent = Q_NULLPTR);
    ~Scan();

    // operators

    const As::ScanSection_t operator[](const QString& key) const;
    As::ScanSection_t& operator[](const QString& key);

    // general set data methods

    void init();

    void setData(const QString& section,
                 const QString& entry,
                 const QString& data);
    void appendData(const QString& section,
                    const QString& entry,
                    const QString& data);
    void removeData(const QString& section,
                    const QString& entry);

    // general return data methods

    const QString value(const QString& section,
                        const QString& entry,
                        const QString& name,
                        bool* ok = Q_NULLPTR) const;
    const QString data(const QString& section,
                       const QString& entry,
                       bool* ok = Q_NULLPTR) const;
    const QString format(const QString& section,
                         const QString& entry,
                         bool* ok = Q_NULLPTR) const;

    const QString printDataSingle(const QString& section,
                                  const QString& entry) const;
    const QString printDataRange(const QString& section,
                                 const QString& entry) const;

    const QStringList keys() const;

    // convert methods

    const As::Scan_t toQMap() const;

    // set and get the scan parameters

    int scanLine() const;
    qreal numPoints() const;
    qreal millerIndex(const QString& index) const;

    void setFileIndex(const int index);
    int fileIndex() const;
    int m_fileIndex = 0; // move to private!

    void setAbsoluteFilePath(const QString& name);
    const QString absoluteFilePath() const;
    const QString absolutePath() const;
    const QString baseName() const;
    const QString absolutePathWithBaseName() const;
    const QString absolutePathWithBaseNameAndHkl() const;
    QString m_absoluteFilePath = ""; // move to private!

    void setScanStep(const qreal value);
    qreal scanStep() const;
    qreal m_scanStep = qQNaN(); // move to private!

    void setScanAngle(const QString& name);
    void findAndSetScanAngle();
    const QString scanAngle() const;
    QString m_scanAngle = ""; // move to private!

    void setPlotType(const As::PlotType plotType);
    As::PlotType plotType() const;
    As::PlotType m_plotType = As::PlotType::Raw; // move to private!

    void setMcCandlishFactor(const qreal val);
    qreal mcCandlishFactor() const;
    qreal m_mcCandlishFactor = 0.0; // move to private!

    void createExtractedTableModel();
    QStandardItemModel* extractedTableModel() const;
    QStandardItemModel* m_extractedTableModel; // move to private!

    // sidebar 'scan treatment' group

    void setIndividuallyTreated(const bool b);
    bool isIndividuallyTreated() const;
    bool m_isIndividuallyTreated = false; // move to private!

    // sidebar 'scan correction' group

    enum NeighborsRemoveType { ManualNeighborsRemove, AutoNeighborsRemove };
    Q_ENUM(NeighborsRemoveType)
    static const QMap<As::Scan::NeighborsRemoveType, QString> NeighborsRemoveTypeDict;
    void setNeighborsRemoveType(const As::Scan::NeighborsRemoveType type);
    As::Scan::NeighborsRemoveType neighborsRemoveType() const;
    As::Scan::NeighborsRemoveType m_neighborsRemoveType = As::Scan::ManualNeighborsRemove; // move to private!

    // sidebar 'peak integration' group -> rename to 'peak analysis' group?
    //https://www.originlab.com/index.aspx?go=products/origin/dataanalysis/peakanalysis

    enum PeakAnalysisType { PeakIntegration, PeakFit };
    Q_ENUM(PeakAnalysisType)
    static const QMap<As::Scan::PeakAnalysisType, QString> PeakAnalysisTypeDict;
    void setPeakAnalysisType(const As::Scan::PeakAnalysisType type);
    As::Scan::PeakAnalysisType peakAnalysisType() const;
    As::Scan::PeakAnalysisType m_peakAnalysisType = As::Scan::PeakIntegration; // move to private!

    enum BkgDetectType { ManualBkgSet, AutoBkgDetect };
    Q_ENUM(BkgDetectType)
    static const QMap<As::Scan::BkgDetectType, QString> BkgDetectTypeDict;
    void setBkgDetectType(const As::Scan::BkgDetectType type);
    As::Scan::BkgDetectType bkgDetectType() const;
    As::Scan::BkgDetectType m_bkgDetectType = As::Scan::AutoBkgDetect; // move to private!

    enum PeakFitType { GaussFit, LorentzFit, PseudoVoigtFit };
    Q_ENUM(PeakFitType)
    static const QMap<As::Scan::PeakFitType, QString> PeakFitTypeDict;
    void setPeakFitType(const As::Scan::PeakFitType type);
    As::Scan::PeakFitType peakFitType() const;
    As::Scan::PeakFitType m_peakFitType = As::Scan::GaussFit; // move to private!

    // move to private!

    int m_numLeftSkipPoints;
    int m_numRightSkipPoints;
    int m_numNonSkipPoints;
    int m_numLeftBkgPoints;
    int m_numRightBkgPoints;
    int m_numPeakPoints;

    qreal m_normMeanBkg;
    qreal m_fullWidthHalfMax, m_fullWidthHalfMaxErr;
    qreal m_flippingRatio, m_flippingRatioErr;
    qreal m_peakPosition;

    QMap<QString, qreal> m_maxPeakInty,  m_maxPeakIntyErr;
    QMap<QString, qreal> m_sumPeakInty,  m_sumPeakIntyErr;
    QMap<QString, qreal> m_peakArea,     m_peakAreaErr;
    QMap<QString, qreal> m_normPeakArea, m_normPeakAreaErr;
    QMap<QString, qreal> m_structFactor, m_structFactorErr;

  private:
    As::Scan_t m_scan; };

} //AS_END_NAMESPACE

QDebug operator<<(QDebug debug, const As::Scan& scan);

#endif // AS_DIFFRACTION_SCAN_HPP


