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

#include <QString>

#include "Macros.hpp"

#include "CheckBox.hpp"

/*!
    \class As::CheckBox

    \brief The CheckBox widget is a custom class based on the QCheckBox.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs a checkbox with the given \a text and \a parent.
*/
As::CheckBox::CheckBox(const QString& text,
                       QWidget* parent)
    : QCheckBox(parent) {
    setText(text); }

/*!
    Destroys the widget.
*/
As::CheckBox::~CheckBox() {
    ADESTROYED; }

/*!
    Modifies the checked state of the check box with \a state while
    signals emitted by this object are blocked.
*/
void As::CheckBox::setCheckedSilently(const bool state) {
    blockSignals(true);
    setChecked(state);
    blockSignals(false); }
