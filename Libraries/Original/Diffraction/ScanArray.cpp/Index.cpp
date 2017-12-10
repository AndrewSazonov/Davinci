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

#include <QtMath>

#include "Macros.hpp"
#include "Functions.hpp"

#include "RealMatrix9.hpp"
#include "RealVector.hpp"
#include "Scan.hpp"

#include "ScanArray.hpp"

/*!
Index the reflection based on the scattering angles.
*/
void As::ScanArray::indexPeaks()
{
    ADEBUG2;

    // For every scan in the scan array
    for (auto scan : m_scanArray) {

        // Get data
        As::RealVector wavelength = (*scan)["conditions"]["Wavelength"]["data"];
        As::RealVector twotheta = (*scan)["angles"]["2Theta"]["data"];
        As::RealVector omega = (*scan)["angles"]["Omega"]["data"];
        As::RealVector chi = (*scan)["angles"]["Chi"]["data"];
        As::RealVector phi = (*scan)["angles"]["Phi"]["data"];
        As::RealVector gamma = (*scan)["angles"]["Gamma"]["data"];
        As::RealVector nu = (*scan)["angles"]["Nu"]["data"];
        As::RealVector h = (*scan)["indices"]["H"]["data"];
        As::RealVector k = (*scan)["indices"]["K"]["data"];
        As::RealVector l = (*scan)["indices"]["L"]["data"];

        // Check if ub matrix is read
        if (!(*scan)["orientation"]["matrix"]["data"].isEmpty()) {

            As::RealMatrix9 ub = (*scan)["orientation"]["matrix"]["data"]; // make variable m_ubMatrix?!
            //int size = scan["misc"]["nPoints"]["data"].toInt();

            // Set angles
            if (!h.isEmpty() AND !k.isEmpty() AND !l.isEmpty() AND gamma.isEmpty()) {

                // Define angles of the scan center
                As::RealVector xyz = hklToXyz(ub, h.mean(), k.mean(), l.mean());
                As::RealVector angles = xyzToAngles(wavelength.mean(), xyz[0], xyz[1], xyz[2]);

                // Fill arrays with the found angles
                for (int i = 0; i < scan->size(); ++i) {
                    twotheta.append(angles[0]);
                    chi.append(angles[2]);
                    phi.append(angles[3]);

                    // Define omega angle considering the scan step
                    qreal center = angles[1];
                    qreal shift = (i + 1 - qCeil((qreal)scan->size() / 2)) * scan->scanStep();
                    omega.append(center + shift); }

                // Set angle arrays to the scan
                scan->setData("angles", "2Theta", twotheta.toQString());
                scan->setData("angles", "Omega",  omega.toQString());
                scan->setData("angles", "Chi",    chi.toQString());
                scan->setData("angles", "Phi",    phi.toQString()); }

            // Set hkl
            else {

                // Lifting counter geometry /// not very good !!!
                if (!gamma.isEmpty()) {
                //if (nu.size() > 0) {
                    //if (gamma.size() == 0) // if name twotheta is wrongly used instead of gamma ?!
                    //    gamma = twotheta;
                    for (int i = 0; i < scan->size(); ++i) {
                        As::RealVector xyz = anglesToXyz(wavelength[i], gamma[i], nu[i], omega[i]);
                        As::RealVector hkl = xyzToHkl(ub, xyz[0], xyz[1], xyz[2]);
                        h.append(hkl[0]);
                        k.append(hkl[1]);
                        l.append(hkl[2]); } }

                // 4-circle geometry
                else {
                    for (int i = 0; i < scan->size(); ++i) {
                        As::RealVector xyz = anglesToXyz(wavelength[i], twotheta[i], omega[i], chi[i], phi[i]);
                        As::RealVector hkl = xyzToHkl(ub, xyz[0], xyz[1], xyz[2]);
                        h.append(hkl[0]);
                        k.append(hkl[1]);
                        l.append(hkl[2]); } }

                // Set indices to scan
                scan->setData("indices", "H", h.toQString());
                scan->setData("indices", "K", k.toQString());
                scan->setData("indices", "L", l.toQString()); } }

        // Define mean indices
        scan->m_meanIndexH = h.mean();
        scan->m_meanIndexK = k.mean();
        scan->m_meanIndexL = l.mean(); } }

/*!
Returns the calculated reciprocal lattice vectors \e x, \e y, \e z from the
given wavelength \a wavelength and scattering angles \a gamma, \a nu, \a omega
(Lifting counter geometry).
*/
const As::RealVector As::ScanArray::anglesToXyz(const qreal wavelength,
                                                qreal gamma,
                                                qreal nu,
                                                qreal omega) const {
    omega = qDegreesToRadians(omega);
    gamma = qDegreesToRadians(gamma);
    nu = qDegreesToRadians(nu);

    const qreal z = qSin(nu) / wavelength;
    const qreal z2 = As::Sqr(z);
    const qreal theta = 0.5 * qAcos(qCos(gamma) * qCos(nu));
    const qreal q2 = As::Sqr(2 / wavelength * qSin(theta));
    const qreal x2plusY2 = q2 - z2;
    const qreal delta = qAsin(0.5 * wavelength * q2 / qSqrt(x2plusY2));  // GEO.sig from UBmatrix.c is not used?
    const qreal phi = -0.5 * M_PI + delta - omega;
    const qreal x = qCos(phi) * qSqrt(x2plusY2);
    const qreal y = qSin(phi) * qSqrt(x2plusY2);

    return QVector<qreal>{x, y, z};
}

/*!
\overload

Returns the calculated reciprocal lattice vectors \e x, \e y, \e z from the
given wavelength \a wavelength and scattering angles \a twotheta, \a omega, \a chi,
\a phi (Four-circle geometry).
*/
const As::RealVector As::ScanArray::anglesToXyz(const qreal wavelength,
                                                qreal twotheta,
                                                qreal omega,
                                                qreal chi,
                                                qreal phi) const
{
    twotheta = qDegreesToRadians(twotheta);
    omega = qDegreesToRadians(omega); // not used?
    chi = qDegreesToRadians(chi);
    phi = qDegreesToRadians(phi);

    const qreal d = 2 *  qSin(0.5  * twotheta) / wavelength;
    const qreal x = d *  qCos(phi) * qCos(chi);
    const qreal y = d * -qSin(phi) * qCos(chi);
    const qreal z = d *              qSin(chi);

    return QVector<qreal>{x, y, z};
}

/*!
Returns the calculated Miller indices \e h, \e k, \e l from the given
UB matrix \a ub and reciprocal lattice vectors \a x, \a y, \a z. Miller indices
are returned in the form of As::RealVector with the following order:
0 - \e h, 1 - \e k, 2 - \e l.
*/
const As::RealVector As::ScanArray::xyzToHkl(const As::RealMatrix9 &ub,
                                             const qreal x,
                                             const qreal y,
                                             const qreal z) const
{
    const As::RealMatrix9 inv = ub.inv();

    qreal h = inv[0] * x + inv[1] * y + inv[2] * z;
    qreal k = inv[3] * x + inv[4] * y + inv[5] * z;
    qreal l = inv[6] * x + inv[7] * y + inv[8] * z;

    return QVector<qreal>{h, k, l};
}

/*!
Returns the calculated reciprocal lattice vectors \e x, \e y, \e z from the given
UB matrix \a ub and Miller indices \a h, \a k, \a l. Reciprocal lattice
vectors are returned in the form of As::RealVector with the following order:
0 - \e x, 1 - \e y, 2 - \e z.
*/
const As::RealVector As::ScanArray::hklToXyz(const As::RealMatrix9 &ub,
                                             const qreal h,
                                             const qreal k,
                                             const qreal l) const
{
    const qreal x = ub[0] * h + ub[3] * k + ub[6] * l;
    const qreal y = ub[1] * h + ub[4] * k + ub[7] * l;
    const qreal z = ub[2] * h + ub[5] * k + ub[8] * l;

    return QVector<qreal>{x, y, z};
}

/*!
Returns the calculated scattering angles from the given wavelength \a wavelength
and reciprocal lattice vectors \a x, \a y, \a z. Angles are returned in the form
of As::RealVector with the following order: 0 - \e twotheta, 1 - \e omega, 2 - \e chi,
3 - \e phi (Four-circle geometry), 4 - \e twotheta, 5 - \e gamma, 6 - \e nu,
7 - \e omega (Lifting counter geometry).
*/
const As::RealVector As::ScanArray::xyzToAngles(const qreal wavelength,
                                                const qreal x,
                                                const qreal y,
                                                const qreal z) const
{
    const qreal q2xyz = As::Sqr(x) + As::Sqr(y) + As::Sqr(z);
    const qreal q2xy =  As::Sqr(x) + As::Sqr(y);

    // 4-circle geometry theta = omega
    qreal phi1 = M_PI - qAtan(y / x);
    qreal chi1 = qAtan(z / qSqrt(q2xy));
    qreal omega1 = qAsin(0.5 * wavelength * qSqrt(q2xyz));
    qreal twotheta1 = 2 * omega1;

    // Lifting counter geometry
    qreal omega2 = -qAtan2(y, x) + qAsin(0.5 * wavelength * q2xyz / qSqrt(q2xy)) - 0.5*M_PI; // GEO.sig?
    qreal nu2 = qAsin(wavelength * z); // -0.5Pi to +0.5Pi
    qreal twotheta2 = 2 * qAsin(0.5 * wavelength * qSqrt(q2xyz)); // 0 to +Pi
    qreal gamma2 = qAcos(qCos(twotheta2) / qCos(nu2)); // GEO.sig?; 0 to +Pi or -Pi to 0

    // Convert angles
    QVector<qreal> angles({twotheta1, omega1, chi1, phi1, twotheta2, gamma2, nu2, omega2});
    for (qreal &value : angles)
        value = As::ToMainAngularRange(qRadiansToDegrees(value));

    return angles;
}

