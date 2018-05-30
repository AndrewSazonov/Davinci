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

#include <QDateTime>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QFont>
#include <QFontMetrics>
#include <QRegExp>
#include <QRegularExpression>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVector>

#include <QtGlobal>
#include <QtMath>

#include "Constants.hpp"
#include "Macros.hpp"

#include "Functions.hpp"

/*!
Returns the square of \a v.
*/
qreal As::Sqr(const qreal v)
{
    return qPow(v, 2);
}

/*!
Returns the sign of a real number \a v.

\sa \link https://en.wikipedia.org/wiki/Sign_function Wiki: Sign function \endlink
*/
qreal As::Sign(const qreal v)
{
    IF (v > 0)
      return 1.;
    EI (v < 0)
      return -1.;
    EL
      return 0.;
}

/*!
Returns the angle \a angle converted to the range from -180 to +180 degrees.
*/
qreal As::ToMainAngularRange(const qreal angle)
{
    // -180 and 180 cases
    IF (angle == -180. OR angle == 180.)
        return angle;
    // Other cases
    IF (angle > 0.)
        return std::fmod(angle + 180., 360.) - 180.;
    EI (angle < 0.)
        return std::fmod(angle - 180., 360.) + 180.;
    EL /* angle == 0. */
        return 0.;
}

/*!
Returns the QVector<qreal> variable obtained from the given of \a string.
*/
QVector<qreal> As::ToRealVector(const QString &string)
{
    QVector<qreal> vector;
    QStringList list = string.split(QRegExp("[^-.0-9]"), QString::SkipEmptyParts);
    for (const QString &item : list) {
        bool ok = true;
        const qreal value = item.toDouble(&ok);
        if (ok)
            vector.append(value); }
    return vector;
}

/*!
Returns the string variable in '2 May 1979' format obtained from the given
\a string in '1979-05-02' format.
*/
QString As::ToHumanDate(const QString &string)
{
    QVector<int> vector;
    QStringList list = string.split(QRegExp("[^0-9]"), QString::SkipEmptyParts);
    for (const QString &item : list) {
        bool ok = true;
        const int value = item.toInt(&ok);
        if (ok)
            vector.append(value); }
    if (vector.size() >= 3) {
        int yyyy = vector[0];
        int MM = vector[1];
        int DD = vector[2];
        return QDate(yyyy, MM, DD).toString("d MMM yyyy"); }
    return QString();
}

/*!
Returns the QDateTime variable obtained from the given \a string.
*/
// not in use?!
QDateTime As::ToDateTime(const QString &string)
{
    QVector<int> vector;
    QStringList list = string.split(QRegExp("[^0-9]"), QString::SkipEmptyParts);
    for (const QString &item : list) {
        bool ok = true;
        const int value = item.toInt(&ok);
        if (ok)
            vector.append(value); }
    if (vector.size() >= 5) {
        int yyyy = vector[0];
        int MM = vector[1];
        int DD = vector[2];
        int hh = vector[3];
        int mm = vector[4];
        return QDateTime(QDate(yyyy, MM, DD), QTime(hh, mm)); }
    return QDateTime();
}

/*!
Returns the converted string from the given \a string.
*/
QString As::ToString(const QString &string)
{
    return string.split(QRegExp("[^-.0-9]"), QString::SkipEmptyParts).join(" ");
}

/*!
Returns the QStringList variable obtained from the given of \a string.
*/
QStringList As::ToStringList(const QString &string)
{
    return string.split(QRegExp("[^-.0-9]"), QString::SkipEmptyParts);
}

/*!
Returns the string formatted to a single value based on the given of \a string and \a format.
*/
const QString As::FormatString(const QString &string,
                               const QString &format)
{
    if (string.isEmpty() OR format.isEmpty())
        return string;
    QString result;
    // comma separated values
    if (format.contains("csv")) {
        result = QString().sprintf(qPrintable("%s;"), qPrintable(string)); }/// select ';' or ',' depends on the localisation?
    // date and time
    else if (format.contains("dd")) {
        const QRegularExpression re("[-/\\s_:.]");
        const QStringList data  = string.split(re, QString::SkipEmptyParts);
        const QStringList fmt  = As::DATA_TIME_FORMAT.split(re, QString::SkipEmptyParts);
        QVector<int> vector;
        for (const auto num : data)
            vector << num.toInt();
        const int yyyy = vector[fmt.indexOf("yyyy")];
        const int MM   = vector[fmt.indexOf("MM")];
        const int dd   = vector[fmt.indexOf("dd")];
        const int hh   = vector[fmt.indexOf("hh")];
        const int mm   = vector[fmt.indexOf("mm")];
        QDateTime dateTime(QDate(yyyy, MM, dd), QTime(hh, mm));
        result = dateTime.toString(format); }
    // string
    else if (format.contains("s")) {
        result = QString().sprintf(qPrintable("%" + format), qPrintable(string.split(" ")[0])); }
    // float (real) number
    else if (format.contains("f")) {
        QVector<qreal> vector;
        qreal sum = 0.;
        for (const auto num : string.split(" ")) {
            vector << num.toDouble();
            sum += num.toDouble(); }
        qreal mean = sum / vector.size();
        result = QString().sprintf(qPrintable("%" + format), mean); }
    // integer number
    else if (format.contains("i")) {
        QVector<qreal> vector;
        qreal sum = 0.;
        for (const auto num : string.split(" ")) {
            vector << num.toDouble();
            sum += num.toDouble(); }
        qreal mean = sum / vector.size();
        result = QString().sprintf(qPrintable("%" + format), qRound(mean)); }
    return result;
}

/*!
Returns the string formatted to a text based on the given \a string and \a format.
*/
const QString As::FormatStringToText(const QString &string,
                                     const QString &format)
{
    if (string.isEmpty() OR format.isEmpty())
        return string;
    QString result;
    // comma separated values
    IF (format.contains("csv")) {
        result = QString("%1%2").arg(string).arg(";"); }/// select ';' or ',' depends on the localisation?
    // date and time
    EI (format.contains("dd")) {
        result = QString("%1").arg(string, format.size()); }
    // string or float (real) number or integer number
    EI (format.contains("s") OR format.contains("f") OR format.contains("i")) {
        const QStringList list = format.split(QRegExp("[^0-9]"), QString::SkipEmptyParts);
        const int fieldWidth = QString(list[0]).toInt(); // list[0] contains the full field width
        const QString substring = string.left(fieldWidth - 1); // cut name if it >= fieldWidth
        result = QString("%1").arg(substring, fieldWidth); }
    // show error message if unknown format
    EL {
        const QString message = QString("unknown format '%1'").arg(format);
        Q_ASSERT_X(false, AFUNC, qPrintable(message)); }
    return result;
}

/*!
Returns the string formatted to the range based on the given of \a string and \a format.
*/
const QString As::FormatStringToRange(const QString &string,
                                      const QString &format)
{
    if (string.isEmpty() OR format.isEmpty())
        return string;
    QString other;
    if (format.contains("f")) { // float (real) number
        QVector<qreal> vector;
        qreal min = qInf();
        qreal max = -qInf();
        for (const auto num : string.split(" ")) {
            qreal d = num.toDouble();
            vector << d;
            if (d < min)
                min = d;
            if (d > max)
                max = d; }
        if (qAbs(max/min) > 1.01)
            other = QString().sprintf(qPrintable("%" + format + " - " + "%" + format), min, max);
        else
            other = FormatString(string, format); }
    return other;
}

/*!
Calculates the width of the sidebar (in px) depends on the width of the given \a font.
*/
int As::SidebarWidth(const QFont &font)
{
    QFontMetrics fm(font);
    //setCursorWidth(fm.averageCharWidth());
    int widthPx = fm.width("abcdefghijklmnopqrstuvwxyz0123456789-+*");
    if (widthPx % 2) // check if it's odd, i.e. 2n+1
        ++widthPx;   // if it's odd, convert it to even 2n+2 (for equal widths ofthe sidebar tabs)
    return widthPx;
}

/*!
Sets the debug output if \a isDebugMode is \c true.
*/
void As::SetDebugOutputFormat(const bool isDebugMode)
{
    // Changes the output of the default message handler (qDebug() = ADEBUG, etc.)
    // http://stackoverflow.com/questions/24012108/qdebug-not-showing-file-line
    if (isDebugMode)
        qInstallMessageHandler(As::DetailedMessageOutput);
    else
        qInstallMessageHandler(As::NoMessageOutput);
}

/*!
Sets the... \a skipLines \a fillChar
*/
QString As::DebugHeader(const int skipLines,
                        const QChar fillChar)
{
    const int width = As::DEBUG_WIDTH + skipLines - 3;
    return QString().fill('\n', skipLines).leftJustified(width, fillChar);
}

/*!
Prints out no debug messages using the given \a type.
*/
void As::NoMessageOutput(QtMsgType type,
                         const QMessageLogContext &,
                         const QString &)
{
    if (type == QtFatalMsg)
        abort();
}

/*!
Prints out the empty debug messages using the given \a type
and \a msg.
*/
void As::EmptyMessageOutput(QtMsgType type,
                            const QMessageLogContext &,
                            const QString &msg)
{
    const QByteArray localMsg = msg.toLocal8Bit();
    const QString message = QString(localMsg.constData()).
            leftJustified(As::DEBUG_WIDTH).
            left(As::DEBUG_WIDTH);

    fprintf(stderr, "%s\n", qUtf8Printable(message));

    if (type == QtFatalMsg)
        abort();
}

/*!
Prints out the detailed debug messages using the given \a type,
\a context and \a msg.
*/
void As::DetailedMessageOutput(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg) // qInstallMessageHandler
{
    // Index of the debug message
    const QString index = QString::number(++As::debugCount).
            rightJustified(As::DEBUG_INDEX_WIDTH).
            right(As::DEBUG_INDEX_WIDTH);

    // Name of the cpp file which contains the debug message
    const QFileInfo fi(context.file);
    const QString file = fi.baseName().
            leftJustified(As::DEBUG_FILE_WIDTH).
            left(As::DEBUG_FILE_WIDTH);

    // Number of line in the file which contains the debug message
    const QString line = QString::number(context.line).
            rightJustified(DEBUG_LINE_WIDTH);

    // Function name which contains the debug message
    const QString function = QString(context.function).
            remove(QRegExp("\\(.*$")).
            remove(QRegExp("^.*:")).
            leftJustified(As::DEBUG_FUNCTION_WIDTH).
            left(As::DEBUG_FUNCTION_WIDTH);

    // Time since the previous debug message
    qreal elapsedTime = (qreal)As::elapsedTimer.elapsed()/1000;
    QString units;
    IF (elapsedTime < 0.1) {
        units = ""; }
    EI (elapsedTime < 60) {
        units = "s"; }
    EI (elapsedTime < 3600) {
        elapsedTime /= 60;
        units = "m"; }
    EL {
        elapsedTime /= 3600;
        units = "h"; }
    QString time = units.isEmpty() ? "" : QString::number(elapsedTime, 'f', 1);
    time = time.
            append(units).
            rightJustified(As::DEBUG_TIME_WIDTH).
            right(As::DEBUG_TIME_WIDTH);
    As::elapsedTimer.restart();

    // Actual message
    const QByteArray localMsg = msg.toLocal8Bit();
    const QString message = QString(localMsg.constData()).
            leftJustified(As::DEBUG_MESSAGE_WIDTH).
            left(As::DEBUG_MESSAGE_WIDTH);

    // Print
    fprintf(stderr, "%s | %s | %s | %s | %s | %s\n",
            qUtf8Printable(index),
            qUtf8Printable(file),
            qUtf8Printable(line),
            qUtf8Printable(function),
            qUtf8Printable(time),
            qUtf8Printable(message));

    // Abort in the case of fatal message
    if (type == QtFatalMsg)
        abort();
}

/*!
Returns the \a version in string format converted to integer.

Example:
\code
// string: v1.7.13
// int: 1007013
\endcode
*/
int As::StringVersionToInt(QString version)
{
    const QStringList list = version.remove("v").split(".");

    if (list.size() != 3)
        return 0;

    for (const QString &str : list) {
        bool ok;
        str.toInt(&ok);
        if (!ok)
            return 0; }

    const int major = list[0].toInt();
    const int minor = list[1].toInt();
    const int patch = list[2].toInt();

    return major*1000000 + minor*1000 + patch;
}

/*!
Returns the formatted file path from the given base name of the first file
\a baseNameFirst, base name of the last file \a baseNameLast and absolute
path of the last file \a absolutePathLast.
*/
QString As::FormatToPathWithName(const QString &baseNameFirst,
                                 const QString &baseNameLast,
                                 const QString &absolutePathLast)
{
    QString path = QString("%1/%2-%3").arg(absolutePathLast).arg(baseNameFirst).arg(baseNameLast);
    if (baseNameFirst == baseNameLast)
        path = QString("%1/%2").arg(absolutePathLast).arg(baseNameLast);
    return path;
}

