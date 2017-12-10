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

#include <QListView>

#include "Macros.hpp"

#include "ComboBox.hpp"

/*!
\class As::ComboBox

\brief The ComboBox widget is a custom class based on the QComboBox.

\inmodule Widgets
\ingroup Widgets
*/

/*!
Constructs a combobox with the given \a parent.
*/
As::ComboBox::ComboBox(QWidget *parent)
    : QComboBox(parent)
{
    // Repetition of FontComboBox!
    setMinimumHeight(24);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    setView(new QListView); // to apply common stylesheet to the default combobox item view
    setEditable(false);
}

/*!
Destroys the widget.
*/
As::ComboBox::~ComboBox()
{
    ADESTROYED;
}
