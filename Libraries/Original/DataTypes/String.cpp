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

#include <QRegExp>

#include "String.hpp"

/*!
    \class As::String

    \brief The String class is a class that provides a string of the
    QString type, which takes just the required functions of QString.

    \inmodule DataTypes
*/

/*!
    Constructs an empty string.
*/
As::String::String() {}

/*!
    Constructs a copy of \a other.
*/
As::String::String(const As::String& other) :
    m_string(other.m_string) {}

/*!
    Constructs a copy of \a other.
*/
As::String::String(const QString& other) :
    m_string(other) {}

/*!
    Destroys the string.
*/
As::String::~String() {}

/*!
    Assigns \a other to this string and returns a reference to this string.
*/
As::String& As::String::operator=(const As::String& other) {
    if (this != &other) {
        m_string = other.m_string; }

    return *this; }

/*!
    Returns the number of characters in this string.

    The last character in the string is at position size() - 1.
*/
int As::String::size() const {
    return m_string.size(); }

/*!
    Returns \c true if the string starts with \a subString; otherwise returns
    \c false.
*/
bool As::String::startsWith(const QString& subString) const {
    return m_string.startsWith(subString); }

/*!
    Appends the string \a other onto the end of this string.
*/
QString& As::String::append(const QString& other) {
    return m_string.append(other); }

/*!
    Replaces every occurrence of the string \a before with the string \a
    after and returns a reference to this string.
*/
QString& As::String::replace(const QRegExp& before, const QString& after) {
    return m_string.replace(before, after); }

/*!
    Removes every occurrence of the given \a subString string in this
    string, and returns a reference to this string.
*/
QString& As::String::remove(const QRegExp& subString) {
    return m_string.remove(subString); }

/*!
    Returns a string that contains \a n characters of this string,
    starting at the specified \a position index.

    Returns a null string if the \a position index exceeds the
    length of the string. If there are less than \a n characters
    available in the string starting at the given \a position, or if
    \a n is -1 (default), the function returns all characters that
    are available from the specified \a position.
*/
QString As::String::mid(const int position, const int n) const {
    return m_string.mid(position, n); }

/*!
    Returns the string as a QString.
*/
QString As::String::toQString() const {
    return m_string; }

/**
    Overloads operator<< for QDebug to accept String output
*/
QDebug operator<<(QDebug debug, const As::String& string) {
    debug << string.toQString();
    return debug; }
