#include <QDateTime>
#include <QtMath>

#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "Line.hpp"
#include "RealVector.hpp"
#include "Scan.hpp"

#include "ScanArray.hpp"

/*!
Treats the data preliminary.
*/
void As::ScanArray::preTreatData()
{
    ADEBUG2;

    // For every scan in the scan array
    for (auto scan : m_scanArray) {
        scan->setData("number", "Excluded", "0"); // Init. It's modified then in excludeScanSlot
        definePolarisationCrossSection(scan);
        calcEsd(scan);
        normalizeByTime(scan); }
}

/*!
Treats the single As::Scan with the given \a index in the scan array
if \a sure is \c true.
*/
void As::ScanArray::treatSingle(const int index,
                                const bool sure)
{
    //////ADEBUG << "index:" << index << ", sure:" << sure;

    if (!sure)
        return;

    auto scan = m_scanArray.at(index);

    findNonPeakPoints(scan);
    adjustBkgPoints(scan);
    calcBkg(scan);
    calcMaxPeakInty(scan);
    calcSumPeakInty(scan);
    calcPeakArea(scan);
    calcNormPeakArea(scan);
    calcStructFactor(scan);
    calcFullWidthHalfMax(scan);
    calcFlippingRatio(scan);

    if (scan->plotType() != As::Excluded)
        scan->setPlotType(As::Integrated); // what is better?!

    //scan.setData("number", "xcluded", scan.plotType() == As::Scan::Excluded ? "0" : "1"); // it's modified then in excludeScanSlot
    //scan.m_isAlreadyTreated = true; // what is better?
}

/*!
Treats all the non-treated scans in the scan array.
*/
void As::ScanArray::treatData()
{
    ADEBUG2;

    for (int i = 0; i < size(); ++i) {
        if (at(i)->plotType() == As::PlotType(As::Raw)) {
           treatSingle(i); } }

    //createFullOutputTable();
}

/*!
Creates the output table with all the processed parameters.
*/
void As::ScanArray::createFullOutputTable()
{
    ADEBUG;

    // Set the groups to be shown in the output table. Their order is preserved.
    // Group elements (subitems) are sorted automatically by QMap in alphabetic order
    // of their string keys
    const QStringList itemKeys = {"number", "indices", "calculations",
                                  "angles", "cosines", "conditions"};

    // Make a map (headerMap) and list (m_outputTableHeaders) of the actually measured headers
    // from the 1st scan: m_scanArray.at(0)
    m_outputTableHeaders = QStringList();
    QMap<QString, QStringList> headerMap;
    for (const auto &itemKey : itemKeys) {
        const QStringList subitemKeys = (*m_scanArray.at(0))[itemKey].keys();
        m_outputTableHeaders.append(subitemKeys);
        headerMap.insert(itemKey, subitemKeys); }
    //m_outputTableHeaders = QStringList();
    //for (const auto &itemKey : itemKeys)
    //    m_outputTableHeaders.append(headerMap[itemKey]);
    /*QStringList headerRow;
    for (const auto &itemKey : itemKeys) {
        auto scan = m_scanArray.at(0);
        const QStringList subitemKeys = (*scan)[itemKey].keys();
        headerMap.insert(itemKey, subitemKeys);
        for (const auto &subitemKey : subitemKeys) {
            if (!subitemKey.isEmpty())
                headerRow << subitemKey; } } */

    // Make a table of the calculated values according to the headerMap for all the peaks
    m_outputTableData = QList<QStringList>();
    for (const auto scan : m_scanArray) {
        QStringList dataRow;
        int j = 0;
        for (const auto &itemKey : itemKeys)
            for (const auto &subitemKey : headerMap[itemKey])
                    dataRow.insert(j++, scan->printDataSingle(itemKey, subitemKey));
        m_outputTableData << dataRow; }
    /*QList<QStringList> dataTable;
    for (auto scan : m_scanArray) {
        QStringList dataRow;
        for (const auto &itemKey : itemKeys) {
            const QStringList subitemKeys = (*scan)[itemKey].keys();
            for (const auto &subitemKey : subitemKeys) {
                ADEBUG << scan << itemKey << subitemKey;
                if (headerRow.contains(subitemKey)) {
                    const int column = headerRow.indexOf(subitemKey);
                    dataRow.insert(column, scan->printDataSingle(itemKey, subitemKey));
                } } }
        dataTable << dataRow; }*/
}

/*!
Defines the polarisation cross-section for the given \a scan.
*/
void As::ScanArray::definePolarisationCrossSection(As::Scan *scan)
{
    //////ADEBUG;

    // Read polarisation an flipper parameters
    QString fin  = (*scan)["polarisation"]["Fin" ]["data"].split(" ")[0];
    QString fout = (*scan)["polarisation"]["Fout"]["data"].split(" ")[0];
    QString pin  = (*scan)["polarisation"]["Pin" ]["data"].split(" ")[0];
    QString pout = (*scan)["polarisation"]["Pout"]["data"].split(" ")[0];

    // Adjust polarisation parameters depends on the flipper parameters
    if (fin == "on") {
        if (pin.startsWith("+"))
            pin.replace("+", "-");
        else
            pin.replace("-", "+"); }
    if (fout == "on") {
        if (pout.startsWith("+"))
            pout.replace("+", "-");
        else
            pout.replace("-", "+"); }

    // Set the measured polarisation cross-ection parameter
    if (!(pin + pout).isEmpty())
        scan->setData("conditions", "Polarisation (in/out)", pin + pout); // what if we work in a half polarized mode?
}

/*!
Calculates the estimated standard deviations for the measured detector and
monitor data arrays of the given \a scan.
*/
void As::ScanArray::calcEsd(As::Scan *scan)
{
    //////ADEBUG;

    for (const QString &countType : As::COUNT_TYPES) {
        const As::RealVector detector = (*scan)["intensities"]["Detector" + countType]["data"];
        const As::RealVector monitor  = (*scan)["intensities"]["Monitor" + countType]["data"];
        if (!detector.isEmpty())
            scan->setData("intensities", "sDetector" + countType, detector.sqrt().toQString());
        if (!monitor.isEmpty())
            scan->setData("intensities", "sMonitor" + countType,  monitor.sqrt().toQString()); }
}

/*!
Normalizes the measured detector and monitor data arrays of the given \a scan by the measured time.
*/
void As::ScanArray::normalizeByTime(As::Scan *scan)
{
    //////ADEBUG;

    for (const QString &countType : As::COUNT_TYPES) {
        const As::RealVector detector  = (*scan)["intensities"]["Detector" + countType]["data"];
        const As::RealVector sdetector = (*scan)["intensities"]["sDetector" + countType]["data"];
        const As::RealVector monitor   = (*scan)["intensities"]["Monitor" + countType]["data"];
        const As::RealVector smonitor  = (*scan)["intensities"]["sMonitor" + countType]["data"];
        const As::RealVector time      = (*scan)["conditions"]["Time/step" + countType]["data"];

        //ADEBUG << detector;
        //ADEBUG << sdetector;
        //ADEBUG << monitor;
        //ADEBUG << smonitor;
        //ADEBUG << time;

        if (!detector.isEmpty())
            scan->setData("intensities", "DetectorNorm" + countType,  detector.normalizeBy(time).toQString());
        if (!sdetector.isEmpty())
            scan->setData("intensities", "sDetectorNorm" + countType, sdetector.normalizeBy(time).toQString());
        if (!monitor.isEmpty())
            scan->setData("intensities", "MonitorNorm" + countType,   monitor.normalizeBy(time).toQString());
        if (!smonitor.isEmpty())
            scan->setData("intensities", "sMonitorNorm" + countType,  smonitor.normalizeBy(time).toQString()); }
}

/*
//Not in use!?
void As::ScanArray::findNonPeakPoints(const As::RealVector &inty)
{
    ADEBUG;

    int from = MIN_NUM_SKIP;
    int to =   inty.size() - MIN_NUM_SKIP - 2 * MIN_NUM_BKG;
    for (int numSkipLeft = from; numSkipLeft < to; ++numSkipLeft) {

        int from = MIN_NUM_SKIP;
        int to =   inty.size() - numSkipLeft;
        for (int numSkipRight = from; numSkipRight < to; ++numSkipRight) {

            int from = MIN_NUM_BKG;
            int to =   inty.size() - numSkipLeft - numSkipRight;
            for (int numBkgLeft = from; numBkgLeft < to; ++numBkgLeft) {

                int from = MIN_NUM_BKG;
                int to =   inty.size() - numSkipLeft - numSkipRight - numBkgLeft;
                for (int numBkgRight = from; numBkgRight < to; ++numBkgRight) {

                    qreal ratio = sigmaToInty(inty, numBkgLeft, numBkgRight, numSkipLeft, numSkipRight);
                    if (ratio > 0 AND ratio < ratio_) {
                        ratio_        = ratio;
                        numBkgLeft_   = numBkgLeft;
                        numBkgRight_  = numBkgRight;
                        numSkipLeft_  = numSkipLeft;
                        numSkipRight_ = numSkipRight; } } } } }
}*/

/*!
Finds non-peak points (neighbors to be skipped + background) of
the given \a scan.
*/
void As::ScanArray::findNonPeakPoints(As::Scan *scan)
{
    //////ADEBUG << scan;

    const As::RealVector detector  = (*scan)["intensities"]["DetectorNorm"]["data"];
    const As::RealVector sdetector = (*scan)["intensities"]["sDetectorNorm"]["data"];

    qreal minRatio = qInf();

    const bool autoSkip = scan->m_removeNeighborsType.contains("Automatically");
    const bool autoBkg = scan->m_integrationSubType.contains("Automatically");

    /////ADEBUG << "scan->m_mcCandlishFactor" << scan->m_mcCandlishFactor;

    // Automatically detect background and skip points
    if (autoBkg AND autoSkip) {

        // Find Bkg points
        const int fromLS = MIN_NUM_BKG;
        const int toLS   = scan->m_numPoints - MIN_NUM_BKG;
        for (int numLeftBkgPoints = fromLS; numLeftBkgPoints < toLS; ++numLeftBkgPoints) {
            const int fromRS = MIN_NUM_BKG;
            const int toRS   = scan->m_numPoints - numLeftBkgPoints;
            for (int numRightBkgPoints = fromRS; numRightBkgPoints < toRS; ++numRightBkgPoints) {
                const As::RealVector intyWithSig = IntensityWithSigma(detector, sdetector,
                                             numLeftBkgPoints, numRightBkgPoints,
                                             0, 0,
                                             scan->m_mcCandlishFactor);
                const qreal ratio = intyWithSig[1] / intyWithSig[0];
                if (ratio > 0 AND ratio < minRatio) {
                    minRatio = ratio;
                    scan->m_numLeftBkgPoints = numLeftBkgPoints;
                    scan->m_numRightBkgPoints = numRightBkgPoints; } } }

        // Find Skip points
        const int fromLB = MIN_NUM_SKIP;
        const int toLB   = scan->m_numLeftBkgPoints - MIN_NUM_BKG;
        for (int numLeftSkipPoints = fromLB; numLeftSkipPoints < toLB; ++numLeftSkipPoints) {
            const int numLeftBkgPoints = scan->m_numLeftBkgPoints - numLeftSkipPoints;
            const int fromRB = MIN_NUM_SKIP;
            const int toRB   = scan->m_numRightBkgPoints - MIN_NUM_BKG;
            for (int numRightSkipPoints = fromRB; numRightSkipPoints < toRB; ++numRightSkipPoints) {
                const int numRightBkgPoints = scan->m_numRightBkgPoints - numRightSkipPoints;
                const As::RealVector intyWithSig = IntensityWithSigma(detector, sdetector,
                                             numLeftBkgPoints, numRightBkgPoints,
                                             numLeftSkipPoints, numRightSkipPoints,
                                             scan->m_mcCandlishFactor);
                const qreal ratio = intyWithSig[1] / intyWithSig[0];
                ///////ADEBUG << ratio << minRatio << numLeftBkgPoints << numRightBkgPoints;
                if (ratio > 0 AND ratio < minRatio) {
                    minRatio = ratio;
                    scan->m_numLeftSkipPoints = numLeftSkipPoints;
                    scan->m_numRightSkipPoints = numRightSkipPoints; } } }

        // Update Bkg points
        scan->m_numLeftBkgPoints -= scan->m_numLeftSkipPoints;
        scan->m_numRightBkgPoints -= scan->m_numRightSkipPoints; }

    // Automatically detect background and manually set skip points
    else if (autoBkg AND !autoSkip) {

        // Find Bkg points
        const int numNonSkipPoints = scan->m_numPoints - scan->m_numLeftSkipPoints - scan->m_numRightSkipPoints;
        const int fromLB = MIN_NUM_BKG;
        const int toLB   = numNonSkipPoints - MIN_NUM_BKG;
        for (int numLeftBkgPoints = fromLB; numLeftBkgPoints < toLB; ++numLeftBkgPoints) {
            const int fromRB = MIN_NUM_BKG;
            const int toRB   = numNonSkipPoints - numLeftBkgPoints;
            for (int numRightBkgPoints = fromRB; numRightBkgPoints < toRB; ++numRightBkgPoints) {
                const As::RealVector intyWithSig = IntensityWithSigma(detector, sdetector,
                                             numLeftBkgPoints, numRightBkgPoints,
                                             scan->m_numLeftSkipPoints, scan->m_numRightSkipPoints,
                                             scan->m_mcCandlishFactor);
                const qreal ratio = intyWithSig[1] / intyWithSig[0];
                /////ADEBUG << ratio << minRatio << numLeftBkgPoints << numRightBkgPoints;
                if (ratio > 0 AND ratio < minRatio) {
                    minRatio = ratio;
                    scan->m_numLeftBkgPoints = numLeftBkgPoints;
                    scan->m_numRightBkgPoints = numRightBkgPoints; } } } }

    // Manually set background and automatically detect skip points
    else if (!autoBkg AND autoSkip) {
        const int numNonBkgPoints = scan->m_numPoints - scan->m_numLeftBkgPoints - scan->m_numRightBkgPoints;
        const int fromLS = MIN_NUM_SKIP;
        const int toLS   = numNonBkgPoints - MIN_NUM_SKIP;
        for (int numLeftSkipPoints = fromLS; numLeftSkipPoints < toLS; ++numLeftSkipPoints) {
            const int fromRS = MIN_NUM_SKIP;
            const int toRS   = numNonBkgPoints - numLeftSkipPoints - MIN_NUM_SKIP;
            for (int numRightSkipPoints = fromRS; numRightSkipPoints < toRS; ++numRightSkipPoints) {
                const As::RealVector intyWithSig = IntensityWithSigma(detector, sdetector,
                                             scan->m_numLeftBkgPoints, scan->m_numRightBkgPoints,
                                             numLeftSkipPoints, numRightSkipPoints,
                                             scan->m_mcCandlishFactor);
                const qreal ratio = intyWithSig[1] / intyWithSig[0];
                if (ratio > 0 AND ratio < minRatio) {
                    minRatio = ratio;
                    scan->m_numLeftSkipPoints = numLeftSkipPoints;
                    scan->m_numRightSkipPoints = numRightSkipPoints; } } } }

    // Manually set background and skip points
    else {
        IntensityWithSigma(detector, sdetector,
                           scan->m_numLeftBkgPoints, scan->m_numRightBkgPoints,
                           scan->m_numLeftSkipPoints, scan->m_numRightSkipPoints,
                           scan->m_mcCandlishFactor); }

    // Update points
    scan->m_numNonSkipPoints = scan->m_numPoints - scan->m_numLeftSkipPoints - scan->m_numRightSkipPoints;
    scan->m_numPeakPoints = scan->m_numNonSkipPoints - scan->m_numLeftBkgPoints - scan->m_numRightBkgPoints;
}

/*!
Adjusts the background points of the given \a scan.
*/
void As::ScanArray::adjustBkgPoints(As::Scan *scan)
{
    //////ADEBUG;

    if (scan->m_integrationSubType == "Automatically detect background") {
        for (int i = 0; i < ADD_NUM_PEAK; ++i) {
            if (scan->m_numLeftBkgPoints > 1) {
                --scan->m_numLeftBkgPoints;
                ++scan->m_numPeakPoints; }
            if (scan->m_numRightBkgPoints > 1) {
                --scan->m_numRightBkgPoints;
                ++scan->m_numPeakPoints; } } }
}

/*!
Calculates the average background of the given \a scan.
*/
void As::ScanArray::calcBkg(As::Scan *scan)
{
    /////ADEBUG;

    const As::RealVector detector = (*scan)["intensities"]["DetectorNorm"]["data"];
    const As::RealVector time = (*scan)["conditions"]["Time/step"]["data"];
    if (!detector.isEmpty()) {
        int from, to;
        qreal bkg = 0.;
        // Calculate the sum of the left background intensities
        from = scan->m_numLeftSkipPoints;
        to   = from + scan->m_numLeftBkgPoints;
        for (int i = from; i < to; ++i)
            bkg += detector[i];
        // Calculate the sum of the right background intensities
        to   = scan->m_numPoints - scan->m_numRightSkipPoints;
        from = to - scan->m_numRightBkgPoints;
        for (int i = from; i < to; ++i)
            bkg += detector[i];
        // Calculate the mean background
        scan->m_normMeanBkg = bkg / (scan->m_numLeftBkgPoints + scan->m_numRightBkgPoints);
        // Create a background array and array with background estimated standard deviations (ESD)
        As::RealVector vectorBkg;
        for (int i = 0; i < time.size(); ++i) {
            vectorBkg.append(scan->m_normMeanBkg * time[i]); }
        As::RealVector vectorBkgErr(vectorBkg.sqrt());
        // Normalize background and ESD arrays by time
        As::RealVector vectorBkgNorm(vectorBkg.normalizeBy(time));
        As::RealVector vectorBkgErrNorm(vectorBkgErr.normalizeBy(time));
        // Set data for the auto appearance in the output table
        scan->setData("calculations", "BkgNorm",  vectorBkgNorm.toQString());
        scan->setData("calculations", "BkgNormErr", vectorBkgErrNorm.toQString()); } }

/*!
Returns the ratio of standard deviation to integrated intensity according
to the given \a intensities, \a sigmas, \a numLeftBkgPoints, \a numRightBkgPoints,
\a numLeftSkipPoints, \a numRightSkipPoints and \a mcCandlishFactor.
*/
As::RealVector As::ScanArray::IntensityWithSigma(const As::RealVector &intensities,
                                                 const As::RealVector &sigmas,
                                                 const int numLeftBkgPoints,
                                                 const int numRightBkgPoints,
                                                 const int numLeftSkipPoints,
                                                 const int numRightSkipPoints,
                                                 const qreal mcCandlishFactor)
{
    // BKG - Background : http://www.acronymfinder.com/BKG.html
    // Calculate number of points
    const int numPoints = intensities.size();
    // Set intensities for the left + right background points: 0.8 sec (735 peaks)
    As::RealVector intyBkg, sigIntyBkg;
    const int posLeft     = numLeftSkipPoints;
    const int lengthLeft  = numLeftBkgPoints;
    const int posRight    = numPoints - numRightBkgPoints - numRightSkipPoints;
    const int lengthRight = numRightBkgPoints;
    intyBkg  = intensities.mid(posLeft, lengthLeft) + intensities.mid(posRight, lengthRight);
    //////ADEBUG << "intyBkg" << intyBkg;

    sigIntyBkg = sigmas.mid(posLeft, lengthLeft) + sigmas.mid(posRight, lengthRight);
    // Set intensities for the peak points (background is included): 0.4 sec (735 peaks)
    As::RealVector intyPeak, sigIntyPeak;
    const int pos     = numLeftSkipPoints + numLeftBkgPoints;
    const int length  = numPoints - numLeftSkipPoints - numLeftBkgPoints - numRightSkipPoints - numRightBkgPoints;
    intyPeak  = intensities.mid(pos, length);
    sigIntyPeak = sigmas.mid(pos, length);
    // Calculate ratio of peak points number to background points number
    const qreal ratioPeakToBkgPoints = (qreal)intyPeak.size() / intyBkg.size();
    /////ADEBUG << "ratioPeakToBkgPoints" << ratioPeakToBkgPoints;
    // Calculate peak sum intensity: 0.6 sec (735 peaks)
    qreal intensity = intyPeak.sum() - intyBkg.sum() * ratioPeakToBkgPoints;
    /////ADEBUG << "intensity" << intensity;
    // Calculate sigma (standard deviation) for the peak sum intensity: 0.5 sec (735 peaks)
    qreal sigma = qSqrt(sigIntyPeak.sumSqr() +
                  sigIntyBkg.sumSqr() * As::Sqr(ratioPeakToBkgPoints) +
                  As::Sqr(mcCandlishFactor * intensity));
    /////ADEBUG << "sigma" << sigma;
    return As::RealVector(QVector<qreal>{intensity, sigma});
}

/*!
Calculates the maximum peak intensity of the given \a scan.
*/
void As::ScanArray::calcMaxPeakInty(As::Scan *scan)
{
    for (const QString &countType : As::COUNT_TYPES) {
        const As::RealVector detector  = (*scan)["intensities"]["DetectorNorm" + countType]["data"];
        const As::RealVector sdetector = (*scan)["intensities"]["sDetectorNorm" + countType]["data"];
        if (!detector.isEmpty()) {
            scan->m_maxPeakInty[countType]    = detector.max();
            scan->m_maxPeakIntyErr[countType] = sdetector.max();
            // Set data for the auto appearance in the output table
            scan->setData("calculations", "IntMax" + countType, QString::number(scan->m_maxPeakInty[countType]));
            scan->setData("calculations", "IntMaxErr" + countType, QString::number(scan->m_maxPeakIntyErr[countType])); } }
}

/*!
Calculates the sum of all the peak point intensities of the given \a scan.
*/
void As::ScanArray::calcSumPeakInty(As::Scan *scan)
{
    for (const QString &countType : As::COUNT_TYPES) {
        const As::RealVector detector  = (*scan)["intensities"]["DetectorNorm" + countType]["data"];
        const As::RealVector sdetector = (*scan)["intensities"]["sDetectorNorm" + countType]["data"];
        if (!detector.isEmpty()) {
            As::RealVector intyWithSig =
                    IntensityWithSigma(detector,
                                       sdetector,
                                       scan->m_numLeftBkgPoints,
                                       scan->m_numRightBkgPoints,
                                       scan->m_numLeftSkipPoints,
                                       scan->m_numRightSkipPoints,
                                       scan->m_mcCandlishFactor);
            scan->m_sumPeakInty[countType]    = intyWithSig[0];
            scan->m_sumPeakIntyErr[countType] = intyWithSig[1];
            // Set data for the auto appearance in the output table
            scan->setData("calculations", "IntSum" + countType, QString::number(scan->m_sumPeakInty[countType]));
            scan->setData("calculations", "IntSumErr" + countType, QString::number(scan->m_sumPeakIntyErr[countType])); } }
}

/*!
Calculates the peak area of the given \a scan.
*/
void As::ScanArray::calcPeakArea(As::Scan *scan)
{
    const As::RealVector angle = (*scan)["angles"][scan->scanAngle()]["data"];
    const qreal step = angle.step();
    for (const QString &countType : As::COUNT_TYPES) {
        if (!qIsNaN(scan->m_sumPeakInty[countType])) {
            scan->m_peakArea[countType]    = scan->m_sumPeakInty[countType]    * step;
            scan->m_peakAreaErr[countType] = scan->m_sumPeakIntyErr[countType] * step;
            // Set data for the auto appearance in the output table
            scan->setData("calculations", "Area" + countType, QString::number(scan->m_peakArea[countType]));
            scan->setData("calculations", "AreaErr" + countType, QString::number(scan->m_peakAreaErr[countType])); } }
}

/*!
Calculates the normalised peak area of the given \a scan.
*/
void As::ScanArray::calcNormPeakArea(As::Scan *scan)
{
    const As::RealVector monitor = (*scan)["intensities"]["MonitorNorm"]["data"];
    //if (!monitor.isEmpty()) {
        qreal monitorMean = monitor.mean();
        if (monitorMean == 0.)
            monitorMean = MONITOR_NORM;
        const qreal normalizer = MONITOR_NORM / monitorMean;
        for (const QString &countType : As::COUNT_TYPES) {
            if (!qIsNaN(scan->m_peakArea[countType])) {
                scan->m_normPeakArea[countType]    = scan->m_peakArea[countType]    * normalizer;
                scan->m_normPeakAreaErr[countType] = scan->m_peakAreaErr[countType] * normalizer;
                // Set data for the auto appearance in the output table
                scan->setData("calculations", "AreaNorm" + countType, QString::number(scan->m_normPeakArea[countType]));
                scan->setData("calculations", "AreaNormErr" + countType, QString::number(scan->m_normPeakAreaErr[countType])); } }
} //}

/*!
Returns the Lorentz correction coefficient calculated from the given
detector 2theta angle \a gammaMean and lifting counter nu anlge \a nuMean.
*/
qreal As::ScanArray::lorentzCorrectionFactor(const qreal gammaMean, const qreal nuMean)
{
    return qSin(qDegreesToRadians(gammaMean)) * qCos(qDegreesToRadians(nuMean));
}

/*!
Calculates the structure factor area of the given \a scan.
*/
void As::ScanArray::calcStructFactor(As::Scan *scan)
{
    const As::RealVector twotheta = (*scan)["angles"]["2Theta"]["data"];
    const As::RealVector gamma    = (*scan)["angles"]["Gamma"]["data"];
    const As::RealVector nu       = (*scan)["angles"]["Nu"]["data"];
    qreal correction = qQNaN();
    if (!twotheta.isEmpty())
        correction = lorentzCorrectionFactor(twotheta.mean());
    else if (!gamma.isEmpty() AND nu.isEmpty())         /// check!!! not very sophisticated
        correction = lorentzCorrectionFactor(gamma.mean());
    else if (!gamma.isEmpty() AND !nu.isEmpty())
        correction = lorentzCorrectionFactor(gamma.mean(), nu.mean());
    else
        qFatal("%s: unknown Lorentz correction input", __FUNCTION__);
    for (const QString &countType : As::COUNT_TYPES) {
        if (!qIsNaN(scan->m_normPeakArea[countType])) {
            scan->m_structFactor[countType]    = scan->m_normPeakArea[countType]    * correction;
            scan->m_structFactorErr[countType] = scan->m_normPeakAreaErr[countType] * correction;
            // Set data for the auto appearance in the output table
            scan->setData("calculations", "Sf2" + countType, QString::number(scan->m_structFactor[countType]));
            scan->setData("calculations", "Sf2Err" + countType, QString::number(scan->m_structFactorErr[countType])); } }
}

/*!
Calculates the full width at half maximum (FWHM) of the given \a scan.
*/
void As::ScanArray::calcFullWidthHalfMax(As::Scan *scan)
{
    // Read measred data
    const As::RealVector x     = (*scan)["angles"][scan->scanAngle()]["data"];
    const As::RealVector inty  = (*scan)["intensities"]["DetectorNorm"]["data"];
    const As::RealVector sInty = (*scan)["intensities"]["sDetectorNorm"]["data"];
    const As::RealVector bkg   = (*scan)["calculations"]["BkgNorm"]["data"];
    const As::RealVector sBkg  = (*scan)["calculations"]["BkgNormErr"]["data"];
    // Calc intensity and its standard deviation substracting background from total measured detector intensity
    As::RealVector y, sy;
    for (int i = 0; i < inty.size(); ++i) {
        y.append(inty[i] - bkg[i]);
        sy.append(qSqrt(As::Sqr(sInty[i]) + As::Sqr(sBkg[i]))); }
    // Define local variables
    const qreal yHM            = 0.5 * y.max();    // intensity at half maximum (HM)
    const qreal iLast          = scan->m_numPoints - 1;
    const int iLeftPeakBorder  = scan->m_numLeftSkipPoints + scan->m_numLeftBkgPoints;
    const int iRightPeakBorder = scan->m_numPoints - scan->m_numRightSkipPoints - scan->m_numRightBkgPoints - 1;
    // Find position of maximum within the peak range
    As::RealVector yPeak;
    for (int i = iLeftPeakBorder; i <= iRightPeakBorder; ++i) {
        yPeak.append(y[i]); }
    const int iMaxOfPeak  = iLeftPeakBorder + yPeak.indexOfMax();
    // Find indices of two points: one is just before half max and one is just after half max
    // (for left part of the peak)
    int iAfterLeftHalfMax = iMaxOfPeak;
    for (int i = iLeftPeakBorder; i < iMaxOfPeak; ++i) {
        if (y[i] > yHM) {
            iAfterLeftHalfMax = i;
            break; } }
    const int iBeforeLeftHalfMax = (iAfterLeftHalfMax == 0) ? 0 : (iAfterLeftHalfMax - 1);
    // (for right part of the peak)
    int iBeforeRightHalfMax = iMaxOfPeak;
    for (int i = iRightPeakBorder; i > iMaxOfPeak; --i) {
        if (y[i] > yHM) {
            iBeforeRightHalfMax = i;
            break; } }
    const int iAfterRightHalfMax = (iBeforeRightHalfMax == iLast) ? iLast : (iBeforeRightHalfMax + 1);
    // Error in the instrumental angle positioning in degrees
    const qreal sx = 0.01;
    // Create line connecting to points: one is just before half max and one is just after half max
    // (for left part of the peak)
    const qreal xL1 = x[iBeforeLeftHalfMax]; const qreal sxL1 = sx;
    const qreal yL1 = y[iBeforeLeftHalfMax]; const qreal syL1 = sy[iBeforeLeftHalfMax];
    const qreal xL2 = x[iAfterLeftHalfMax];  const qreal sxL2 = sx;
    const qreal yL2 = y[iAfterLeftHalfMax];  const qreal syL2 = sy[iAfterLeftHalfMax];
    const As::Line left(xL1, yL1, xL2, yL2, sxL1, syL1, sxL2, syL2);
    // (for right part of the peak)
    const qreal xR1 = x[iBeforeRightHalfMax]; const qreal sxR1 = sx;
    const qreal yR1 = y[iBeforeRightHalfMax]; const qreal syR1 = sy[iBeforeRightHalfMax];
    const qreal xR2 = x[iAfterRightHalfMax];  const qreal sxR2 = sx;
    const qreal yR2 = y[iAfterRightHalfMax];  const qreal syR2 = sy[iAfterRightHalfMax];
    const As::Line right(xR1, yR1, xR2, yR2, sxR1, syR1, sxR2, syR2);
    // Full width at half maximum (Fwhm) and its estimated standard deviation
    scan->m_fullWidthHalfMax    = qAbs(right.xForY(yHM) - left.xForY(yHM));
    scan->m_fullWidthHalfMaxErr = qSqrt(As::Sqr(left.esdXForY(yHM, left.esdYForY(yHM))) +
                                        As::Sqr(right.esdXForY(yHM, right.esdYForY(yHM))));
    // Set data for the auto appearance in the output table
    scan->setData("calculations", "Fwhm",  QString::number(scan->m_fullWidthHalfMax));
    scan->setData("calculations", "FwhmErr", QString::number(scan->m_fullWidthHalfMaxErr));
}

/*!
Calculates the flipping ratio of the given \a scan.
*/
void As::ScanArray::calcFlippingRatio(As::Scan *scan)
{
    // Check if all the countTypes are defined
    for (const QString &countType : As::COUNT_TYPES)
        if (qIsNaN(scan->m_structFactor[countType]))
            return;
    // Get structure factors of the up(-) and down(-) polarised data
    const qreal plus     = scan->m_structFactor[As::COUNT_TYPES[1]];
    const qreal minus    = scan->m_structFactor[As::COUNT_TYPES[2]];
    const qreal sigPlus  = scan->m_structFactorErr[As::COUNT_TYPES[1]];
    const qreal sigMinus = scan->m_structFactorErr[As::COUNT_TYPES[2]];
    // Calc flipping ratio
    scan->m_flippingRatio    = plus / minus;
    scan->m_flippingRatioErr = qSqrt(As::Sqr(1 / minus * sigPlus) +
                                     As::Sqr(-plus / As::Sqr(minus) * sigMinus));
    qreal significance = qAbs(scan->m_flippingRatio - 1) / scan->m_flippingRatioErr;
    if (scan->m_flippingRatio < 0)
        significance *= -1;
    // Set data for the auto appearance in the output table
    scan->setData("calculations", "FR",           QString::number(scan->m_flippingRatio));
    scan->setData("calculations", "FRerr",        QString::number(scan->m_flippingRatioErr));
    scan->setData("calculations", "|FR-1|/FRerr", QString::number(significance));
}

/*!
\fn void As::ScanArray::facilityTypeChanged_Signal(const QString &type)

This signal is emitted whenever the type of the facility in the input data
is changed. The new value is passed in \a type.
*/

/*!
\fn void As::ScanArray::instrumentTypeChanged_Signal(const QString &type)

This signal is emitted whenever the type of the instrument in the input data
is changed. The new value is passed in \a type.
*/

/*!
\fn void As::ScanArray::dataTypeChanged_Signal(const QString &type)

This signal is emitted whenever the type of the input data is changed.
The new value is passed in \a type.
*/

/*!
\fn void As::ScanArray::scanIndexChanged(const int index)

This signal is emitted whenever the index of the current scan is changed.
The new value is passed in \a index.
*/

/*!
\fn void As::ScanArray::fileIndexChanged(const int index)

This signal is emitted whenever the file index of the current scan is changed.
The new value is passed in \a index.
*/
