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

#ifndef AS_DIFFRACTION_SCANARRAY_HPP
#define AS_DIFFRACTION_SCANARRAY_HPP

#include <QObject>
#include <QVector>

#include "Constants.hpp"

//class QFutureWatcher<void>;
class QString;
class QStringList;
template <typename T> class QFutureWatcher;
template <typename T> class QVector;


namespace As { //AS_BEGIN_NAMESPACE

class RealMatrix9;
class RealVector;
class SaveHeaders;
class Scan;

class ScanArray : public QObject
{
    Q_OBJECT

public:
    ScanArray(QObject *parent = Q_NULLPTR);
    ~ScanArray();

    // Base methods: ScanArray.cpp/Base.cpp
    As::Scan *at(const int i);
    const As::Scan *at(const int i) const;
    As::Scan *operator[](const int i);
    const As::Scan *operator[](const int i) const;
    int size() const;
    void append(As::Scan *scan);
    As::Scan *begin();
    As::Scan *end();
    void clear();
    void setSelectedOutputColumns(As::SaveHeaders &saveHeaders,
                                  QString &table);
    void saveSelectedOutputColumns(const QString &fileName,
                                   const QString &filter);

    // Detect methods: ScanArray.cpp/Detect.cpp
    bool detectInputFileType();
    As::InputFileType filesType();

    // Extract methods: ScanArray.cpp/Extract.cpp
    void extractDataFromFile(const int index);

    // Fill methods: ScanArray.cpp/Fill.cpp
    void fillMissingDataArray(const int index);
    void calcUnpolData(const QString &section,
                       const QString &entry,
                       As::Scan *scan);

    // Indexing methods: ScanArray.cpp/Index.cpp
    void indexSinglePeak(const int index);
    void calcDirectionCosines(As::Scan *scan);

    // Treating methods: ScanArray.cpp/Treat.cpp
    void preTreatSingleScan(const int index);
    void treatSingleScan(const int index);
    void createFullOutputTable();

    //
    int scanIndex() const;
    int fileIndex() const;

public slots:
    void setInputFileType(const As::InputFileType type);

    void setScanIndex(const int index);
    void setFileIndex(const int index);
    void setFileIndexByScanIndex(const int index);

signals:
    //void inputFileTypeChanged_Signal(const As::InputFileType type);
    void facilityTypeChanged_Signal(const QString &type);
    void instrumentTypeChanged_Signal(const QString &type);
    void dataTypeChanged_Signal(const QString &type);

    void scanIndexChanged(const int index);
    void fileIndexChanged(const int index);

    void scanCountChanged(const int size);

    // treat
    //void signalTreatRangeChanged(const int min, const int max);
    //void signalTreatSingleFinished(const int index);
    //void signalTreatDataFinished();


    // processing
    //void signalProcessRangeChanged(const int min, const int max);
    //void signalPSingleFinished(const int index);
    //void signalProcessFinished();


private:
public: // to check qdoc!

    // Array of pointers to the individual scans
    QVector<As::Scan*> m_scanArray;

    // Type of data
    As::InputFileType m_inputFileType = As::InputFileType(0);
    QString m_facilityType;
    QString m_instrumentType;
    QString m_dataType;

    //
    QStringList m_outputTableHeaders;
    QList<QStringList> m_outputTableData;

    // Files contents
    QPair<QStringList, QStringList> m_inputFilesContents;

    //
    int m_scanIndex = 0;
    int m_fileIndex = 0;

    // Extract methods: ScanArray.cpp/Extract.cpp
    // Instrument specific methods
    void extractHeidiData(const int fileIndex,
                          const QString& filePath,
                          const QString& fileContent);
    void extractHeidiLog(const int fileIndex,
                         const QString& filePath,
                         const QString& fileContent);
    void extractNicosData(const int fileIndex,
                          const QString& filePath,
                          const QString& fileContent);
    void extractPoliLog(const int,
                        const QString&,
                        const QString&);
    void extract6t2Data(const int fileIndex,
                        const QString& filePath,
                        QString& fileContent);
    // Common methods
    void findScanAngle(As::Scan *scan);
    void extractDataFromTable(As::Scan *scan,
                              QList<QStringList> &headerMap);
    void appendScan(As::Scan *scan);

    // Indexing methods: ScanArray.cpp/Index.cpp
    const As::RealVector anglesToXyz(const qreal wavelength,
                                     qreal gamma,
                                     qreal nu,
                                     qreal omega) const;
    const As::RealVector anglesToXyz(const qreal wavelength,
                                     qreal twotheta,
                                     qreal omega,
                                     qreal chi,
                                     qreal phi) const;
    const As::RealVector xyzToHkl(const As::RealMatrix9 &ub,
                                  const qreal x,
                                  const qreal y,
                                  const qreal z) const;
    const As::RealVector hklToXyz(const As::RealMatrix9 &ub,
                                  const qreal h,
                                  const qreal k,
                                  const qreal l) const;
    const As::RealVector xyzToAngles(const qreal wavelength,
                                     const qreal x,
                                     const qreal y,
                                     const qreal z,
                                     qreal psi) const;
    void correctForAzimuthAnglePsi(qreal &omega,
                                   qreal &chi,
                                   qreal &phi,
                                   qreal psi,
                                   const qreal q2xyz,
                                   const qreal z) const;
    const As::RealVector directionCosines(const As::RealMatrix9 &ub,
                                          qreal twotheta,
                                          qreal omega,
                                          qreal chi,
                                          qreal phi) const;

    // Treating methods: ScanArray.cpp/Treat.cpp
    void definePolarisationCrossSection(As::Scan *scan);
    void calcEsd(As::Scan *scan);
    void normalizeByTime(As::Scan *scan);
    void findNonPeakPoints(As::Scan *scan);
    void adjustBkgPoints(As::Scan *scan);
    void calcBkg(As::Scan *scan);
    void calcMaxPeakInty(As::Scan *scan);
    void calcSumPeakInty(As::Scan *scan);
    void calcPeakArea(As::Scan *scan);
    void calcNormPeakArea(As::Scan *scan);
    void calcStructFactor(As::Scan *scan);
    void calcFullWidthHalfMax(As::Scan *scan);
    void calcFlippingRatio(As::Scan *scan);
    qreal lorentzCorrectionFactor(const qreal gammaMean,
                                  const qreal nuMean = 0.);
    As::RealVector IntensityWithSigma(const As::RealVector &intensities,
                                      const As::RealVector &sigmas,
                                      const int numLeftBkgPoints,
                                      const int numRightBkgPoints,
                                      const int numLeftSkipPoints,
                                      const int numRightSkipPoints,
                                      const qreal mcCandlishFactor);

};

} //AS_END_NAMESPACE

#endif // AS_DIFFRACTION_SCANARRAY_HPP

//class ScanArray : public QObject // QObject is required for the signal-slot functionality
