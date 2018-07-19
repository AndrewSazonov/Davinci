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

#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

#include "Macros.hpp"

#include "PushButtonWithProgress.hpp"

/*!
\class As::PushButtonWithProgress

\brief The PushButtonWithProgress widget provides a PushButton with ProgressBar.

\inmodule Widgets
\ingroup Widgets
*/

/*!
Constructs a button with given \a button, \a progress and \a parent.
*/
As::PushButtonWithProgress::PushButtonWithProgress(QPushButton *button,
                                                   QProgressBar *progress,
                                                   QWidget *parent)
    : QWidget(parent)
{
    progress->setMinimum(0);

    auto layout = new QVBoxLayout;
    layout->addWidget(button);
    layout->addWidget(progress);

    layout->setMargin(0);
    layout->setSpacing(2);

    setLayout(layout);
}

/*!
Destroys the widget.
*/
As::PushButtonWithProgress::~PushButtonWithProgress()
{
    ADESTROYED;
}
