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

#ifndef AS_MACROS_HPP
#define AS_MACROS_HPP

#include <QDebug>

#ifdef QT_DEBUG
#   define IS_DEBUG 1
#else
#   define IS_DEBUG 0
#endif

#define H1_DEBUG qDebug() << qUtf8Printable(As::DebugHeader(0, '-'))
#define H2_DEBUG qDebug() << qUtf8Printable(As::DebugHeader(0, '='))
#define H3_DEBUG qDebug() << qUtf8Printable(As::DebugHeader(0, '#'))

#define ADEBUG qDebug()
#define ADEBUG1 H1_DEBUG;
#define ADEBUG2 H2_DEBUG;
#define ADEBUG3 H3_DEBUG;

#define ADESTROYED qDebug() << "Destroyed:" << this

#define AFUNC __func__

#define AEXIT exit(0)

#define OR  ||
#define AND &&

#define IF if
#define EI else if
#define EL else

//#define APP_DATE ToHumanDate(APP_GIT_DATE)

#define SB_INT (void(QSpinBox::*)(int))
#define TE_INT (void(As::TextEditor::*)(int))
//#define TE_NO  (void(As::TextEditor::*)())
//#define SB (void(QSpinBox::*)(int))
//#define TE (void(As::TextEditor::*)(int))
//#define CONNECT_SBINT connect<void(QSpinBox::*)(int)>

//#define AS_BEGIN_NAMESPACE namespace As {
//#define AS_END_NAMESPACE   }

namespace As { //AS_BEGIN_NAMESPACE
    extern int macrosObjHasNoSymbols;
} //AS_END_NAMESPACE

#endif // AS_MACROS_HPP
