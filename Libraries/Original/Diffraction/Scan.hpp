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
#include <QStandardItemModel> // Get rid of this in the scan!

#include "Constants.hpp"

class QString;
class QStringList;
template <class Key, class T> class QMap;

namespace As { //AS_BEGIN_NAMESPACE

class RealVector;

class Scan : public QObject {
    Q_OBJECT

  public:
    Scan(QObject* parent = Q_NULLPTR);
    ~Scan();

    void init();

    void setData(const QString& section,
                 const QString& entry,
                 const QString& data);
    void appendData(const QString& section,
                    const QString& entry,
                    const QString& data);
    void removeData(const QString& section,
                    const QString& entry);

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
                                  const QString& entry,
                                  const QString& format) const;
    const QString printDataSingle(const QString& section,
                                  const QString& entry) const;
    const QString printDataRange(const QString& section,
                                 const QString& entry) const;

    const QStringList keys() const;

    const As::ScanSectionMap_t operator[](const QString& key) const;
    const As::ScanMap_t toQMap() const;


    void setScanStep(const qreal value);
    qreal scanStep() const;

    void setAbsoluteFilePath(const QString& name);
    const QString absoluteFilePath() const;
    const QString absolutePath() const;
    const QString baseName() const;
    const QString absolutePathWithBaseName() const;
    const QString absolutePathWithBaseNameAndHkl() const;

    void setFileIndex(const int index);
    int fileIndex() const;

    void setPlotType(const As::PlotType plotType);
    As::PlotType plotType() const;

    // Return Miller index based on its string name
    qreal millerIndex(const QString& index) const;

    // Scan parameters
    qreal numPoints() const;
    int scanLine() const;
    const QString scanAngle() const;
    void setScanAngle(const QString& name);
    void findScanAngle();

    //protected:


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

    // MISC
    QStandardItemModel* m_tableModel;
    qreal m_mcCandlishFactor;

    bool m_isIndividuallyTreated = false;
    //bool m_isAlreadyTreated = false;

    static const QStringList FIT_TYPES;
    static const QStringList BKG_TYPES;

    QString m_removeNeighborsType = "Manually remove neighbors";
    QString m_integrationType = "Conventional peak integration";
    QString m_integrationSubType = BKG_TYPES[0];

  public slots:
    void createExtractedTableModel_Slot();
    // void setSize(const int size);

  signals:
    // void extractedTableModelChanged_Signal(QStandardItemModel *tableModel);
    // void sizeChanged(const int size);

  private:
    As::ScanMap_t m_scan;
    QString m_scanAngle;

    QString m_absoluteFilePath; // currently used!!! absoluteFilePath
    int m_fileIndex;
    As::PlotType m_plotType;
    qreal m_scanStep = qQNaN();

};

/*
    static const QHash<int, QString> BkgTypes;

    enum BkgTypesEnum
    {
    autoBkg,
    manualBkg
    };
*/

} //AS_END_NAMESPACE

QDebug operator<<(QDebug debug, const As::Scan& scan);

#endif // AS_DIFFRACTION_SCAN_HPP


