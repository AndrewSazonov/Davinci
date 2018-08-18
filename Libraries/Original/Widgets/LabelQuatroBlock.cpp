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

#include <QLabel>
#include <QHBoxLayout>

#include "Macros.hpp"

#include "LabelQuatroBlock.hpp"

/*!
    \class As::LabelQuatroBlock

    \brief The LabelQuatroBlock widget provides a block for side bar.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs a block with four given labels \a label, \a value, \a range, \a step and \a parent.
*/
As::LabelQuatroBlock::LabelQuatroBlock(QLabel* label,
                                       QLabel* value,
                                       QLabel* range,
                                       QLabel* step,
                                       QWidget* parent)
    : QWidget(parent) {

    auto layout = new QHBoxLayout;

    layout->addWidget(label, 0);
    layout->addWidget(value, 1);
    layout->addWidget(range, 2);
    layout->addWidget(step,  3);

    layout->setMargin(0);
    layout->setStretch(0, 1);
    layout->setStretch(1, 1);
    layout->setStretch(2, 1);
    layout->setStretch(3, 1);

    label->setAlignment(Qt::AlignLeft);
    value->setAlignment(Qt::AlignRight);
    range->setAlignment(Qt::AlignRight);
    step->setAlignment (Qt::AlignRight);

    label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    value->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    range->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    step->setSizePolicy (QSizePolicy::Ignored, QSizePolicy::Preferred);

    setLayout(layout); }

/*!
    Destroys the widget.
*/
As::LabelQuatroBlock::~LabelQuatroBlock() {
    ADESTROYED; }
