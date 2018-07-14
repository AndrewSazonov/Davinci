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

#include <string.h> // strrchr function for __FILENAME__, temporary solution

#include <QDebug>

#define __AFILENAME__ (strrchr("/" __FILE__, '/') + 1)

#ifdef QT_DEBUG
    #define IS_DEBUG_OR_PROFILE 1 // Debug or Profile build
    #define AASSERT(cond, what) ((!(cond)) ? qt_assert_x(__func__, qPrintable(what), __AFILENAME__, __LINE__) : qt_noop())
#else
    #define IS_DEBUG_OR_PROFILE 0 // Release build
    #define AASSERT(cond, what) qt_noop()
#endif

#define ADEBUG qDebug()

#define ADEBUG_H1 qDebug() << qUtf8Printable(QString().leftJustified(50, '-'))
#define ADEBUG_H2 qDebug() << qUtf8Printable(QString().leftJustified(50, '='))
#define ADEBUG_H3 qDebug() << qUtf8Printable(QString().leftJustified(50, '#'))

#define ADESTROYED qDebug() << "Destroyed:" << this

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
