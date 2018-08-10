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

#include <QHBoxLayout>
#include <QSettings>
#include <QString>

#include "Macros.hpp"

#include "Label.hpp"
#include "SpaceObject.hpp"
#include "SpinBox.hpp"

#include "SpinBoxSingleBlock.hpp"

/*!
    \class As::SpinBoxSingleBlock

    \brief The SpinBoxSingleBlock widget provides a block for side bar.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs a single spinbox block with the given \a parent and
    widgets \a changer, \a text and \a units.
*/
As::SpinBoxSingleBlock::SpinBoxSingleBlock(As::SpinBox*   changer,
                                           As::Label*     text,
                                           const QString& units,
                                           QWidget*       parent)
    : QWidget(parent) {
    const int SIDEBAR_WIDTH = QSettings().value("Sidebar/width").toInt();

    changer->setMinimumWidth(0.4 * SIDEBAR_WIDTH);
    //changer->setFixedWidth(0.4 * SIDEBAR_WIDTH);

    auto layout = new QHBoxLayout;
    layout->addWidget(changer);
    layout->addWidget(new QLabel(tr("of")));
    layout->addWidget(text);
    layout->addWidget(new QLabel(units));
    layout->addWidget(new As::SpaceObject);
    layout->setMargin(0);

    setLayout(layout); }

/*!
    Destroys the widget.
*/
As::SpinBoxSingleBlock::~SpinBoxSingleBlock() {
    ADESTROYED; }
