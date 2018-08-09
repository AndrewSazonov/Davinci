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
#include <QtGlobal>

namespace As { //AS_BEGIN_NAMESPACE

class RealArray {

  public:
    RealArray(); // + default constructor

    RealArray(const As::RealArray& other); // + copy constructor
    RealArray(const QVector<qreal>& other);

    RealArray(const int size,
              const qreal defaultValue = 0.0);

    virtual ~RealArray(); // + virtual destructor

    virtual void set(const QVector<qreal>& other);

    qreal& operator[](const int i); // + subscript operator, modification allowed
    const qreal& operator[](const int i) const; // + subscript operator, modification not allowed

    QVector<qreal>& operator=(const QVector<qreal>& other);
    RealArray& operator=(const RealArray& other); // ? copy assignment operator

    RealArray operator+(const As::RealArray& other) const;

    bool operator==(const As::RealArray& other) const; // + equality operator

    bool isEmpty() const; // +
    int size() const; // +
    int indexOf(const qreal value) const; // +
    void append(const qreal value);//+
    void prepend(const qreal value);//+

    QVector<qreal> mid(const int pos,
                       const int length = -1) const; // +

    QVector<qreal> toQVector() const; // +
    QString toQString() const; // +


  protected:
    void resize(const int size); // + used by RealMatrix9
    const qreal* begin() const; // + for iterator
    const qreal* end() const; // + for iterator

  private:
    QVector<qreal> m_array; // make protected to access by subclasses? no

};

} //AS_END_NAMESPACE

QDebug operator<<(QDebug debug, const As::RealArray& array);

Q_DECLARE_METATYPE(As::RealArray) // To use it as a custom type in QVariant.

#endif // AS_DATATYPES_REALARRAY_HPP
