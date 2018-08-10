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
#include <QStringList>
#include <QRegExp>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>

#include "Constants.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "StringParser.hpp"

/*!
    \class As::StringParser

    \brief The StringParser is a class based on the As::String that provides
    a string parser.

    \inmodule DataTypes
*/

/*!
    Constructs an empty string parser.
*/
As::StringParser::StringParser() {}

/*!
    Constructs a copy of \a other.
*/
As::StringParser::StringParser(const As::StringParser& other) :
    As::String(other) {}

/*!
    Constructs a copy of \a other.
*/
As::StringParser::StringParser(const As::String& other) :
    As::String(other) {}

/*!
    Constructs a copy of \a other.
*/
As::StringParser::StringParser(const QString& other) :
    As::String(other) {}

/*!
    Destroys the string parser.
*/
As::StringParser::~StringParser() {}

/*!
    Returns the index position of the first element in the \a list which contains \a text,
    searching forward from index position \a from. Returns -1 if no item matched.

    This function is case sensitive.
*/
int As::StringParser::indexOfText(const QString& text,
                                  const QStringList& list,
                                  const int from) const {
    // very expensive! 350 files take 7 seconds.
    //return list.indexOf(QRegExp("(^|.+)" + QRegExp::escape(text) + "($|.+)"), from);

    // less expensive. 350 files take 1.1 second.
    const int size = list.size();

    for (int i = from; i < size; ++i)
        if (list[i].contains(text)) {
            return i; }

    return -1; }

/*!
    Reads the \a list at the position \a index and increments \a index,
    if it is smaller than the number of list elements.
*/
void As::StringParser::readCheckIncrement(const QStringList& list,
                                          int& index) {
    *this = list[index];

    if (index < list.size()) {
        ++index; } }

/*!
    Sets the string parser from the given \a list, \a index, \a skip
    and \a grab.
*/
void As::StringParser::setFromList(const int index,
                                   const QStringList& list,
                                   const int skip,
                                   const int grab) {
    //ADEBUG;
    const int indexFrom = index + skip;
    const int indexTo = indexFrom + grab;

    *this = QString();

    if (indexFrom != -1 AND indexTo <= list.size()) {
        for (int i = indexFrom; i < indexTo; ++i) {
            QString string = list[i];

            if (!string.endsWith("\n")) {
                string.append("\n"); }

            append(string); }

        remove(QRegExp("\n$")); } }

/*!
    Sets the string parser from the given \a text, \a list, \a skip
    and \a grab.

    \overload
*/
void As::StringParser::setFromList(const QString& text,
                                   const QStringList& list,
                                   const int skip,
                                   const int grab) {
    const int indexFrom = indexOfText(text, list) + skip;
    const int indexTo = indexFrom + grab;

    *this = QString();

    if (indexFrom != -1 AND indexTo <= list.size()) {
        for (int i = indexFrom; i < indexTo; ++i) {
            QString string = list[i];

            if (!string.endsWith("\n")) {
                string.append("\n"); }

            append(string); }

        remove(QRegExp("\n$")); } }

/*!
    Sets the string parser from the given \a textFrom, \a textTo, \a list
    and \a skip.

    \overload
*/
void As::StringParser::setFromList(const QString& textFrom,
                                   const QString& textTo,
                                   const QStringList& list,
                                   const int skip) {
    //ADEBUG;
    const int indexFrom = indexOfText(textFrom, list) + skip;
    const int indexTo = indexOfText(textTo, list);

    *this = QString();

    if (indexFrom != -1 AND indexTo != -1 AND indexFrom <= indexTo) {
        for (int i = indexFrom; i < indexTo; ++i) {
            QString string = list[i];

            if (!string.endsWith("\n")) {
                string.append("\n"); }

            append(string); }

        remove(QRegExp("\n$")); } }

/*!
    Returns the parsed string based on the given \a type and \a del.
*/
QString As::StringParser::parseString(const QString& type,
                                      const QString& del) {
    //ADEBUG;

    QRegExp before;
    QString after(" ");

    if (type == "date") {
        before.setPattern("[^0-9]"); // all characters but: '0123456789'
        QString result = this->remove(QRegExp(del)).replace(before, after).simplified();
        QString format = As::DATA_TIME_FORMAT;
        format.replace(" ", "_");
        return As::FormatString(result, format); }

    else if (type == "num") { /// FIX ALL HERE
        QRegularExpression re("([-+]?\\d+[\\.]?\\d*[eE]?[-+]?\\d*)");
        QRegularExpressionMatchIterator i = re.globalMatch(this->toQString());
        QStringList list;

        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString num = match.captured(1);
            list << num; }

        return list.join(" "); }

    else if (type == "txt") {
        before.setPattern("[#;,=><]"); }  // only characters: '#', ';', ',', '=', '>', '<'

    else if (type == "raw") {
        return remove(QRegExp("\n")); }

    else if (type == "mult") {
        return replace(QRegExp("\t"), " ").remove(QRegExp("[^-.0-9\\s]")).replace(QRegExp(" +"), " "); }

    else {
        qFatal("%s: unknown type '%s'", __FUNCTION__, qPrintable(type)); } // is qPrintable needed everywhere?

    //return this->replace(before, after).remove(del).simplified();
    return remove(QRegExp(del)).replace(before, after).simplified(); }

/*!
    Returns the parsed string based on the given \a index, \a type, \a list,
    \a skip and \a grab.

    \overload
*/
QString As::StringParser::parseString(const int index,
                                      const QString& type,
                                      const QStringList& list,
                                      const int skip,
                                      const int grab) {
    setFromList(index, list, skip, grab);
    return parseString(type); }

/*!
    Returns the parsed string based on the given \a text, \a type, \a list,
    \a skip and \a grab.

    \overload
*/
QString As::StringParser::parseString(const QString& text,
                                      const QString& type,
                                      const QStringList& list,
                                      const int skip,
                                      const int grab) {
    setFromList(text, list, skip, grab);
    return parseString(type, text); }

/*!
    Returns the parsed string based on the given \a textFrom, \a textTo, \a type,
    \a list and \a skip.

    \overload
*/
QString As::StringParser::parseString(const QString& textFrom,
                                      const QString& textTo,
                                      const QString& type,
                                      const QStringList& list,
                                      const int skip) {
    setFromList(textFrom, textTo, list, skip);
    return parseString(type); }

/*!
    Returns the splitted and separated with spaces text based on the given
    \a nChars and \a nColumns.
*/
QString As::StringParser::splitText(const int nChars,
                                    const int nColumns) {
    // Calculate number of chunks in single column
    int nChunks = size() / (nChars * nColumns);
    QStringList list;

    for (int j = 0; j < nChunks; ++j) {
        QString str;

        for (int i = 0; i < nColumns; ++i) {
            QTextStream(&str) << " " << this->mid(nChars * (i * nChunks + j), nChars); }

        list << str.simplified(); }

    return list.join("\n"); }

/*!
    Returns the parsed pattern based on the given \a pattern and \a del.
*/
QString As::StringParser::parsePattern(const QString& pattern,
                                       const QString& del) {
    QRegExp re(pattern);
    re.indexIn(this->toQString());
    return re.cap().remove(QRegExp(del)); }

/*!
    Returns the parsed number based on the given \a text and \a order.
*/
QString As::StringParser::parseNumberNearText(const QString& text,
                                              const QString& order) {
    const QString num = "\\D*\\-?\\+?\\d+.?\\d*";
    const QRegExp re("[^-.0-9]");
    QString string;

    if (order == "num:txt") {
        string = num + text; }

    else if (order == "txt:num") {
        string = text + num; }

    else {
        qFatal("%s: unknown order type '%s'", __FUNCTION__, qPrintable(order)); }

    return parsePattern(string, text).remove(re).simplified(); }

/*!
    Returns the parsed number based on the given \a text, \a order and \a list.

    \overload
*/
QString As::StringParser::parseNumberNearText(const QString& text,
                                              const QString& order,
                                              const QStringList& list) {
    setFromList(text, list);
    return parseNumberNearText(text, order); }
