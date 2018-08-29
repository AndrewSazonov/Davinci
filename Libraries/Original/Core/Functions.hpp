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

#ifndef AS_FUNCTIONS_HPP
#define AS_FUNCTIONS_HPP

#include <QtGlobal> // to recognise typedef 'qreal', enum 'QtMsgType', etc.

class QChar;
class QDateTime;
class QFont;
class QMessageLogContext;
class QString;
class QStringList;
class QTextStream;
template<typename> class QVector;

namespace As { //AS_BEGIN_NAMESPACE

qreal Sqr(const qreal v);
qreal Sign(const qreal v);
qreal ToMainAngularRange(const qreal angle);

QVector<qreal> ToRealVector(const QString& string);
QString ToHumanDate(const QString& string);
QDateTime ToDateTime(const QString& string);
QString ToString(const QString& string); // FormatToString? rename?
QStringList ToStringList(const QString& string);

const QString FormatString(const QString& string, // FormatString? rename?
                           const QString& format);
const QString FormatStringToText(const QString& string,
                                 const QString& format);
const QString FormatStringToRange(const QString& string,
                                  const QString& format);

int SidebarWidth(const QFont& font);

void SetDebugOutputFormat(const bool showDebugInfo);

void NoMessageOutput(QtMsgType type,
                     const QMessageLogContext&,
                     const QString&);
void DetailedMessageOutput(QtMsgType type,
                           const QMessageLogContext& context,
                           const QString& msg);

int StringVersionToInt(QString version);

QString FormatToPathWithName(const QString& baseNameFirst,
                             const QString& baseNameLast,
                             const QString& absolutePathLast);

bool FuzzyCompareDouble(double d1,
                        double d2);

} //AS_END_NAMESPACE

#endif // AS_FUNCTIONS_HPP
