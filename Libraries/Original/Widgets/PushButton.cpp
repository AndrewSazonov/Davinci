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

#include "PushButton.hpp"

/*!
    \class As::PushButton

    \brief The PushButton widget is a custom class based on the QPushButton.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs a push button with the given \a text and \a parent.
*/
As::PushButton::PushButton(const QString& text,
                           QWidget* parent)
    : QPushButton(parent) {

    setText(text);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    setMinimumHeight(24); }

/*!
    Destroys the widget.
*/
As::PushButton::~PushButton() {
    ADESTROYED; }
