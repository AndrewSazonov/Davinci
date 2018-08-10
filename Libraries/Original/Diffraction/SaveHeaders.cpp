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

#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVector>

#include "Macros.hpp"

#include "SaveHeaders.hpp"

/*!
\class As::SaveHeaders

\brief The SaveHeaders is a class that provides the different output
text formats to save the processed data.

\inmodule Diffraction
*/

/*!
Constructs the class depends on the given \a type and \a headers list.
*/
As::SaveHeaders::SaveHeaders(QString type,
                             const QStringList &headers)
{
    ADEBUG_H1;
    ADEBUG << type;
    ADEBUG << headers;

    // make enum for simplicity!
    // and add format to the name of the output file, before extension
    // in order not to overwrite input data file
    // when open just 1 file and use e.g. dat extension!

    // Convert to lower case
    type = type.toLower();

    // Read global settings: Save excluded scan
    const auto isAlwaysSaveHeaders = QSettings().value("OutputSettings/alwaysSaveHeaders", false).toBool();

    // Check if headers should be saved
    m_addHeader = isAlwaysSaveHeaders;

    // Choose the output (save) type
    if (type.contains("general")) { // General comma separated
        for (const QString &header : headers) {
            m_name   << header;
            m_format << "csv";
            m_addHeader = true; } }

    else if (type.contains("shelx") AND type.contains("real")) {
        m_name   << "H"    << "K"    << "L"    << "Sf2"  << "Sf2Err" << "Batch" << "S0X"  << "S2X"  << "S0Y"  << "S2Y"  << "S0Z"  << "S2Z";
        m_format << "8.3f" << "8.3f" << "8.3f" << "8.2f" << "8.2f"   << "4i"    << "8.5f" << "8.5f" << "8.5f" << "8.5f" << "8.5f" << "8.5f"; }

    //else if (type.contains("shelx") AND type.contains("integer")) {
    else if (type.contains("shelx")) {
        m_name   << "H"  << "K"  << "L"  << "Sf2"  << "Sf2Err" << "Batch" << "S0X"  << "S2X"  << "S0Y"  << "S2Y"  << "S0Z"  << "S2Z";
        m_format << "4i" << "4i" << "4i" << "8.2f" << "8.2f"   << "4i"    << "8.5f" << "8.5f" << "8.5f" << "8.5f" << "8.5f" << "8.5f"; }

    else if (type.contains("tbar")) {
        m_name   << "Scan" << "H"  << "K"  << "L"  << "Sf2"   << "Sf2Err" << "Theta" << "Omega" << "Chi"  << "Phi"  << "Temperature" << "Psi"   << "Fwhm";
        m_format << "6i"   << "4i" << "4i" << "4i" << "10.2f" << "10.2f"  << "8.2f"  << "8.2f"  << "8.2f" << "8.2f" << "10.2f"       << "12.2f" << "13.4f"; }

    else if (type.contains("umweg")) { // https://www.xray.cz/ecm-cd/soft/xray/general/umweg/umweg-98.htm
        m_name   << "H"  << "K"  << "L"  << "Theta" << "Psi"   << "Sf2";
        m_format << "4i" << "4i" << "4i" << "8.2f"  << "8.2f"  << "10.2f"; }

    else if (type.contains("ccsl")) {
        m_name   << "Scan" << "H"  << "K"  << "L"  << "Omega" << "Gamma" << "Nu"   << "FR"    << "FRerr" << "|FR-1|/FRerr" << "Temperature" << "Magnetic field";
        m_format << "5i"   << "5i" << "5i" << "5i" << "8.2f"  << "8.2f"  << "8.2f" << "10.6f" << "10.6f" << "8.2f"         << "7.1f"        << "5.1f"; }
}

/*!
Destroys the class.
*/
As::SaveHeaders::~SaveHeaders()
{
    ADESTROYED;
}

