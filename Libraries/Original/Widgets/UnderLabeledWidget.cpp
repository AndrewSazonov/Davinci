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

#include <QLabel>
#include <QString>
#include <QVBoxLayout>

#include "Macros.hpp"

#include "UnderLabeledWidget.hpp"

/*!
    \class As::UnderLabeledWidget

    \brief The UnderLabeledWidget widget provides a widget with a text label
    below it for tool bar.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs the given \a widget with the under label \a text and
    given \a parent.
*/

As::UnderLabeledWidget::UnderLabeledWidget(QWidget* widget,
                                           const QString& text,
                                           QWidget* parent)
    : QWidget(parent) {

    auto label = new QLabel(text);
    auto font = label->font();
    font.setPointSize(font.pointSize() - 2);
    label->setFont(font);

    auto layout = new QVBoxLayout;
    layout->addWidget(widget);
    layout->addWidget(label);
    layout->setAlignment(widget, Qt::AlignHCenter | Qt::AlignBottom);
    layout->setAlignment(label,  Qt::AlignHCenter | Qt::AlignBottom);
    layout->setSpacing(3);
    layout->setMargin(0);
    setLayout(layout); }

/*!
    Destroys the widget.
*/
As::UnderLabeledWidget::~UnderLabeledWidget() {
    ADESTROYED; }
