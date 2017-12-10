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

#include <QLabel>
#include <QHBoxLayout>

#include "Macros.hpp"

#include "LabelTripleBlock.hpp"

/*!
\class As::LabelTripleBlock

\brief The LabelTripleBlock widget provides a block for side bar.

\inmodule Widgets
\ingroup Widgets
*/

/*!
Constructs a block with three given labels \a label, \a data, \a units and \a parent.
*/
As::LabelTripleBlock::LabelTripleBlock(QLabel *label,
                                       QLabel *data,
                                       QLabel *units,
                                       QWidget *parent)
    : QWidget(parent)
{

    auto layout = new QHBoxLayout;
    layout->addWidget(label, 0);
    layout->addWidget(data,  1);
    layout->addWidget(units, 2);

    layout->setMargin(0);
    layout->setStretch(0, 5);
    layout->setStretch(1, 5);
    layout->setStretch(2, 1);

    label->setAlignment(Qt::AlignLeft);
    data->setAlignment( Qt::AlignRight);
    units->setAlignment(Qt::AlignLeft);

    label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    data->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    units->setSizePolicy(QSizePolicy::Ignored,          QSizePolicy::Preferred);

    setLayout(layout);
}

/*!
Destroys the widget.
*/
As::LabelTripleBlock::~LabelTripleBlock()
{
    ADESTROYED;
}
