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

#include "ProgressBar.hpp"

/*!
\class As::ProgressBar

\brief The ProgressBar widget is a custom class based on the QProgressBar.

It provides a horizontal progress bar to give the user an indication of the
progress of an operation.

\inmodule Widgets
\ingroup Widgets
*/

/*!
Constructs a progress bar with the given \a parent.
*/
As::ProgressBar::ProgressBar(QWidget *parent)
    : QProgressBar(parent)
{
    setMinimum(0);
    setMaximum(100);
    setMinimumHeight(2);
    setMaximumHeight(2);
    setTextVisible(false);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
}

/*!
Destroys the widget.
*/
As::ProgressBar::~ProgressBar()
{
    ADESTROYED;
}

/*!
Hides or shows the progress bar depends on the current \a index.
*/
void As::ProgressBar::hideOrShow(const int index)
{
    if (index == minimum())
        show();

    if (index == maximum())
        hide();
}
