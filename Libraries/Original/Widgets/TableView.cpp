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

#include <QtMath>

#include <QFont>
#include <QHeaderView>
#include <QSettings>

#include "Macros.hpp"

#include "SortFilterProxyModel.hpp"

#include "TableView.hpp"

/*!
\class As::TableView

\brief The TableView widget is a custom class based on the QTableView.

\inmodule Widgets
\ingroup Widgets
*/

/*!
Constructs a table view widget with the given \a parent.
*/
As::TableView::TableView(QWidget *parent)
    : QTableView(parent)
{
    const auto font = QSettings().value("TextSettings/font").value<QFont>();
    setFont(font);

    setSortingEnabled(true);
    setAlternatingRowColors(true);

    horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    horizontalHeader()->setFixedHeight(m_minRowHeight); // setMinimumHeight

    verticalHeader()->setDefaultAlignment(Qt::AlignRight | Qt::AlignCenter);
    verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(m_minRowHeight); // setMinimumSectionSize
    verticalHeader()->setMinimumWidth(m_minRowHeadersWidth); // setFixedWidth

    m_proxyModel = new As::SortFilterProxyModel(this);
}

/*!
Destroys the widget.
*/
As::TableView::~TableView()
{
    ADESTROYED;
}

/*!
Sets the \a model for the view to present.
*/
void As::TableView::setModel(QAbstractItemModel *model)
{
    m_proxyModel->setSourceModel(model);

    //QTableView::setModel(model);
    QTableView::setModel(m_proxyModel);

    // Set columns width
    const int columnCount = m_proxyModel->sourceModel()->columnCount();
    for (int i = 0; i < columnCount; ++i) {
        // adjust column width to the contents+10, if needed
        // width of the header text is not taken into account by sizeHintForColumn. Fix it!?
        const int w = qMax(m_minColumnWidth, sizeHintForColumn(i) + 10);
        setColumnWidth(i, w); }

    adjustRowColumnCount();
}

// Enlarge table if nesessary
void As::TableView::adjustRowColumnCount() {

    // Calc table height and width
    setTableHeight();
    setTableWidth();

    // Calc number of rows and columns to be added to the table
    const QRect cr = contentsRect();
    const int rowsCount = qCeil((static_cast<qreal>(cr.height()) - m_tableHeight) / m_minRowHeight);
    const int columnsCount = qCeil((static_cast<qreal>(cr.width()) - m_tableWidth) / m_minColumnWidth);

    // Add the required number of rows (to the bottom) and columns (to the right)
    if (rowsCount > 0)
        m_proxyModel->sourceModel()->insertRows(m_proxyModel->sourceModel()->rowCount(), rowsCount);
    if (columnsCount > 0)
        m_proxyModel->sourceModel()->insertColumns(m_proxyModel->sourceModel()->columnCount(), columnsCount);
}

// Calc table height
void As::TableView::setTableHeight() {
    m_tableHeight = horizontalHeader()->height();
    const int rowCount = m_proxyModel->sourceModel()->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        m_tableHeight += rowHeight(i); }
}

// Calc table width
void As::TableView::setTableWidth() {
    m_tableWidth = verticalHeader()->width();
    const int columnCount = m_proxyModel->sourceModel()->columnCount();
    for (int i = 0; i < columnCount; ++i) {
        m_tableWidth += columnWidth(i); }
}

/*!
Reimplements the QTableView resizeEvent function to fill empty spaces around table with
empty rows and columns, i.e. enlarge table if it is smaller than the widget size.
*/
void As::TableView::resizeEvent(QResizeEvent *e)
{
    QTableView::resizeEvent(e);

    adjustRowColumnCount();
}

