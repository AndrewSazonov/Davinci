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

#include <QDialogButtonBox>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QWidget>

#include "Macros.hpp"

#include "PushButton.hpp"
#include "HBoxLayout.hpp"

#include "MessageWidget.hpp"

/*!
\class As::MessageWidget

\brief The MessageWidget widget is a custom class based on the QDialog.

It provides a modal dialog for informing the user or for asking the user
a question and receiving an answer.

\inmodule Widgets
\ingroup Widgets
*/

/*!
Constructs a message box with the given \a parent, box \a title and \a text
information, as well as the text \a okButton for ok button and text
\a cancelButton for cancel button.
*/
As::MessageWidget::MessageWidget(QWidget *parent,
                                 const QString &title,
                                 const QString &text,
                                 const QString &okButton,
                                 const QString &cancelButton,
                                 const bool showIcon)
    : QDialog(parent)
{
    //setWindowModality(Qt::WindowModal);

    // Application icon
    auto icon = new QLabel;
    icon->setPixmap(QPixmap(":/Icon/Davinci.svg"));

    // Message text
    auto message = new QLabel(text);
    message->setOpenExternalLinks(true);

    // Buttons
    auto buttonBox = new QDialogButtonBox;
    buttonBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    if (!okButton.isEmpty()) {
        buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept); }
    if (!cancelButton.isEmpty()) {
        buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject); }

    // Check if buttons should be shown (when text for buttons is provided)
    const bool showButtons = (!okButton.isEmpty() OR !cancelButton.isEmpty());

    // Layout
    auto layout = new QVBoxLayout;
    if (showIcon)
        layout->addWidget(icon);
    layout->addWidget(message);
    if (showButtons)
        layout->addWidget(buttonBox);

    // Horisontal centering for all the widgets in the layout
    for (int i = 0; i < layout->count(); ++i) {
        auto widget = layout->itemAt(i)->widget();
        layout->setAlignment(widget, Qt::AlignHCenter); }

    // Mesage box
    setLayout(layout);
    setWindowTitle(title);
    //setMaximumWidth(300);
    //resize(300, 200);
    //setGeometry(x(), y(), 300, height());
}

/*!
Destroys the widget.
*/
As::MessageWidget::~MessageWidget()
{
    ADESTROYED;
}


