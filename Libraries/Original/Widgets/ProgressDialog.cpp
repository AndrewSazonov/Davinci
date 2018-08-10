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

#include <QVBoxLayout>
#include <QVBoxLayout>

#include "Macros.hpp"

#include "ProgressBar.hpp"
#include "PushButton.hpp"

#include "ProgressDialog.hpp"

/*!
    \class As::ProgressDialog

    \brief The ProgressDialog widget is a custom class based on the QProgressDialog.

    It provides a progress dialog window to give the user an indication of the
    progress of an operation.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs a progress dialog window with the given \a parent.
*/
As::ProgressDialog::ProgressDialog(QWidget* parent)
    : QDialog(parent), m_bar(new As::ProgressBar) {
    setWindowFlags(Qt::Widget); // allows to be embeded inside layout. not needed?!

    auto layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(m_bar);

    setLayout(layout); }

/*!
    Destroys the widget.
*/
As::ProgressDialog::~ProgressDialog() {
    ADESTROYED; }

/*!
    Sets the progress dialog's minimum and maximum values to \a minimum and \a maximum, respectively.
*/
void As::ProgressDialog::setRange(const int minimum, const int maximum) {
    m_bar->setRange(minimum, maximum); }

/*!
    Sets the progress dialog's value to \a progress. If current value becomes equal to maximum,
    the reset method is called.
*/
void As::ProgressDialog::setValue(const int progress) {
    show();

    m_bar->setValue(progress);

    if (progress == m_bar->maximum()) {
        reset(); } }

/*!
    Resets the progress bar and hides the progress dialog.
*/
void As::ProgressDialog::reset() {
    hide();

    m_bar->reset(); }
