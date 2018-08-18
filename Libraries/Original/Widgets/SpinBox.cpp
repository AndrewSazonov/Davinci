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

#include "Macros.hpp"

#include "SpinBox.hpp"

/*!
    \class As::SpinBox

    \brief The SpinBox widget is a custom class based on the QSpinBox.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs a spin box with the given \a parent.
*/
As::SpinBox::SpinBox(QWidget* parent)
    : QSpinBox(parent) {
    setMinimum(0);
    setMaximum(0);
    setMinimumHeight(24);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    setAlignment(Qt::AlignRight);

    // Get rid of the blue focus rectangle around the widget on macOS
    setAttribute(Qt::WA_MacShowFocusRect, false); }

/*!
    Destroys the widget.
*/
As::SpinBox::~SpinBox() {
    ADESTROYED; }

/*!
    Sets the \a minimum and \a maximum values of the spin box with a single
    function call while signals emitted by this object are blocked.
*/
void As::SpinBox::setRangeSilently(const int minimum,
                                   const int maximum) {
    blockSignals(true);
    setRange(minimum, maximum);
    blockSignals(false); }

/*!
    Sets the minimum value of the spin box with \a value while signals
    emitted by this object are blocked.
*/
void As::SpinBox::setMinimumSilently(const int value) {
    blockSignals(true);
    setMinimum(value);
    blockSignals(false); }

/*!
    Sets the minimum value of the spin box with \a value while signals
    emitted by this object are blocked.
*/
void As::SpinBox::setMaximumSilently(const int value) {
    blockSignals(true);
    setMaximum(value);
    blockSignals(false); }

/*!
    Sets the \a value of the spin box while signals emitted by this object
    are blocked.
*/
void As::SpinBox::setValueSilently(const int value) {
    blockSignals(true);
    setValue(value);
    blockSignals(false); }

