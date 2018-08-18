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

#include "Constants.hpp"
#include "Macros.hpp"

#include "Label.hpp"
#include "SpaceObject.hpp"
#include "SpinBox.hpp"
#include "UnderLabeledWidget.hpp"

#include "SpinBoxDoubleBlock.hpp"

/*!
    \class As::SpinBoxDoubleBlock

    \brief The SpinBoxDoubleBlock widget provides a block for side bar.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs a double spinbox block with the given \a parent and
    widgets \a leftChanger, \a middleText, \a rightChanger and
    their labels \a nameLeft, \a nameMiddle, \a nameRight.
*/
As::SpinBoxDoubleBlock::SpinBoxDoubleBlock(As::SpinBox*  leftChanger,
                                           As::Label*    middleText,
                                           As::SpinBox*  rightChanger,
                                           const QString& nameLeft,
                                           const QString& nameMiddle,
                                           const QString& nameRight,
                                           QWidget*       parent)
    : QWidget(parent) {

    //const int APP_SIDEBAR_WIDTH = QSettings().value("Sidebar/width").toInt();

    leftChanger->setMinimumWidth (0.33 * As::APP_SIDEBAR_WIDTH);
    rightChanger->setMinimumWidth(0.33 * As::APP_SIDEBAR_WIDTH);

    auto objectLeft   = new As::UnderLabeledWidget(leftChanger,  nameLeft);
    auto objectMiddle = new As::UnderLabeledWidget(middleText,   nameMiddle);
    auto objectRight  = new As::UnderLabeledWidget(rightChanger, nameRight);

    auto layout = new QHBoxLayout;
    layout->addWidget(objectLeft);
    layout->addWidget(objectMiddle);
    layout->addWidget(objectRight);
    layout->setMargin(0);

    setLayout(layout); }

/*!
    Destroys the widget.
*/
As::SpinBoxDoubleBlock::~SpinBoxDoubleBlock() {
    ADESTROYED; }
