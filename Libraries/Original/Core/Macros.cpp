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

/*!
\macro IS_DEBUG
\relates As

This macro returns 1 if the program is build in debug mode;
otherwise returns 0.
*/

/*!
\macro ADEBUG
\relates As

This macro is a shortcut for \c 'qDebug()'.
*/

/*!
\macro ADESTROYED
\relates As

This macro is a shortcut for \c 'qDebug() << "Destroyed:" << this', which prints
a pointer to the current function.
*/

/*!
\macro AEXIT
\relates As

This macro is a shortcut for \c 'exit(0);', which tells the application to exit.
*/

/*!
\macro OR
\relates As

This macro is a shortcut for the logical operator \c '||' (or).
*/

/*!
\macro AND
\relates As

This macro is a shortcut for the logical operator \c '&&' (and).
*/

/*!
\macro IF
\relates As

This macro is a shortcut for the conditional operator \c 'if'.
*/

/*!
\macro EI
\relates As

This macro is a shortcut for the conditional operator \c 'else if'.
*/

/*!
\macro EL
\relates As

This macro is a shortcut for the conditional operator \c 'else'.
*/

/*/*!
\macro APP_DATE
\relates As

This macro is a shortcut for \c 'ToHumanDate(APP_GIT_DATE)', which converts
a date in the '1979-05-02' format to the '2 May 1979' format.
*/


/*!
\macro SB_INT
\relates As

This macro is a shortcut for \c '(void(QSpinBox::*)(int))', which is required in the
Qt5 signal/slot syntax to force the \c int type argument in the QSpinBox::valueChanged
signal.

Old signal-slot syntax:
connect(fileChanger, SIGNAL(valueChanged(int)), this, SLOT(gotoFile_Slot(int)));

New signal-slot syntax:
connect(fileChanger, QOverload<int>::of(&As::SpinBox::valueChanged), this, &As::Window::gotoFile_Slot);
connect(fileChanger, (void(QSpinBox::*)(int))&As::SpinBox::valueChanged, this, &As::Window::gotoFile_Slot);

With macro:
connect(fileChanger, SB&As::SpinBox::valueChanged, this, &As::Window::gotoFile_Slot);

OR:!!!
//QObject::connect<void(QSpinBox::*)(int)>(fileChanger, &As::SpinBox::valueChanged, this, &As::Window::gotoFile_Slot);
connect<void(QSpinBox::*)(int)>(fileChanger, &As::SpinBox::valueChanged, this, &As::Window::gotoFile_Slot);
*/

/*!
\macro TE_INT
\relates As

This macro is a shortcut for \c '(void(As::TextEdit::*)(int))', which is required in the
Qt5 signal/slot syntax to force the \c int type argument in the
As::TextEdit::cursorPositionChanged signal or As::TextEdit::setCursorPosition slot.
*/

/*/*!
\macro TE_NO
\relates As

This macro is a shortcut for \c '(void(As::TextEdit::*)())', which is required in the
Qt5 signal/slot syntax to force no arguments in the As::TextEdit::setCursorPosition slot.
*/

// to get rid of 'libAsCore.a(Macros.o) has no symbols'
int As::macrosObjHasNoSymbols = 0;
