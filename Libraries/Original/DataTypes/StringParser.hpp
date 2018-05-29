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

#ifndef AS_DATATYPES_STRINGPARSER_HPP
#define AS_DATATYPES_STRINGPARSER_HPP

#include "String.hpp"

class QString;
class QStringList;

namespace As { //AS_BEGIN_NAMESPACE

class StringParser : public String
{

public:
    StringParser();
    StringParser(const String &other);

    virtual ~StringParser();

    using String::operator=;

    int indexOfText(const QString &text,
                    const QStringList &list,
                    const int from = 0) const;
    void readCheckIncrement(const QStringList &list,
                            int &index);
    void setFromList(const int index,
                     const QStringList &list,
                     const int skip = 0,
                     const int grab = 1);
    void setFromList(const QString &text,
                     const QStringList &list,
                     const int skip = 0,
                     const int grab = 1);
    void setFromList(const QString &textFrom,
                     const QString &textTo,
                     const QStringList &list,
                     const int skip = 0);
    void test();
    QString test2(const QString &type,
                  const QString &del);
    QString parseString(const QString &type,
                        const QString &del = QString());
    QString parseString(const int index,
                        const QString &type,
                        const QStringList &list,
                        const int skip = 0,
                        const int grab = 1);
    QString parseString(const QString &text,
                        const QString &type,
                        const QStringList &list,
                        const int skip = 0,
                        const int grab = 1);
    QString parseString(const QString &textFrom,
                        const QString &textTo,
                        const QString &type,
                        const QStringList &list,
                        const int skip = 0);
    QString splitText(const int nChars,
                      const int nColumns);
    QString parsePattern(const QString &pattern,
                         const QString &del = QString());
    QString parseNumberNearText(const QString &text,
                                const QString &order);
    QString parseNumberNearText(const QString &text,
                                const QString &order,
                                const QStringList &list);

};

} //AS_END_NAMESPACE

#endif // AS_DATATYPES_STRINGPARSER_HPP
