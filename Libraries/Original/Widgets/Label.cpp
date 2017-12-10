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

#include "Macros.hpp"

#include "Label.hpp"

/*!
\class As::Label

\brief The Label widget is a custom class based on the QLabel.

\inmodule Widgets
\ingroup Widgets
*/

/*!
Constructs a label with the given \a parent.
*/
As::Label::Label(QWidget *parent)
    : QLabel(parent) {}

/*!
Destroys the widget.
*/
As::Label::~Label()
{
    ADESTROYED;
}
