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

#include <QCheckBox>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Macros.hpp"

#include "CheckBox.hpp"
#include "ComboBox.hpp"
#include "GroupBox.hpp"
#include "HBoxLayout.hpp"
#include "Label.hpp"
#include "PushButton.hpp"
#include "Style.hpp"
#include "VBoxLayout.hpp"

#include "PreferencesDialog.hpp"

/*!
    ...
*/
As::PreferencesDialog::PreferencesDialog(QWidget* parent)
    : QDialog(parent) {
    ADEBUG;

    auto layout = new As::VBoxLayout;
    layout->addWidget(createLanguageGroup());
    layout->addWidget(createUpdateGroup());

    setLayout(layout);
    setWindowTitle(tr("Preferences")); }

/*!
    ...
*/
As::GroupBox* As::PreferencesDialog::createLanguageGroup() {
    ADEBUG;

    auto languageComboBox = new As::ComboBox;
    languageComboBox->setToolTip(tr("Select program language. Restart is required."));
    languageComboBox->addItem(tr("English"));
    //languageComboBox->addItem(tr("German"));
    //languageComboBox->addItem(tr("Russian"));

    auto layout = new QVBoxLayout;
    layout->addWidget(languageComboBox);

    auto group = new As::GroupBox("PreferencesGeneralGroup", tr("Language"));
    group->setLayout(layout);

    return group; }

/*!
    ...
*/
As::GroupBox* As::PreferencesDialog::createUpdateGroup() {
    ADEBUG;

    const bool autoUpdate = QSettings().value("Preferences/autoUpdate", true).toBool();

    auto checkAuto = new As::CheckBox(tr("Automatically check for updates"));
    checkAuto->setToolTip(tr("Automatically check for updates."));
    checkAuto->setChecked(autoUpdate);
    connect(checkAuto, &As::CheckBox::toggled, this,
            &As::PreferencesDialog::setAutoUpdateSettings);

    auto checkNow = new As::PushButton(tr("Check now"));
    checkNow->setToolTip(tr("Click to check for updates."));
    connect(checkNow, &As::PushButton::clicked, this,
            &As::PreferencesDialog::checkUpdateNowClicked);

    auto layout = new QVBoxLayout;
    layout->addWidget(checkAuto);
    layout->addWidget(checkNow);

    auto group = new As::GroupBox("PreferencesUpdateGroup", tr("Update"));
    group->setLayout(layout);
    group->setProperty("isLast", true);

    return group; }

/*!
    ...
*/
void As::PreferencesDialog::setAutoUpdateSettings(const bool autoUpdate) {
    ADEBUG << "autoUpdate:" << autoUpdate;

    QSettings().setValue("Preferences/autoUpdate", autoUpdate); }

