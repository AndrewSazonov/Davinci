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

#ifndef AS_DATATYPES_STRING_HPP
#define AS_DATATYPES_STRING_HPP

#include <QDebug>

class QString;
class QRegExp;

namespace As { //AS_BEGIN_NAMESPACE

class String
{

public:
    String();
    String(const QString &other);

    virtual ~String();

    //friend QDebug &operator<<(QDebug &debug, const String &string);

    QString &operator=(const QString &other);
    int size() const;
    bool startsWith(const QString &subString) const;
    void append(const QString &other);
    QString &replace(const QRegExp &before, const QString &after);
    QString &remove(const QRegExp &subString);
    QString mid(const int position, const int n) const;
    QString toQString() const;

private:
    QString m_string;

};

} //AS_END_NAMESPACE

QDebug operator<<(QDebug debug, const As::String &string);

Q_DECLARE_METATYPE(As::String) // To use it as a custom type in QVariant.

#endif // AS_DATATYPES_STRING_HPP
