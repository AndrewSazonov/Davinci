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

#ifndef AS_WIDGETS_CHECKBOX_HPP
#define AS_WIDGETS_CHECKBOX_HPP

#include <QCheckBox>

class QString;

namespace As { //AS_BEGIN_NAMESPACE

class CheckBox : public QCheckBox {

  public:

    CheckBox(const QString& text,
             QWidget* parent = Q_NULLPTR);

    virtual ~CheckBox();

  public slots:

    void setCheckedSilently(const bool state);

};

} //AS_END_NAMESPACE

#endif // AS_WIDGETS_CHECKBOX_HPP


