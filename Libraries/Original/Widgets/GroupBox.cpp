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

#include <QLayout>
#include <QSettings>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "Macros.hpp"

#include "GroupBox.hpp"

/*!
    \class As::GroupBox

    \brief The GroupBox is a custom class based on the QGroupBox.

    \inmodule Widgets
    \ingroup Widgets
*/

/*!
    Constructs a groupbox with the given \a objectName, \a title and \a parent.
*/
As::GroupBox::GroupBox(const QString& objectName,
                       const QString& title,
                       QWidget* parent)
    : QGroupBox(parent) {

    setFlat(true);

    if (!objectName.isEmpty() AND !title.isEmpty()) {
        setCheckable(true);
        setObjectName(objectName);
        setTitle(title); }

    // required for the styling
    setProperty("isHideable", isCheckable()); }

/*!
    Reimplements the QGroupBox setLayout function to support show or hide (folded or unfolded)
    group's content possibility (Wraps group \a layout into layout of another widget).
*/
void As::GroupBox::setLayout(QLayout* layout) {
    auto groupContent = new QWidget;
    groupContent->setLayout(layout);

    auto box = new QVBoxLayout;
    box->addWidget(groupContent);
    box->setMargin(0);

    QGroupBox::setLayout(box);

    // Check if group can be toggled
    if (isCheckable()) {

        connect(this, &As::GroupBox::toggled, this,         &As::GroupBox::updateSettings);
        connect(this, &As::GroupBox::toggled, groupContent, &As::GroupBox::setVisible);

        // Set group open according to the readed settings
        const auto isOpen = QSettings().value("UnfoldedGroups/" + objectName(), true).toBool();
        setChecked(isOpen); } }

/*!
    Destroys the widget.
*/
As::GroupBox::~GroupBox() {
    ADESTROYED; }

/*!
    Saves the \a state (folded or unfolded) of the group box in the global settings.
    This is used then when the program is opened again to restore the previous state.
*/
void As::GroupBox::updateSettings(const QVariant& state) {
    // Check if group can be toggled
    if (isCheckable()) {
        AASSERT(!objectName().isEmpty(), "group is not named");
        QSettings().setValue("UnfoldedGroups/" + objectName(), state); } }

