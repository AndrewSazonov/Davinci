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

#include <QMouseEvent>
#include <QString>

#include "Colors.hpp"
#include "Constants.hpp"
#include "Macros.hpp"

#include "RealVector.hpp"
#include "Scan.hpp"

#include "Plot.hpp"

/*!
...
*/
As::Plot::Plot()
{
    // Define global vars
    m_debugLineWidth = 2;
    m_markSize  = 8;
    // Configure right and top axes to show ticks
    axisRect()->setupFullAxesBox();
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(xAxis, SIGNAL(rangeChanged(QCPRange)), xAxis2, SLOT(setRange(QCPRange)));
    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));
    // By default the order of layers is: 0: "background", 1: "grid", 2: "main", 3: "axes", 4: "legend"
    // Create new layer for info labels to be on top (z-order) of the plot
    //addLayer("labels");
    // Move "legend" layer above "labels" layer:
    //moveLayer(layer("legend"), layer("labels"));
    // Create new layer "labels" for info labels to be just below layer "legend"
    addLayer("labels", layer("legend"), limBelow);
    // Axes labels
    setAxesLabels();
    //updateGraphs();
    // Enlarge plot margins
    //this->axisRect()->setAutoMargins(QCP::msLeft | QCP::msTop | QCP::msRight | QCP::msBottom);
    //this->axisRect()->setAutoMargins(QCP::msAll);
    //this->axisRect()->setAutoMargins(QCP::msNone);
    //axisRect()->setMinimumMargins(QMargins(16, 16, 16, 16)); // minimum margins
    plotLayout()->setMargins(QMargins(10, 7, 7, 10));
    // Axes paddings
    xAxis->setLabelPadding(12);
    yAxis->setLabelPadding(12);
    // Get rid of the default zero lines
    xAxis->grid()->setZeroLinePen(Qt::NoPen);
    //yAxis->grid()->setZeroLinePen(Qt::NoPen);
    // Drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking
    //setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    // Slots
    // mouseMove signal that QCustomPlot emits to show point coordinates tooltip
    connect(this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(showPointCoordinatesToolTip(QMouseEvent*)));
    // Mouse events to zoom part of the plot
    connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressToZoom(QMouseEvent*)));
    connect(this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveToZoom(QMouseEvent*)));
    connect(this, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseReleaseToZoom(QMouseEvent*)));
    // Settings
    // comma as decimal separator and space as thousand separator
    //this->setLocale(QLocale(QLocale::Russian, QLocale::Russia));
    // period as decimal separator and comma as thousand separator
    setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
}

/*!
...
*/
void As::Plot::setPlotColors(const As::PlotType plotType,
                             const QString &countType)
{
    // Individual
    if (plotType == As::Raw)
        m_color = As::Color(As::green);
    else if (plotType == As::Integrated)
        m_color = As::Color(As::blue);
    else if (plotType == As::Fitted)
        m_color = As::Color(As::red);
    else if (plotType == As::Excluded)
        m_color = As::Color(As::grayLight);
    // Adjust for Up and Down polarisation data
    if (countType == "+")
        m_color = m_color.lighter(120);
    else if (countType == "-")
        m_color = m_color.darker(120);
    // Common
    m_textColor           = As::Color(As::white);
    m_textBkgColor        = m_color.transparenter(220);
    m_lineDrawColor       = m_color.lighter(120);
    m_markDrawColor       = m_color.darker(160);
    m_markFillColor       = m_color.lighter(120);
    m_errorBarsDrawColor  = m_color;
    m_areaFillColor       = m_color.transparenter(40);
}

/*!
...
*/
//void As::Plot::setAxesLabels(const QString &xLabel)
void As::Plot::setAxesLabels()
{
    //xAxis->setLabel(xLabel + "(degrees)");
    xAxis->setLabel("Scan angle (degrees)");
    yAxis->setLabel("Intensity (counts / second)");
}

/*!
...
*/
void As::Plot::updateAxesRanges(const RealVector &x,
                                const RealVector &y,
                                const RealVector &sy)
{
    qreal extra = 2 * sy.max() + 0.1 * (y.max() + 2 * sy.max());
    m_xAxisMin = x.min();
    m_xAxisMax = x.max();
    m_yAxisMin = y.min() - extra;
    m_yAxisMax = y.max() + 1.5 * extra;
    xAxis->setRange(m_xAxisMin, m_xAxisMax);
    yAxis->setRange(m_yAxisMin, m_yAxisMax);
}

/*!
...
*/
// Label with an arrow-like marker to a specific (x, y) point
void As::Plot::appendArrowInfoLabel(const As::PlotType plotType,
                                    const qreal x,
                                    const qreal y,
                                    const int labelShift,
                                    const int arrowWidth,
                                    const int arrowLength)
{
    // Set colors
    setPlotColors(plotType);
    // Point to attach the text label
    auto point = new QCPItemText(this);
    addItem(point); // allows to clear it when needed
    point->position->setCoords(x, y); // coordinates in the axes rectangle
    point->setVisible(false);
    // Text label
    auto label = new QCPItemText(this);
    addItem(label);  // allows to clear it when needed
    label->position->setParentAnchor(point->position); // set initial position equal to that of the point
    int yShift = labelShift + arrowLength;
    label->position->setCoords(0, -yShift); // additional y-move from the initial anchor
    //label->position->setType(QCPItemPosition::ptPlotCoords);
    if (yShift == 0)
        label->setPositionAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    else
        label->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    label->setClipToAxisRect(false);
    label->setPadding(QMargins(5, 3, 5, 3));
    label->setBrush(QBrush(m_textBkgColor));
    label->setColor(m_textColor);
    label->setTextAlignment(Qt::AlignHCenter);
    label->setText(QString::number(y, 'f', getNumberPrecision(y)));
    label->setLayer("labels");
    // Add the arrow-like marker
    auto arrow = new QCPItemLine(this);
    addItem(arrow); // allows to clear it when needed
    arrow->start->setParentAnchor(label->bottom);
    arrow->end->setParentAnchor(label->bottom);
    arrow->end->setCoords(0, arrowLength);
    arrow->setPen(QPen(m_textBkgColor));
    //arrow->setClipToAxisRect(true);
    // Create arrow head
    //QCPLineEnding head(QCPLineEnding::esFlatArrow, arrowWidth, arrowLength);
    arrow->setHead(QCPLineEnding(QCPLineEnding::esFlatArrow, arrowWidth, arrowLength));
    arrow->setLayer("labels");
}

/*!
...
*/
// Conventional label to appear in the center of the (x, y) point
void As::Plot::appendCentralInfoLabel(const As::PlotType plotType,
                                      const qreal x,
                                      const qreal y)
{
    int labelShift = 0;
    int arrowWidth = 0;
    int arrowLength = 0;
    appendArrowInfoLabel(plotType, x, y, labelShift, arrowWidth, arrowLength);
}

/*!
...
*/
void As::Plot::updateInfoLabels(const As::Scan *scan)
{
    As::RealVector x = (*scan)["angles"][scan->scanAngle()]["data"];
    As::RealVector y = (*scan)["intensities"]["DetectorNorm"]["data"];
    appendArrowInfoLabel(scan->plotType(), x[y.indexOfMax()], y.max());
    appendArrowInfoLabel(scan->plotType(), x[y.indexOfMin()], y.min());
    if (scan->plotType() == As::Integrated) {
        const int l = scan->m_numLeftSkipPoints + scan->m_numLeftBkgPoints;
        const int r = scan->m_numPoints - scan->m_numRightBkgPoints - scan->m_numRightSkipPoints - 1;
        appendCentralInfoLabel(As::Raw, (x[l] + x[r]) / 2, scan->m_normMeanBkg); }
}

/*!
...
*/
int As::Plot::getHklIndexPrecision(const qreal value)
{
    if (qAbs(value - qRound(value)) > 0.03)
        return 3;
    return 0;
}

/*!
...
*/
int As::Plot::getNumberPrecision(const qreal value)
{
    if (value < 1)
        return 2;
    if (value < 10)
        return 1;
    return 0;
}

/*!
...
*/
QString As::Plot::formatForInfoBox(const QString &name,
                                   const qreal value,
                                   const qreal error)
{
    if (qIsNaN(value) OR qIsNaN(error))
        return QString("");
    return QString("%1: %2 \u00B1 %3\n")
            .arg(name)
            .arg(value, 0, 'f', getNumberPrecision(value))
            .arg(error, 0, 'f', getNumberPrecision(error));
}

/*!
...
*/
QString As::Plot::formatForInfoBox(const QString &name,
                                   const qreal value1,
                                   const qreal value2,
                                   const qreal value3)
{
    if (qIsNaN(value1) OR qIsNaN(value2) OR qIsNaN(value3))
        return QString("");
    return QString("%1: %2, %3, %4\n")
            .arg(name)
            .arg(value1, 0, 'f', getHklIndexPrecision(value1))
            .arg(value2, 0, 'f', getHklIndexPrecision(value2))
            .arg(value3, 0, 'f', getHklIndexPrecision(value3));
}

/*!
...
*/
// Make correction according to updateInfoLabels!?
void As::Plot::updateInfoBox(const As::Scan *scan)
{
    // Set colors
    setPlotColors(scan->plotType());
    // Create string with plot info
    QString text = "";
    // Add information to be shown
    text += formatForInfoBox("HKL", scan->m_meanIndexH, scan->m_meanIndexK, scan->m_meanIndexL);
    if (scan->plotType() != As::Excluded) {
        for (const QString &countType : As::COUNT_TYPES) {
            if (!qIsNaN(scan->m_normPeakArea[countType])) {
                text += formatForInfoBox("Area" + countType, scan->m_normPeakArea[countType], scan->m_normPeakAreaErr[countType]); } }
        for (const QString &countType : As::COUNT_TYPES) {
            if (!qIsNaN(scan->m_structFactor[countType])) {
                text += formatForInfoBox("F2" + countType,   scan->m_structFactor[countType], scan->m_structFactorErr[countType]); } }
        text += formatForInfoBox("Fwhm", scan->m_fullWidthHalfMax, scan->m_fullWidthHalfMaxErr);
        text += formatForInfoBox("FR",   scan->m_flippingRatio,    scan->m_flippingRatioErr); }
    // Remove last newline symbol
    text.remove(QRegExp("\n$"));
    // Create point in the top right corner of the scanPlot to move the plotInfo by some pixels from this point
    auto point = new QCPItemLine(this);
    addItem(point);
    point->start->setType(QCPItemPosition::ptAxisRectRatio);
    point->end->setType(QCPItemPosition::ptAxisRectRatio);
    point->start->setCoords(1, 0);
    point->end->setCoords(point->start->coords());
    // Show plot info
    auto plotInfo = new QCPItemText(this);
    addItem(plotInfo);
    plotInfo->position->setParentAnchor(point->start);
    plotInfo->position->setCoords(-11, 11);
    plotInfo->setPositionAlignment(Qt::AlignTop | Qt::AlignRight);
    plotInfo->setTextAlignment(Qt::AlignLeft);
    plotInfo->setPadding(QMargins(10, 7, 10, 7));
    plotInfo->setBrush(QBrush(m_textBkgColor));
    plotInfo->setColor(m_textColor);
    plotInfo->setText(text);
    plotInfo->setVisible(true);
    if (text.isEmpty())
        plotInfo->setVisible(false);
    plotInfo->setLayer("legend");
}

/*!
...
*/
// Not updated after zoom!?
void As::Plot::addXMiddleArrows(const As::Scan *scan)
{
    if (scan->plotType() == As::Integrated) {
        int arrowLength = 8;
        int arrowWidth = 10;
        int yShift = 2;
        As::RealVector x = (*scan)["angles"][scan->scanAngle()]["data"];
        const int l = scan->m_numLeftSkipPoints + scan->m_numLeftBkgPoints;
        const int r = scan->m_numPoints - scan->m_numRightBkgPoints - scan->m_numRightSkipPoints - 1;
        // Upper (top) arrow
        auto arrowT = new QCPItemLine(this);
        addItem(arrowT); // allows to clear it when needed
        arrowT->end->setType(QCPItemPosition::ptAxisRectRatio);
        arrowT->end->setCoords(0.5, 0);
        arrowT->end->setType(QCPItemPosition::ptAbsolute);
        arrowT->end->setCoords(arrowT->end->coords().x(), arrowT->end->coords().y() - yShift);
        arrowT->end->setType(QCPItemPosition::ptAxisRectRatio);
        arrowT->start->setParentAnchor(arrowT->end);
        arrowT->start->setCoords(0, -arrowLength);
        arrowT->start->setCoords(arrowT->start->coords());
        arrowT->setPen(QPen(m_textBkgColor));
        arrowT->setClipToAxisRect(false);
        arrowT->setHead(QCPLineEnding(QCPLineEnding::esFlatArrow, arrowWidth, arrowLength));
        arrowT->setLayer("labels");
        // Lower (bottom) arrow
        auto arrowB = new QCPItemLine(this);
        addItem(arrowB); // allows to clear it when needed
        /*
        arrow->end->setType(QCPItemPosition::ptAxisRectRatio);
        arrow->end->setCoords(0.5, 0);
        arrow->end->setType(QCPItemPosition::ptPlotCoords);
        arrow->end->setCoords((x[l] + x[r]) / 2, arrow->end->coords().y());
        arrow->end->setType(QCPItemPosition::ptAxisRectRatio);
        arrow->start->setParentAnchor(arrow->end);
        arrow->start->setCoords(0, arrowLength);
        */
        arrowB->end->setType(QCPItemPosition::ptPlotCoords);
        arrowB->end->setCoords((x[l] + x[r]) / 2, m_yAxisMax);
        arrowB->start->setType(QCPItemPosition::ptPlotCoords);
        arrowB->start->setParentAnchor(arrowB->end);
        arrowB->start->setCoords(0, arrowLength);
        arrowB->setPen(QPen(m_textBkgColor));
        arrowB->setClipToAxisRect(false);
        arrowB->setHead(QCPLineEnding(QCPLineEnding::esFlatArrow, arrowWidth, arrowLength));
        arrowB->setLayer("labels"); }
}

/*!
...
*/
void As::Plot::addCustomGraph(const As::PlotType plotType,
                              const QString &countType,
                              const QCPScatterStyle::ScatterShape markType,
                              const Qt::PenStyle lineType,
                              const Qt::BrushStyle fillType,
                              const QCPGraph::ErrorType errType)
{
    // Add graph and set its name
    addGraph();
    // Set colors
    setPlotColors(plotType, countType);
    // Scatter symbols (marks)
    graph()->setScatterStyle(QCPScatterStyle(markType, m_markDrawColor, m_markFillColor, m_markSize));
    // Curve line
    graph()->setPen(QPen(m_lineDrawColor, m_debugLineWidth, lineType));
    // Filled area
    graph()->setBrush(QBrush(m_areaFillColor, fillType));
    // Error bars
    graph()->setErrorType(errType);
    graph()->setErrorPen(QPen(m_errorBarsDrawColor));
}

/*!
...
*/
void As::Plot::addAllGraphs(const As::Scan *scan)
{
    // Define data
    As::RealVector x  = (*scan)["angles"][scan->scanAngle()]["data"];
    As::RealVector y  = (*scan)["intensities"]["DetectorNorm"]["data"];
    As::RealVector sy = (*scan)["intensities"]["sDetectorNorm"]["data"];
    // Define local variables
    QPair<QVector<int>, QVector<int> > ranges;
    QVector<QVector<qreal> > data;
    //bool hasSkipPoints = scan->m_numLeftSkipPoints + scan->m_numRightSkipPoints;
    QStringList countTypes{""};
    // Create graphs depends on plotType
    switch (scan->plotType()) {

    case As::Raw: {
        ranges.first.clear();
        ranges.second.clear();
        ranges.first << scan->m_numLeftSkipPoints;
        ranges.second << scan->m_numPoints - scan->m_numRightSkipPoints;
        // Add graphs according to the measured data (unpolarised or polarised)
        for (const QString &countType : countTypes) {
            As::RealVector y  = scan->data("intensities", "DetectorNorm" + countType);
            As::RealVector sy  = scan->data("intensities", "sDetectorNorm" + countType);
            if (!y.isEmpty()) {
                data.clear();
                data << x.toQVector() << y.toQVector() << sy.toQVector();
                addCustomGraph(scan->plotType(), countType,
                               QCPScatterStyle::ssCircle, Qt::SolidLine,
                               Qt::SolidPattern, QCPGraph::etValue);
                updateGraphOnPlot(ranges, data);
                graph()->setName(tr(qPrintable("Scan" + countType))); } }
        break; }

    case As::Integrated: {
        // Peak marks, line and fill
        ranges.first.clear();
        ranges.second.clear();
        ranges.first  << scan->m_numLeftSkipPoints + scan->m_numLeftBkgPoints;
        ranges.second << scan->m_numPoints - scan->m_numRightSkipPoints - scan->m_numRightBkgPoints;
        data.clear();
        data << x.toQVector() << y.toQVector() << sy.toQVector();
        addCustomGraph(scan->plotType(), "",
                       QCPScatterStyle::ssCircle, Qt::SolidLine,
                       Qt::SolidPattern, QCPGraph::etValue);
        updateGraphOnPlot(ranges, data);
        graph()->setName(tr("Peak"));
        // Bottom line to cut the filled area of the above peak graph
        data.clear();
        data << x.toQVector() << QVector<qreal>(x.toQVector().size(), scan->m_normMeanBkg);
        addCustomGraph(scan->plotType(), "",
                       QCPScatterStyle::ssNone, Qt::NoPen,
                       Qt::NoBrush, QCPGraph::etNone);
        updateGraphOnPlot(ranges, data);
        int previous = graphCount() - 2;
        graph(previous)->setChannelFillGraph(graph());
        graph()->removeFromLegend();
        // Skipped marks, if any
        if (scan->m_numLeftSkipPoints + scan->m_numRightSkipPoints > 0) {
            ranges.first.clear();
            ranges.second.clear();
            ranges.first  << 0;
            ranges.second << scan->m_numLeftSkipPoints;
            ranges.first  << scan->m_numPoints - scan->m_numRightSkipPoints;
            ranges.second << scan->m_numPoints;
            data.clear();
            data << x.toQVector() << y.toQVector() << sy.toQVector();
            addCustomGraph(As::Excluded, "",
                           QCPScatterStyle::ssCircle, Qt::NoPen,
                           Qt::NoBrush, QCPGraph::etValue);
            updateGraphOnPlot(ranges, data);
            graph()->setName(tr("Skipped")); }
        // Background marks
        ranges.first.clear();
        ranges.second.clear();
        ranges.first  << scan->m_numLeftSkipPoints;
        ranges.second << scan->m_numLeftSkipPoints + scan->m_numLeftBkgPoints;
        ranges.first  << scan->m_numPoints - scan->m_numRightSkipPoints - scan->m_numRightBkgPoints;
        ranges.second << scan->m_numPoints - scan->m_numRightSkipPoints;
        data.clear();
        data << x.toQVector() << y.toQVector() << sy.toQVector();
        addCustomGraph(As::Raw, "",
                       QCPScatterStyle::ssCircle, Qt::NoPen,
                       Qt::NoBrush, QCPGraph::etValue);
        updateGraphOnPlot(ranges, data);
        graph()->setName(tr("Background"));
        // Background line
        ranges.first.clear();
        ranges.second.clear();
        ranges.first  << scan->m_numLeftSkipPoints;
        ranges.second << scan->m_numPoints - scan->m_numRightSkipPoints;
        data.clear();
        data << x.toQVector() << QVector<qreal>(x.toQVector().size(), scan->m_normMeanBkg);
        addCustomGraph(As::Raw, "",
                       QCPScatterStyle::ssNone, Qt::DotLine,
                       Qt::SolidPattern, QCPGraph::etNone);
        updateGraphOnPlot(ranges, data);
        graph()->setName(tr("Background mean"));
        break; }

    case As::Fitted: {
        ADEBUG;
        break; }

    case As::Excluded: {
        ranges.first.clear();
        ranges.second.clear();
        ranges.first << scan->m_numLeftSkipPoints;
        ranges.second << scan->m_numPoints - scan->m_numRightSkipPoints;
        data.clear();
        data << x.toQVector() << y.toQVector() << sy.toQVector();
        addCustomGraph(scan->plotType(), "",
                       QCPScatterStyle::ssCircle, Qt::SolidLine,
                       Qt::SolidPattern, QCPGraph::etValue);
        updateGraphOnPlot(ranges, data);
        graph()->setName(tr("Scan"));
        break; }

    default: {
        qFatal("%s: unknown plotType", __FUNCTION__);
        break; } }
}

/*!
...
*/
void As::Plot::updateGraphOnPlot(const QPair<QVector<int>, QVector<int> > ranges,
                                 const QVector<QVector<qreal> > data)
{
    QVector<QVector<qreal> > subData(data.size());
    // Fill subData arrays
    for (int m = 0; m < ranges.first.size(); ++m) {
        for (int k = ranges.first[m]; k < ranges.second[m]; ++k) {
            for (int i = 0; i < data.size(); ++i) {
                subData[i] << data[i][k]; } } }
    // Select appropriate plot type depends on number of data columns
    if (subData.size() == 3)
        graph()->setDataValueError(subData[0], subData[1], subData[2]);
    else if (subData.size() == 2)
        graph()->setData(subData[0], subData[1]);
    else
        qFatal("%s: wrong size of the subData array", __FUNCTION__);
}

/*!
...
*/
void As::Plot::updateAllOnPlot(const As::Scan *scan)
{
    //ADEBUG << scan;
    // Get data to plot
    As::RealVector x  = (*scan)["angles"][scan->scanAngle()]["data"];
    As::RealVector y  = (*scan)["intensities"]["DetectorNorm"]["data"];
    As::RealVector sy = (*scan)["intensities"]["sDetectorNorm"]["data"];
    //ADEBUG << x;
    //ADEBUG << y;
    //ADEBUG << sy;
    // Update axes ranges
    updateAxesRanges(x, y, sy); // Auto by QCustomPlot: rescaleAxes();
    // Remove all existing graphs
    clearPlottables();
    // Removes all item from the plot (like additional text previously plotted: info labels and box)
    clearItems();
    // Set plot colors
    setPlotColors(scan->plotType());
    // Create graphs depends on plotType
    addAllGraphs(scan);
    // Update info labels
    updateInfoLabels(scan);
    // Update info box
    updateInfoBox(scan);
    // Create Legend
    auto isLegendHidden = QSettings().value("PlotSettings/hideLegend", false).toBool();
    legend->setVisible(!isLegendHidden);
    legend->setBrush(QBrush(QColor(255,255,255,230)));
    // By default, the legend is in the inset layout of the main axis rect.
    // So this is how we access it to change legend placement:
    axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignLeft);
    // Plot arrows to compare the position of the scan center with that of the actual peak center
    addXMiddleArrows(scan);
    // Re-plot everything defined above
    replot();
}

/*!
...
*/
void As::Plot::showPointCoordinatesToolTip(QMouseEvent *event)
{
    const qreal x = xAxis->pixelToCoord(event->x());
    const qreal y = yAxis->pixelToCoord(event->y());
    const qreal xMin = xAxis->range().lower;
    const qreal xMax = xAxis->range().upper;
    const qreal yMin = yAxis->range().lower;
    const qreal yMax = yAxis->range().upper;
    const bool isInside = x >= xMin AND x <= xMax AND y >= yMin AND y <= yMax;
    QString toolTip;
    if (isInside)
        toolTip = QString("(%1, %2)").arg(x, 0, 'f', 2).arg(y, 0, 'f', 1);
    setToolTip(toolTip);
    /*
    // Coordinates of mouse pointer (in px)
    QPoint p(event->pos().x(), event->pos().y());
    // Coordinates of top left and bottom right corners of axis rectangle (in px)
    QPoint tL(this->axisRect()->topLeft());
    QPoint bR(this->axisRect()->bottomRight());
    // Check if pointer is inside axis rectangle. If yes, convert px to axis dimentions and visualize via tooltip
    const bool isInside = p.x() >= tL.x() AND p.x() <= bR.x() AND p.y() >= tL.y() AND p.y() <= bR.y();
    if (isInside) {
        const qreal x = this->xAxis->pixelToCoord(p.x());
        const qreal y = this->yAxis->pixelToCoord(p.y());
        setToolTip(QString("(%1, %2)").arg(x, 0, 'f', 2).arg(y, 0, 'f', 1)); }
    */
}

/*!
...
*/
void As::Plot::mousePressToZoom(QMouseEvent *event)
{
    m_leftMouseButtonPressed = false;
    m_rightMouseButtonPressed = false;
    // Check if the left button is pressed
    if (event->buttons() == Qt::LeftButton) {
        m_leftMouseButtonPressed = true;
        // Create zoom rectangle
        m_zoomRectangle = new QCPItemRect(this);
        addItem(m_zoomRectangle);
        // Zoom rectangle line and fill colors
        m_zoomRectangle->setPen(As::Color(As::red));
        m_zoomRectangle->setBrush(As::Color(As::redLightTransparent));
        // Get coordinates when the left button is pressed
        m_xZoom1 = xAxis->pixelToCoord(event->x());
        m_yZoom1 = yAxis->pixelToCoord(event->y());
        // Set axes for top left and bottom right points
        //m_zoomRectangle->topLeft->setAxes(xAxis, yAxis);
        //m_zoomRectangle->bottomRight->setAxes(xAxis, yAxis);
        // Set 1st coordinates of the zoom rectangle
        m_zoomRectangle->topLeft->setCoords(m_xZoom1, m_yZoom1); }
    // Check if the right button is pressed
    else if (event->buttons() == Qt::RightButton) {
        m_rightMouseButtonPressed = true; }
}

/*!
...
*/
void As::Plot::mouseMoveToZoom(QMouseEvent *event)
{
    // Check if the left button is pressed
    if (event->buttons() == Qt::LeftButton) {
        // Get coordinates when the left button is pressed
        m_xZoom2 = xAxis->pixelToCoord(event->x());
        m_yZoom2 = yAxis->pixelToCoord(event->y());
        // Set 2nd coordinates of the zoom rectangle
        m_zoomRectangle->bottomRight->setCoords(m_xZoom2, m_yZoom2);
        // Replot graph to see the movement of the zoom rectangle
        replot(); }
}

/*!
...
*/
void As::Plot::mouseReleaseToZoom(QMouseEvent *event)
{
    // Check if the left button is released
    if (m_leftMouseButtonPressed) {
        m_xZoom2 = xAxis->pixelToCoord(event->x());
        m_yZoom2 = yAxis->pixelToCoord(event->y());
        // Calc max and min coordinates of the zoom rectangle, but stay inside the axes rectangle
        qreal xMin = qMax(qMin(m_xZoom1, m_xZoom2), m_xAxisMin);
        qreal xMax = qMin(qMax(m_xZoom1, m_xZoom2), m_xAxisMax);
        qreal yMin = qMax(qMin(m_yZoom1, m_yZoom2), m_yAxisMin);
        qreal yMax = qMin(qMax(m_yZoom1, m_yZoom2), m_yAxisMax);
        // Set new ranges to zoom
        xAxis->setRange(xMin, xMax);
        yAxis->setRange(yMin, yMax);
        // Remove zoom rectangle
        removeItem(m_zoomRectangle); }
    // Check if the right button is released
    else if (m_rightMouseButtonPressed) {
        // Reset ranges to restore the graph after zoom
        xAxis->setRange(m_xAxisMin, m_xAxisMax);
        yAxis->setRange(m_yAxisMin, m_yAxisMax); }
    // Replot graph with new ranges
    replot();
}
