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

#ifndef AS_SCANDICT_HPP
#define AS_SCANDICT_HPP

#include <QMap>

#include "Constants.hpp"

class QString;
class QStringList;
//template<typename Key, typename T> class QMap;

namespace As { //AS_BEGIN_NAMESPACE

class ScanDict {

  public:
    using ElementAttributes_t = QMap<QString, QString>;
    using GroupElements_t     = QMap<QString, ElementAttributes_t>;
    using PropertyGroups_t    = QMap<QString, GroupElements_t>;

    ScanDict();
    ~ScanDict();

    const GroupElements_t operator[](const QString& section) const;
    const QStringList keys() const;

  private:
    void selectGroup(const QString& group);
    void set(const QString& element,
             const QString& format,
             const QString& units,
             const QString& tooltip);

    QString m_selectedGroup;
    PropertyGroups_t m_scanDict;

};

} //AS_END_NAMESPACE

#endif // AS_SCANDICT_HPP


