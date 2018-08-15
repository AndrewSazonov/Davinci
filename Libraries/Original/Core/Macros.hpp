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

#ifndef AS_MACROS_HPP
#define AS_MACROS_HPP

#include <string.h> // strrchr function for AFILENAME, temporary solution!

#include <QDebug>

#define AFILENAME (strrchr("/" __FILE__, '/') + 1)

#ifdef QT_DEBUG
    #define IS_DEBUG_OR_PROFILE 1 // Debug or Profile build
    #define AASSERT(cond, what) ((!(cond)) ? qt_assert_x(__func__, qPrintable(what), AFILENAME, __LINE__) : qt_noop())
#else
    #define IS_DEBUG_OR_PROFILE 0 // Release build
    #define AASSERT(cond, what) qt_noop()
#endif

#define ADEBUG qDebug()

#define ADEBUG_H1 qDebug() << qUtf8Printable(QString().leftJustified(50, '-'))
#define ADEBUG_H2 qDebug() << qUtf8Printable(QString().leftJustified(50, '='))
#define ADEBUG_H3 qDebug() << qUtf8Printable(QString().leftJustified(50, '#'))

#define ADESTROYED qDebug() << "Destroyed:" << this

#define AEXIT exit(0);
#define AQUIT quit();

#define OR  ||
#define AND &&

//#define APP_DATE ToHumanDate(APP_GIT_DATE)

// The following is not correctly interpreted by qdoc
//#define AS_BEGIN_NAMESPACE namespace As {
//#define AS_END_NAMESPACE }

namespace As { //AS_BEGIN_NAMESPACE
extern int macrosObjHasNoSymbols; } //AS_END_NAMESPACE

#endif // AS_MACROS_HPP
