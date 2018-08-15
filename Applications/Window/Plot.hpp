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

#ifndef AS_PLOT_HPP
#define AS_PLOT_HPP

#include <QtGlobal>

#include "Colors.hpp"
#include "Constants.hpp"

#include "qcustomplot.h"

class QMouseEvent;
class QString;

namespace As { //AS_BEGIN_NAMESPACE

class Color;
class RealVector;
class Scan;

class Plot : public QCustomPlot {
    Q_OBJECT

  public:
    Plot();
    ///void showCoordinatesToolTip(QMouseEvent *event);
    void setPlotColors(const As::PlotType plotType,
                       const QString& countType = "");
    //void setAxesLabels(const QString &xLabel); // how to send labels there?
    void setAxesLabels();
    void updateAxesRanges(const As::RealVector& x,
                          const As::RealVector& y,
                          const As::RealVector& sy);
    void appendArrowInfoLabel(const As::PlotType plotType,
                              const qreal x,
                              const qreal y,
                              const int labelShift = 16,
                              const int arrowWidth = 14,
                              const int arrowLength = 7);
    void appendCentralInfoLabel(const As::PlotType plotType,
                                const qreal x,
                                const qreal y);
    void updateInfoLabels(const As::Scan* scan);
    int getHklIndexPrecision(const qreal value);
    int getNumberPrecision(const qreal value);
    QString formatForInfoBox(const QString& name,
                             const qreal value,
                             const qreal error);
    QString formatForInfoBox(const QString& name,
                             const qreal value1,
                             const qreal value2,
                             const qreal value3);
    void updateInfoBox(const As::Scan* scan);
    void addXMiddleArrows(const As::Scan* scan);
    void addCustomGraph(const As::PlotType plotType,
                        const QString& countType,
                        const QCPScatterStyle::ScatterShape markType,
                        const Qt::PenStyle lineType,
                        const Qt::BrushStyle fillType,
                        const QCPGraph::ErrorType errType);
    void addAllGraphs(const As::Scan* scan);
    void updateGraphOnPlot(const QPair<QVector<int>, QVector<int>> ranges,
                           const QVector<QVector<qreal>> data);
    void updateAllOnPlot(const Scan* scan);

  private slots:
    void showPointCoordinatesToolTip(QMouseEvent* event);
    void mousePressToZoom(QMouseEvent* event);
    void mouseMoveToZoom(QMouseEvent* event);
    void mouseReleaseToZoom(QMouseEvent* event);

  private:
    int m_markSize;
    int m_debugLineWidth;
    As::Color m_color;
    //As::Color scanBkgColor_;
    As::Color m_textColor;
    As::Color m_textBkgColor;
    As::Color m_lineDrawColor;
    As::Color m_markDrawColor;
    As::Color m_markFillColor;
    As::Color m_errorBarsDrawColor;
    As::Color m_areaFillColor;
    qreal m_xAxisMin;
    qreal m_xAxisMax;
    qreal m_yAxisMin;
    qreal m_yAxisMax;
    qreal m_xZoom1;
    qreal m_yZoom1;
    qreal m_xZoom2;
    qreal m_yZoom2;
    bool m_leftMouseButtonPressed;
    bool m_rightMouseButtonPressed;
    QCPItemRect* m_zoomRectangle;

};

} //AS_END_NAMESPACE

#endif // AS_PLOT_HPP


