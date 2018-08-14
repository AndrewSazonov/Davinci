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

#ifndef AS_DIFFRACTION_SCANARRAY_HPP
#define AS_DIFFRACTION_SCANARRAY_HPP

#include <QObject>
#include <QVector>

#include "Constants.hpp"

class QString;
class QStringList;
template<typename> class QFutureWatcher;
//template<typename> class QVector;

namespace As { //AS_BEGIN_NAMESPACE

class RealMatrix9;
class RealVector;
class SaveHeaders;
class Scan;

class ScanArray : public QObject {
    Q_OBJECT

  public:

    // ScanArray.cpp/Base.cpp
    ScanArray(QObject* parent = Q_NULLPTR);
    ~ScanArray();

    As::Scan* operator[](const int i);
    const As::Scan* operator[](const int i) const;

    As::Scan* at(const int i);
    const As::Scan* at(const int i) const;

    As::Scan* begin();
    As::Scan* end();

    int size() const;
    void clear();

    void append(As::Scan* scan);

    int scanIndex() const;
    int fileIndex() const;

    void setSelectedOutputColumns(As::SaveHeaders& saveHeaders,
                                  QString& table);
    void saveSelectedOutputColumns(const QString& fileName,
                                   const QString& filter);

    // ScanArray.cpp/Detect.cpp
    bool detectInputFilesType();

    // ScanArray.cpp/Extract.cpp
    void extractDataFromFile(const int index);

    // ScanArray.cpp/Fill.cpp
    void fillMissingDataArray(const int index);

    // ScanArray.cpp/Index.cpp
    void indexSinglePeak(const int index);

    // ScanArray.cpp/Treat.cpp
    void preTreatSinglePeak(const int index);
    void treatSinglePeak(const int index);
    void createFullOutputTable();

  public slots:

    // ScanArray.cpp/Base.cpp
    void setScanIndex(const int index);
    void setFileIndex(const int index);
    void setFileIndexByScanIndex(const int index); // not in use?

    // ScanArray.cpp/Detect.cpp
    void setInputFileType(const As::InputFileType type);

  signals:

    void facilityTypeChanged(const QString& type);
    void instrumentTypeChanged(const QString& type);
    void dataTypeChanged(const QString& type);

    void scanIndexChanged(const int index);
    void fileIndexChanged(const int index);

    void scanCountChanged(const int size);

  public: // move to private!

    QString m_facilityType;     // Name of the facility used
    QString m_instrumentType;   // Name of the instrument used
    QString m_dataType;         // Name of the data measured

    QPair<QStringList, QStringList> m_inputFilesContents; // Content of all the input files

    As::InputFileType m_inputFilesType = As::InputFileType(0); // Type of input file

    QStringList m_outputTableHeaders;       // Headers of the output table
    QList<QStringList> m_outputTableData;   // Output table data


  private:

    QVector<As::Scan*> m_scanArray; // Array of pointers to the individual scans

    int m_scanIndex = 0; // Index of the currently processed scan
    int m_fileIndex = 0; // Index of the file which contains the currently processed scan

    // ScanArray.cpp/Extract.cpp

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
    void extractDataFromTable(As::Scan* scan,
                              QList<QStringList>& headerMap);
    void appendScan(As::Scan* scan);


    // ScanArray.cpp/Fill.cpp

    void calcUnpolData(const QString& section,
                       const QString& entry,
                       As::Scan* scan);


    // ScanArray.cpp/Index.cpp

    const As::RealVector anglesToXyz(const qreal wavelength,
                                     qreal gamma,
                                     qreal nu,
                                     qreal omega) const;
    const As::RealVector anglesToXyz(const qreal wavelength,
                                     qreal twotheta,
                                     qreal omega,
                                     qreal chi,
                                     qreal phi) const;
    const As::RealVector xyzToHkl(const As::RealMatrix9& ub,
                                  const qreal x,
                                  const qreal y,
                                  const qreal z) const;
    const As::RealVector hklToXyz(const As::RealMatrix9& ub,
                                  const qreal h,
                                  const qreal k,
                                  const qreal l) const;
    const As::RealVector xyzToAngles(const qreal wavelength,
                                     const qreal x,
                                     const qreal y,
                                     const qreal z,
                                     qreal psi) const;
    const As::RealVector directionCosines(const As::RealMatrix9& ub,
                                          qreal twotheta,
                                          qreal omega,
                                          qreal chi,
                                          qreal phi) const;
    void calcDirectionCosines(As::Scan* scan);
    void correctForAzimuthAnglePsi(qreal& omega,
                                   qreal& chi,
                                   qreal& phi,
                                   qreal psi,
                                   const qreal q2xyz,
                                   const qreal z) const;

    // ScanArray.cpp/Treat.cpp
    void definePolarisationCrossSection(As::Scan* scan);
    void normalizeByTime(As::Scan* scan);
    void findNonPeakPoints(As::Scan* scan);
    void adjustBkgPoints(As::Scan* scan);

    void calcEsd(As::Scan* scan);
    void calcBkg(As::Scan* scan);
    void calcMaxPeakInty(As::Scan* scan);
    void calcSumPeakInty(As::Scan* scan);
    void calcPeakArea(As::Scan* scan);
    void calcNormPeakArea(As::Scan* scan);
    void calcStructFactor(As::Scan* scan);
    void calcFullWidthHalfMax(As::Scan* scan);
    void calcFlippingRatio(As::Scan* scan);

    qreal lorentzCorrectionFactor(const qreal gammaMean,
                                  const qreal nuMean = 0.);
    As::RealVector IntensityWithSigma(const As::RealVector& intensities,
                                      const As::RealVector& sigmas,
                                      const int numLeftBkgPoints,
                                      const int numRightBkgPoints,
                                      const int numLeftSkipPoints,
                                      const int numRightSkipPoints,
                                      const qreal mcCandlishFactor);

};

} //AS_END_NAMESPACE

#endif // AS_DIFFRACTION_SCANARRAY_HPP
