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

#include <QFont>
#include <QHeaderView>
#include <QSettings>

#include "Macros.hpp"

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
    horizontalHeader()->setFixedHeight(26);

    verticalHeader()->setDefaultAlignment(Qt::AlignRight | Qt::AlignCenter);
    verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(26);
    verticalHeader()->setFixedWidth(50);
}

/*!
Destroys the widget.
*/
As::TableView::~TableView()
{
    ADESTROYED;
    ADEBUG << "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww" << this;
}

/*!
Sets the \a model for the view to present.
*/
void As::TableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
}
