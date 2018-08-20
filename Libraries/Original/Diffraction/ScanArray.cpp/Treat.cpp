#include <QDateTime>
#include <QtConcurrent>
#include <QtMath>

#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "Line.hpp"
#include "RealVector.hpp"
#include "Scan.hpp"

#include "ScanArray.hpp"

/*!
    Treats the data preliminary for the single scan at \a index in the scan array.
*/
void As::ScanArray::preTreatSinglePeak(const int index) {
    auto scan = at(index);

    scan->setData("number", "Excluded", "0"); // Init. It's modified then in excludeScanSlot
    definePolarisationCrossSection(scan);
    calcEsd(scan);
    normalizeByTime(scan); }

/*!
    Treats the data for the single scan at \a index in the scan array.
*/
void As::ScanArray::treatSinglePeak(const int index) {
    auto scan = at(index);

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

    if (scan->plotType() != As::PlotType::Excluded) {
        scan->setPlotType(As::PlotType::Integrated); } }

/*!
    Creates the full output table with all the processed parameters.
*/
void As::ScanArray::createFullOutputTable() {
    ADEBUG;

    // Set the groups to be shown in the output table. Their order is preserved.
    // Group elements (subitems) are sorted automatically by QMap in alphabetic order
    // of their string keys
    const QStringList itemKeys = {"number", "indices", "calculations",
                                  "angles", "cosines", "conditions" };

    // Make a map (headerMap) and list (m_outputTableHeaders) of the actually measured headers
    // from the 1st scan: m_scanArray.at(0)
    m_outputTableHeaders = QStringList();
    QMap<QString, QStringList> headerMap;
    for (const auto& itemKey : itemKeys) {
        const QStringList subitemKeys = (*m_scanArray.at(0))[itemKey].keys();
        m_outputTableHeaders.append(subitemKeys);
        headerMap.insert(itemKey, subitemKeys); }

    // Make a table of the calculated values according to the headerMap for all the peaks
    m_outputTableData = QList<QStringList>();
    for (const auto scan : m_scanArray) {
        QStringList dataRow;
        int j = 0;
        for (const auto& itemKey : itemKeys)
            for (const auto& subitemKey : headerMap[itemKey]) {
                dataRow.insert(j++, scan->printDataSingle(itemKey, subitemKey)); }
        m_outputTableData << dataRow; }

    ADEBUG; }

/*!
    Defines the polarisation cross-section for the given \a scan.
*/
void As::ScanArray::definePolarisationCrossSection(As::Scan* scan) {

    // Read polarisation and flipper parameters
    QString fin  = scan->data("polarisation", "Fin" ).split(" ")[0];
    QString fout = scan->data("polarisation", "Fout").split(" ")[0];
    QString pin  = scan->data("polarisation", "Pin" ).split(" ")[0];
    QString pout = scan->data("polarisation", "Pout").split(" ")[0];

    // Adjust polarisation parameters depends on the flipper parameters
    if (fin == "on") {
        if (pin.startsWith("+")) {
            pin.replace("+", "-"); }
        else {
            pin.replace("-", "+"); } }
    if (fout == "on") {
        if (pout.startsWith("+")) {
            pout.replace("+", "-"); }
        else {
            pout.replace("-", "+"); } }

    // Set the measured polarisation cross-ection parameter
    if (!(pin + pout).isEmpty()) {
        scan->setData("conditions", "Polarisation (in/out)", pin + pout); } // what if we work in a half polarized mode?
}

/*!
    Calculates the estimated standard deviations for the measured detector and
    monitor data arrays of the given \a scan.
*/
void As::ScanArray::calcEsd(As::Scan* scan) {
    for (const QString& countType : As::ScanDict::BEAM_TYPES.values()) {

        const As::RealVector detector = scan->data("intensities", "Detector" + countType);
        const As::RealVector monitor  = scan->data("intensities", "Monitor" + countType);

        if (!detector.isEmpty()) {
            scan->setData("intensities", "sDetector" + countType, detector.sqrt().toQString()); }
        if (!monitor.isEmpty()) {
            scan->setData("intensities", "sMonitor" + countType,  monitor.sqrt().toQString()); } } }

/*!
    Normalizes the measured detector and monitor data arrays of the given \a scan by the measured time.
*/
void As::ScanArray::normalizeByTime(As::Scan* scan) {
    for (const QString& countType : As::ScanDict::BEAM_TYPES.values()) {

        const As::RealVector detector  = scan->data("intensities", "Detector" + countType);
        const As::RealVector sdetector = scan->data("intensities", "sDetector" + countType);
        const As::RealVector monitor   = scan->data("intensities", "Monitor" + countType);
        const As::RealVector smonitor  = scan->data("intensities", "sMonitor" + countType);
        const As::RealVector time      = scan->data("conditions", "Time/step" + countType);

        if (!detector.isEmpty()) {
            scan->setData("intensities", "DetectorNorm" + countType,  detector.normalizeBy(time).toQString()); }
        if (!sdetector.isEmpty()) {
            scan->setData("intensities", "sDetectorNorm" + countType, sdetector.normalizeBy(time).toQString()); }
        if (!monitor.isEmpty()) {
            scan->setData("intensities", "MonitorNorm" + countType,   monitor.normalizeBy(time).toQString()); }
        if (!smonitor.isEmpty()) {
            scan->setData("intensities", "sMonitorNorm" + countType,  smonitor.normalizeBy(time).toQString()); } } }

/*!
    Finds non-peak points (neighbors to be skipped + background) of
    the given \a scan.
*/
void As::ScanArray::findNonPeakPoints(As::Scan* scan) {
    //ADEBUG;

    const As::RealVector detector  = scan->data("intensities", "DetectorNorm");
    const As::RealVector sdetector = scan->data("intensities", "sDetectorNorm");

    qreal minRatio = qInf();

    const bool autoSkip = (scan->neighborsRemoveType() == As::Scan::AutoNeighborsRemove);
    const bool autoBkg = (scan->bkgDetectType() == As::Scan::AutoBkgDetect);

    // Automatically detect background and skip points
    if (autoBkg AND autoSkip) {

        // Find Bkg points
        const int fromLS = As::ScanDict::MIN_BKG_DATA_POINTS;
        const int toLS   = scan->numPoints() - As::ScanDict::MIN_BKG_DATA_POINTS;
        for (int numLeftBkgPoints = fromLS; numLeftBkgPoints < toLS; ++numLeftBkgPoints) {
            const int fromRS = As::ScanDict::MIN_BKG_DATA_POINTS;
            const int toRS   = scan->numPoints() - numLeftBkgPoints;
            for (int numRightBkgPoints = fromRS; numRightBkgPoints < toRS; ++numRightBkgPoints) {
                const As::RealVector intyWithSig = IntensityWithSigma(detector, sdetector,
                                                                      numLeftBkgPoints, numRightBkgPoints,
                                                                      0, 0,
                                                                      scan->mcCandlishFactor());
                const qreal ratio = intyWithSig[1] / intyWithSig[0];
                if (ratio > 0 AND ratio < minRatio) {
                    minRatio = ratio;
                    scan->m_numLeftBkgPoints = numLeftBkgPoints;
                    scan->m_numRightBkgPoints = numRightBkgPoints; } } }

        // Find Skip points
        const int fromLB = As::ScanDict::MIN_SKIP_DATA_POINTS;
        const int toLB   = scan->m_numLeftBkgPoints - As::ScanDict::MIN_BKG_DATA_POINTS;
        for (int numLeftSkipPoints = fromLB; numLeftSkipPoints < toLB; ++numLeftSkipPoints) {
            const int numLeftBkgPoints = scan->m_numLeftBkgPoints - numLeftSkipPoints;
            const int fromRB = As::ScanDict::MIN_SKIP_DATA_POINTS;
            const int toRB   = scan->m_numRightBkgPoints - As::ScanDict::MIN_BKG_DATA_POINTS;
            for (int numRightSkipPoints = fromRB; numRightSkipPoints < toRB; ++numRightSkipPoints) {
                const int numRightBkgPoints = scan->m_numRightBkgPoints - numRightSkipPoints;
                const As::RealVector intyWithSig = IntensityWithSigma(detector, sdetector,
                                                                      numLeftBkgPoints, numRightBkgPoints,
                                                                      numLeftSkipPoints, numRightSkipPoints,
                                                                      scan->mcCandlishFactor());
                const qreal ratio = intyWithSig[1] / intyWithSig[0];
                //ADEBUG << ratio << minRatio << numLeftBkgPoints << numRightBkgPoints;
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
        const int numNonSkipPoints = scan->numPoints() - scan->m_numLeftSkipPoints - scan->m_numRightSkipPoints;
        const int fromLB = As::ScanDict::MIN_BKG_DATA_POINTS;
        const int toLB   = numNonSkipPoints - As::ScanDict::MIN_BKG_DATA_POINTS;
        for (int numLeftBkgPoints = fromLB; numLeftBkgPoints < toLB; ++numLeftBkgPoints) {
            const int fromRB = As::ScanDict::MIN_BKG_DATA_POINTS;
            const int toRB   = numNonSkipPoints - numLeftBkgPoints;
            for (int numRightBkgPoints = fromRB; numRightBkgPoints < toRB; ++numRightBkgPoints) {
                const As::RealVector intyWithSig = IntensityWithSigma(detector, sdetector,
                                                                      numLeftBkgPoints, numRightBkgPoints,
                                                                      scan->m_numLeftSkipPoints, scan->m_numRightSkipPoints,
                                                                      scan->mcCandlishFactor());
                const qreal ratio = intyWithSig[1] / intyWithSig[0];
                //ADEBUG << ratio << minRatio << numLeftBkgPoints << numRightBkgPoints;
                if (ratio > 0 AND ratio < minRatio) {
                    minRatio = ratio;
                    scan->m_numLeftBkgPoints = numLeftBkgPoints;
                    scan->m_numRightBkgPoints = numRightBkgPoints; } } } }

    // Manually set background and automatically detect skip points
    else if (!autoBkg AND autoSkip) {
        const int numNonBkgPoints = scan->numPoints() - scan->m_numLeftBkgPoints - scan->m_numRightBkgPoints;
        const int fromLS = As::ScanDict::MIN_SKIP_DATA_POINTS;
        const int toLS   = numNonBkgPoints - As::ScanDict::MIN_SKIP_DATA_POINTS;
        for (int numLeftSkipPoints = fromLS; numLeftSkipPoints < toLS; ++numLeftSkipPoints) {
            const int fromRS = As::ScanDict::MIN_SKIP_DATA_POINTS;
            const int toRS   = numNonBkgPoints - numLeftSkipPoints - As::ScanDict::MIN_SKIP_DATA_POINTS;
            for (int numRightSkipPoints = fromRS; numRightSkipPoints < toRS; ++numRightSkipPoints) {
                const As::RealVector intyWithSig = IntensityWithSigma(detector, sdetector,
                                                                      scan->m_numLeftBkgPoints, scan->m_numRightBkgPoints,
                                                                      numLeftSkipPoints, numRightSkipPoints,
                                                                      scan->mcCandlishFactor());
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
                           scan->mcCandlishFactor()); }

    // Update points
    scan->m_numNonSkipPoints = scan->numPoints() - scan->m_numLeftSkipPoints - scan->m_numRightSkipPoints;
    scan->m_numPeakPoints = scan->m_numNonSkipPoints - scan->m_numLeftBkgPoints - scan->m_numRightBkgPoints; }

/*!
    Adjusts the background points of the given \a scan.
*/
void As::ScanArray::adjustBkgPoints(As::Scan* scan) {
    if (scan->bkgDetectType() == As::Scan::AutoBkgDetect) {
        for (int i = 0; i < As::ScanDict::EXTRA_PEAK_DATA_POINTS; ++i) {
            // left
            if (scan->m_numLeftBkgPoints > 1) {
                --scan->m_numLeftBkgPoints;
                ++scan->m_numPeakPoints; }
            // right
            if (scan->m_numRightBkgPoints > 1) {
                --scan->m_numRightBkgPoints;
                ++scan->m_numPeakPoints; } } } }

/*!
    Calculates the average background of the given \a scan.
*/
void As::ScanArray::calcBkg(As::Scan* scan) {
    //ADEBUG;

    const As::RealVector detector = scan->data("intensities", "DetectorNorm");
    const As::RealVector time = scan->data("conditions", "Time/step");

    if (detector.isEmpty()) {
        return; }

    int from, to;
    qreal bkg = 0.0;

    // Calculate the sum of the left background intensities
    from = scan->m_numLeftSkipPoints;
    to   = from + scan->m_numLeftBkgPoints;
    for (int i = from; i < to; ++i) {
        bkg += detector[i]; }

    // Calculate the sum of the right background intensities
    to   = scan->numPoints() - scan->m_numRightSkipPoints;
    from = to - scan->m_numRightBkgPoints;
    for (int i = from; i < to; ++i) {
        bkg += detector[i]; }

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
    scan->setData("calculations", "BkgNormErr", vectorBkgErrNorm.toQString()); }

/*!
    Returns the ratio of standard deviation to integrated intensity according
    to the given \a intensities, \a sigmas, \a numLeftBkgPoints, \a numRightBkgPoints,
    \a numLeftSkipPoints, \a numRightSkipPoints and \a mcCandlishFactor.
*/
As::RealVector As::ScanArray::IntensityWithSigma(const As::RealVector& intensities,
                                                 const As::RealVector& sigmas,
                                                 const int numLeftBkgPoints,
                                                 const int numRightBkgPoints,
                                                 const int numLeftSkipPoints,
                                                 const int numRightSkipPoints,
                                                 const qreal mcCandlishFactor) {
    // Calculate number of points
    const int numPoints = intensities.size();

    // Set intensities for the left + right background points: 0.8 sec (735 peaks)
    const int posLeft     = numLeftSkipPoints;
    const int lengthLeft  = numLeftBkgPoints;
    const int posRight    = numPoints - numRightBkgPoints - numRightSkipPoints;
    const int lengthRight = numRightBkgPoints;
    As::RealVector intyBkg, sigIntyBkg;
    intyBkg    = intensities.mid(posLeft, lengthLeft) + intensities.mid(posRight, lengthRight);
    sigIntyBkg = sigmas.mid(posLeft, lengthLeft) + sigmas.mid(posRight, lengthRight);

    // Set intensities for the peak points (background is included): 0.4 sec (735 peaks)
    const int pos     = numLeftSkipPoints + numLeftBkgPoints;
    const int length  = numPoints - numLeftSkipPoints - numLeftBkgPoints - numRightSkipPoints - numRightBkgPoints;
    As::RealVector intyPeak, sigIntyPeak;
    intyPeak  = intensities.mid(pos, length);
    sigIntyPeak = sigmas.mid(pos, length);

    // Calculate ratio of peak points number to background points number
    const qreal ratioPeakToBkgPoints = static_cast<qreal>(intyPeak.size()) / intyBkg.size();

    // Calculate peak sum intensity: 0.6 sec (735 peaks)
    qreal intensity = intyPeak.sum() - intyBkg.sum() * ratioPeakToBkgPoints;

    // Calculate sigma (standard deviation) for the peak sum intensity: 0.5 sec (735 peaks)
    qreal sigma = qSqrt(sigIntyPeak.sumSqr() +
                        sigIntyBkg.sumSqr() * As::Sqr(ratioPeakToBkgPoints) +
                        As::Sqr(mcCandlishFactor * intensity));

    return As::RealVector(QVector<qreal> {intensity, sigma }); }

/*!
    Calculates the maximum peak intensity of the given \a scan.
*/
void As::ScanArray::calcMaxPeakInty(As::Scan* scan) {
    for (const QString& countType : As::ScanDict::BEAM_TYPES.values()) {
        const As::RealVector detector  = scan->data("intensities", "DetectorNorm" + countType);
        const As::RealVector sdetector = scan->data("intensities", "sDetectorNorm" + countType);

        if (detector.isEmpty()) {
            continue; }

        scan->m_maxPeakInty[countType]    = detector.max();
        scan->m_maxPeakIntyErr[countType] = sdetector.max();

        // Set data for the auto appearance in the output table
        scan->setData("calculations", "IntMax" + countType, QString::number(scan->m_maxPeakInty[countType]));
        scan->setData("calculations", "IntMaxErr" + countType, QString::number(scan->m_maxPeakIntyErr[countType])); } }

/*!
    Calculates the sum of all the peak point intensities of the given \a scan.
*/
void As::ScanArray::calcSumPeakInty(As::Scan* scan) {
    for (const QString& countType : As::ScanDict::BEAM_TYPES.values()) {
        const As::RealVector detector  = scan->data("intensities", "DetectorNorm" + countType);
        const As::RealVector sdetector = scan->data("intensities", "sDetectorNorm" + countType);

        if (detector.isEmpty()) {
            continue; }

        As::RealVector intyWithSig = IntensityWithSigma(detector,
                                                        sdetector,
                                                        scan->m_numLeftBkgPoints,
                                                        scan->m_numRightBkgPoints,
                                                        scan->m_numLeftSkipPoints,
                                                        scan->m_numRightSkipPoints,
                                                        scan->mcCandlishFactor());
        scan->m_sumPeakInty[countType]    = intyWithSig[0];
        scan->m_sumPeakIntyErr[countType] = intyWithSig[1];

        // Set data for the auto appearance in the output table
        scan->setData("calculations", "IntSum" + countType, QString::number(scan->m_sumPeakInty[countType]));
        scan->setData("calculations", "IntSumErr" + countType, QString::number(scan->m_sumPeakIntyErr[countType])); } }

/*!
    Calculates the peak area of the given \a scan.
*/
void As::ScanArray::calcPeakArea(As::Scan* scan) {
    const As::RealVector angle = scan->data("angles", scan->scanAngle());
    const qreal step = angle.step();

    for (const QString& countType : As::ScanDict::BEAM_TYPES.values()) {
        if (!qIsNaN(scan->m_sumPeakInty[countType])) {
            scan->m_peakArea[countType]    = scan->m_sumPeakInty[countType]    * step;
            scan->m_peakAreaErr[countType] = scan->m_sumPeakIntyErr[countType] * step;
            // Set data for the auto appearance in the output table
            scan->setData("calculations", "Area" + countType, QString::number(scan->m_peakArea[countType]));
            scan->setData("calculations", "AreaErr" + countType, QString::number(scan->m_peakAreaErr[countType])); } } }

/*!
    Calculates the normalised peak area of the given \a scan.
*/
void As::ScanArray::calcNormPeakArea(As::Scan* scan) {
    const As::RealVector monitor = scan->data("intensities", "MonitorNorm");
    //if (!monitor.isEmpty()) {

    qreal monitorMean = monitor.mean();

    if (monitorMean == 0.) {
        monitorMean = As::ScanDict::DEFAULT_MONITOR; }

    const qreal normalizer = As::ScanDict::DEFAULT_MONITOR / monitorMean;

    for (const QString& countType : As::ScanDict::BEAM_TYPES.values()) {
        if (!qIsNaN(scan->m_peakArea[countType])) {
            scan->m_normPeakArea[countType]    = scan->m_peakArea[countType]    * normalizer;
            scan->m_normPeakAreaErr[countType] = scan->m_peakAreaErr[countType] * normalizer;
            // Set data for the auto appearance in the output table
            scan->setData("calculations", "AreaNorm" + countType, QString::number(scan->m_normPeakArea[countType]));
            scan->setData("calculations", "AreaNormErr" + countType, QString::number(scan->m_normPeakAreaErr[countType])); } }
    //}
}

/*!
    Returns the Lorentz correction coefficient calculated from the given
    detector 2theta angle \a gammaMean and lifting counter nu anlge \a nuMean.
*/
qreal As::ScanArray::lorentzCorrectionFactor(const qreal gammaMean, const qreal nuMean) {
    return qSin(qDegreesToRadians(gammaMean)) * qCos(qDegreesToRadians(nuMean)); }

/*!
    Calculates the structure factor of the given \a scan.
*/
void As::ScanArray::calcStructFactor(As::Scan* scan) {
    const As::RealVector twotheta = scan->data("angles", "2Theta");
    const As::RealVector gamma    = scan->data("angles", "Gamma");
    const As::RealVector nu       = scan->data("angles", "Nu");
    qreal correction = qQNaN();

    if (!twotheta.isEmpty()) {
        correction = lorentzCorrectionFactor(twotheta.mean()); }

    else if (!gamma.isEmpty() AND nu.isEmpty()) {           // find a better way!
        correction = lorentzCorrectionFactor(gamma.mean()); }

    else if (!gamma.isEmpty() AND !nu.isEmpty()) {
        correction = lorentzCorrectionFactor(gamma.mean(), nu.mean()); }

    //else
    //    qFatal("%s: unknown Lorentz correction input", __FUNCTION__);

    for (const QString& countType : As::ScanDict::BEAM_TYPES.values()) {
        if (!qIsNaN(scan->m_normPeakArea[countType])) {
            scan->m_structFactor[countType]    = scan->m_normPeakArea[countType]    * correction;
            scan->m_structFactorErr[countType] = scan->m_normPeakAreaErr[countType] * correction;
            // Set data for the auto appearance in the output table
            scan->setData("calculations", "Sf2" + countType, QString::number(scan->m_structFactor[countType]));
            scan->setData("calculations", "Sf2Err" + countType, QString::number(scan->m_structFactorErr[countType])); } } }

/*!
    Calculates the full width at half maximum (FWHM) of the given \a scan.
*/
void As::ScanArray::calcFullWidthHalfMax(As::Scan* scan) {
    // Read measred data
    const As::RealVector x     = scan->data("angles",       scan->scanAngle());
    const As::RealVector inty  = scan->data("intensities",  "DetectorNorm");
    const As::RealVector sInty = scan->data("intensities",  "sDetectorNorm");
    const As::RealVector bkg   = scan->data("calculations", "BkgNorm");
    const As::RealVector sBkg  = scan->data("calculations", "BkgNormErr");

    // Calc intensity and its standard deviation substracting background from total measured detector intensity
    As::RealVector y, sy;
    for (int i = 0; i < inty.size(); ++i) {
        y.append(inty[i] - bkg[i]);
        sy.append(qSqrt(As::Sqr(sInty[i]) + As::Sqr(sBkg[i]))); }

    // Define local variables
    const qreal yHM            = 0.5 * y.max();    // intensity at half maximum (HM)
    const qreal iLast          = scan->numPoints() - 1;
    const int iLeftPeakBorder  = scan->m_numLeftSkipPoints + scan->m_numLeftBkgPoints;
    const int iRightPeakBorder = scan->numPoints() - scan->m_numRightSkipPoints - scan->m_numRightBkgPoints - 1;

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
    scan->setData("calculations", "FwhmErr", QString::number(scan->m_fullWidthHalfMaxErr)); }

/*!
    Calculates the flipping ratio of the given \a scan.
*/
void As::ScanArray::calcFlippingRatio(As::Scan* scan) {
    // Check if all the countTypes are defined
    for (const QString& countType : As::ScanDict::BEAM_TYPES.values())
        if (qIsNaN(scan->m_structFactor[countType])) {
            return; }

    // Get structure factors of the up(-) and down(-) polarised data
    const QString typeUp   = As::ScanDict::BEAM_TYPES[As::ScanDict::POLARISED_UP];
    const QString typeDown = As::ScanDict::BEAM_TYPES[As::ScanDict::POLARISED_DOWN];
    const qreal plus     = scan->m_structFactor[typeUp];
    const qreal minus    = scan->m_structFactor[typeDown];
    const qreal sigPlus  = scan->m_structFactorErr[typeUp];
    const qreal sigMinus = scan->m_structFactorErr[typeDown];

    // Calc flipping ratio
    scan->m_flippingRatio    = plus / minus;
    scan->m_flippingRatioErr = qSqrt(As::Sqr(1 / minus * sigPlus) +
                                     As::Sqr(-plus / As::Sqr(minus) * sigMinus));
    qreal significance = qAbs(scan->m_flippingRatio - 1) / scan->m_flippingRatioErr;
    if (scan->m_flippingRatio < 0) {
        significance *= -1; }

    // Set data for the auto appearance in the output table
    scan->setData("calculations", "FR",           QString::number(scan->m_flippingRatio));
    scan->setData("calculations", "FRerr",        QString::number(scan->m_flippingRatioErr));
    scan->setData("calculations", "|FR-1|/FRerr", QString::number(significance)); }

/*!
    \fn void As::ScanArray::facilityTypeChanged(const QString &type)

    This signal is emitted whenever the type of the facility in the input data
    is changed. The new value is passed in \a type.
*/

/*!
    \fn void As::ScanArray::instrumentTypeChanged(const QString &type)

    This signal is emitted whenever the type of the instrument in the input data
    is changed. The new value is passed in \a type.
*/

/*!
    \fn void As::ScanArray::dataTypeChanged(const QString &type)

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
