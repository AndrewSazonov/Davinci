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

#ifndef AS_DATATYPES_REALARRAY_HPP
#define AS_DATATYPES_REALARRAY_HPP

#include <QDebug>
#include <QMetaType>

#include <QtGlobal>

#include <QVector>

class QString;

namespace As { //AS_BEGIN_NAMESPACE

class RealArray {

  public:
    // contructors and destructor
    RealArray();                                // default constructor
    RealArray(const As::RealArray& other);      // copy constructor
    RealArray(const QVector<qreal>& other);     // parameterized constructor
    RealArray(const int size,
              const qreal defaultValue = 0.0);  // parameterized constructor
    RealArray(const QString& string);           // parameterized constructor
    virtual ~RealArray();                       // virtual destructor

    // operators
    const qreal& operator[](const int i) const;                 // subscript operator, without modification
    qreal& operator[](const int i);                             // subscript operator, with modification
    As::RealArray& operator=(const As::RealArray& other);       // copy assignment operator =
    As::RealArray operator+(const As::RealArray& other) const;  // binary operator +
    bool operator==(const As::RealArray& other) const;          // equality operator ==

    // methods
    const qreal& at(const int i) const;
    bool isEmpty() const;
    int size() const;
    int indexOf(const qreal value) const;
    void append(const qreal value);
    As::RealArray mid(const int pos,
                      const int length = -1) const;
    QVector<qreal> toQVector() const;
    QString toQString() const;

  protected:
    void resize(const int size);    // used by derived class RealMatrix9
    const qreal* begin() const;     // allows to use the range-based for loop
    const qreal* end() const;       // allows to use the range-based for loop

  private:
    QVector<qreal> m_array;         // prefer composition over inheritance from QVector

};

} //AS_END_NAMESPACE

QDebug operator<<(QDebug debug, const As::RealArray& array);

Q_DECLARE_METATYPE(As::RealArray)

#endif // AS_DATATYPES_REALARRAY_HPP
