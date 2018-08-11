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
Index single reflection based on the scattering angles.
*/
void As::ScanArray::indexSinglePeak(const int index)
{

    auto scan = at(index);

    // Get data
    As::RealVector wavelength = scan->data("conditions", "Wavelength");
    As::RealVector twotheta = scan->data("angles", "2Theta");
    As::RealVector omega = scan->data("angles", "Omega");
    As::RealVector chi = scan->data("angles", "Chi");
    As::RealVector phi = scan->data("angles", "Phi");
    As::RealVector gamma = scan->data("angles", "Gamma");
    As::RealVector nu = scan->data("angles", "Nu");
    As::RealVector psi = scan->data("angles", "Psi");
    As::RealVector h = scan->data("indices", "H");
    As::RealVector k = scan->data("indices", "K");
    As::RealVector l = scan->data("indices", "L");

    //As::RealVector psi2(QString(""));

    // Check if ub matrix is read
    if (!scan->data("orientation", "matrix").isEmpty()) {

        As::RealMatrix9 ub = scan->data("orientation", "matrix"); // make variable m_ubMatrix?!
        //int size = scan["misc"]["nPoints").toInt();

        // Set angles
        if (!h.isEmpty() AND !k.isEmpty() AND !l.isEmpty() AND gamma.isEmpty()) {

            // Define angles of the scan center
            As::RealVector xyz = hklToXyz(ub, h.mean(), k.mean(), l.mean());
            As::RealVector angles = xyzToAngles(wavelength.mean(), xyz[0], xyz[1], xyz[2], psi.mean());

            // Fill arrays with the found angles
            for (int i = 0; i < scan->numPoints(); ++i) {
                twotheta.append(angles[0]);
                chi.append(angles[2]);
                phi.append(angles[3]);

                // Define omega angle considering the scan step
                qreal center = angles[1];
                qreal shift = (i + 1 - qCeil(static_cast<qreal>(scan->numPoints()) / 2)) * scan->scanStep();
                omega.append(center + shift); }

            // Set angle arrays to the scan
            scan->setData("angles", "2Theta", twotheta.toQString());
            scan->setData("angles", "Theta",  twotheta.normalizeBy(2.0).toQString());
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
                for (int i = 0; i < scan->numPoints(); ++i) {
                    As::RealVector xyz = anglesToXyz(wavelength[i], gamma[i], nu[i], omega[i]);
                    As::RealVector hkl = xyzToHkl(ub, xyz[0], xyz[1], xyz[2]);
                    h.append(hkl[0]);
                    k.append(hkl[1]);
                    l.append(hkl[2]); } }

            // 4-circle geometry
            else {
                for (int i = 0; i < scan->numPoints(); ++i) {
                    As::RealVector xyz = anglesToXyz(wavelength[i], twotheta[i], omega[i], chi[i], phi[i]);
                    As::RealVector hkl = xyzToHkl(ub, xyz[0], xyz[1], xyz[2]);
                    h.append(hkl[0]);
                    k.append(hkl[1]);
                    l.append(hkl[2]); } }

            // Set indices to scan
            scan->setData("indices", "H", h.toQString());
            scan->setData("indices", "K", k.toQString());
            scan->setData("indices", "L", l.toQString()); } }

    // Calc direction cosines
    calcDirectionCosines(scan);
}

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
    omega = qDegreesToRadians(omega); // not used?!
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
                                                const qreal z,
                                                qreal psi) const
{
    // Calc ...
    const qreal q2xyz = As::Sqr(x) + As::Sqr(y) + As::Sqr(z);
    const qreal q2xy =  As::Sqr(x) + As::Sqr(y);

    // 4-circle geometry theta = omega
    //const qreal phi1 = M_PI - qAtan(y / x);
    //const qreal phi1 = qAtan2(-y, x); // from pron.c: phi0=atan2(-xyz[2], xyz[1])
    const qreal theta1 = qAsin(0.5 * wavelength * qSqrt(q2xyz));
    const qreal twotheta1 = 2 * theta1;
    qreal omega1 = theta1;
    qreal chi1 = qAtan(z / qSqrt(q2xy));
    qreal phi1 = qAtan2(-y, x);

    // Correct for psi
    correctForAzimuthAnglePsi(omega1, chi1, phi1, psi, q2xyz, z);

    // Lifting counter geometry
    const qreal omega2 = -qAtan2(y, x) + qAsin(0.5 * wavelength * q2xyz / qSqrt(q2xy)) - 0.5*M_PI; // GEO.sig?
    const qreal nu2 = qAsin(wavelength * z); // -0.5Pi to +0.5Pi
    const qreal twotheta2 = 2 * qAsin(0.5 * wavelength * qSqrt(q2xyz)); // 0 to +Pi
    const qreal gamma2 = qAcos(qCos(twotheta2) / qCos(nu2)); // GEO.sig?; 0 to +Pi or -Pi to 0

    // Convert angles
    QVector<qreal> angles({twotheta1, omega1, chi1, phi1, twotheta2, gamma2, nu2, omega2});
    for (qreal &value : angles)
        value = As::ToMainAngularRange(qRadiansToDegrees(value));

    return angles;
}

/*!
Corrects the input diffractometer angles \a omega, \a chi and \a phi for the azimuthal
angle \a psi (if non-zero).
*/
void As::ScanArray::correctForAzimuthAnglePsi(qreal &omega,
                                              qreal &chi,
                                              qreal &phi,
                                              qreal psi,
                                              const qreal q2xyz,
                                              const qreal z) const
{
    // Return if psi is not defined or zero
    if (qIsNaN(psi) OR psi == 0.)
        return;

    // Convert psi to radians
    psi = qDegreesToRadians(psi);

    // Case, when: -0.02 deg < chi < 0.02 deg
    if (qAbs(chi) < 3.5e-4) {
        omega -= 0.5*M_PI;
        chi   += psi;
        phi   -= 0.5*M_PI * As::Sign(-qCos(chi)); }

    // Case, when: 89.9994 deg < chi < 90.0006 deg
    else if (qAbs(qCos(chi)) < 1.0e-5) {
        phi -= psi * As::Sign(-chi); }

    // All other cases (most probable ones by the way)
    else {
        const qreal o = qAtan( qSin(psi) * qCos(chi) * qSqrt(q2xyz) / z );
        omega -= o;
        chi    = qAtan2( z / (qSqrt(q2xyz) * qCos(o)), qCos(chi) * qCos(psi) );
        phi   -= qAtan2( -qSin(o) / qCos(chi), qCos(o) ); }
}

/*!
Calculates the direction cosines of incident (s0) and diffracted (s2) beams.
*/
// Calculate direction cosines. Move to treat or pre-treat?
// Check all cases including lifting counter!
// Heidi orient matrix = ub.trans!
// make m_meanS0X, as m_meanIndexH?
void As::ScanArray::calcDirectionCosines(As::Scan *scan)
{
    // Check if ub matrix is read
    if (scan->data("orientation", "matrix").isEmpty())
        return;

    // Get data
    const As::RealMatrix9 ub = scan->data("orientation", "matrix");
    const As::RealVector twotheta = scan->data("angles", "2Theta");
    const As::RealVector omega = scan->data("angles", "Omega");
    const As::RealVector chi = scan->data("angles", "Chi");
    const As::RealVector phi = scan->data("angles", "Phi");
    const As::RealVector psi = scan->data("angles", "Phi");

    // Calculate or re-calculate angles which correspond to the 4-circle geometry
    qreal twothetaMean, omegaMean, chiMean, phiMean;
    if (twotheta.isEmpty() OR omega.isEmpty() OR chi.isEmpty() OR phi.isEmpty()) {
        const As::RealVector wavelength = scan->data("conditions", "Wavelength");
        const As::RealVector h = scan->data("indices", "H");
        const As::RealVector k = scan->data("indices", "K");
        const As::RealVector l = scan->data("indices", "L");
        const As::RealVector xyz = hklToXyz(ub, h.mean(), k.mean(), l.mean());
        const As::RealVector angles = xyzToAngles(wavelength.mean(), xyz[0], xyz[1], xyz[2], psi.mean());
        twothetaMean = angles[0];
        omegaMean = angles[1];
        chiMean = angles[2];
        phiMean = angles[3]; }
    else {
        twothetaMean = twotheta.mean();
        omegaMean = omega.mean();
        chiMean = chi.mean();
        phiMean = phi.mean();
    }

    // Get direction cosines for the selected scan
    const As::RealVector dc = directionCosines(ub, twothetaMean, omegaMean, chiMean, phiMean);

    // Save calculated direction cosines
    scan->setData("cosines", "S0X", QString::number(dc[0]));
    scan->setData("cosines", "S2X", QString::number(dc[1]));
    scan->setData("cosines", "S0Y", QString::number(dc[2]));
    scan->setData("cosines", "S2Y", QString::number(dc[3]));
    scan->setData("cosines", "S0Z", QString::number(dc[4]));
    scan->setData("cosines", "S2Z", QString::number(dc[5]));
}

/*!
Returns the calculated direction cosines of incident (s0) and diffracted (s2) beams in the form
of As::RealVector with the following order: s0x, s2x, s0y, s2y, s0z, s2z. Input parameters:
UB matrix \a ub and scattering angles \a twotheta, \a omega, \a chi, \a phi (Four-circle geometry).
*/
const As::RealVector As::ScanArray::directionCosines(const As::RealMatrix9 &ub,
                                                     qreal twotheta,
                                                     qreal omega,
                                                     qreal chi,
                                                     qreal phi) const
{
    // Convert diffractometer angles from degrees to radians
    twotheta = qDegreesToRadians(twotheta);
    omega = qDegreesToRadians(omega);
    chi = qDegreesToRadians(chi);
    phi = qDegreesToRadians(phi);

    // Check for negative 2theta!!!
    //if (reflextype==1) { // reflextype 1 means measured at negative 2theta and omega
    //    stth=-stth; // stth=sin(ttheta); somg=sin(omega);
    //    somg=-somg; }

    // Calculate intermediate variables
    const qreal o =  qCos(omega) * qCos(chi) * qCos(phi) - qSin(omega) * qSin(phi);
    const qreal p = -qCos(omega) * qCos(chi) * qSin(phi) - qSin(omega) * qCos(phi);
    const qreal q =  qCos(omega) * qSin(chi);

    const qreal x =  qSin(omega) * qCos(chi) * qCos(phi) + qCos(omega) * qSin(phi);
    const qreal y = -qSin(omega) * qCos(chi) * qSin(phi) + qCos(omega) * qCos(phi);
    const qreal z =  qSin(omega) * qSin(chi);

    // Get normalized ub matrix
    As::RealMatrix9 ubn = ub.normRows();

    // Calculate direction cosines of incident beam
    const qreal incidentX = x * ubn[0] + y * ubn[1] + z * ubn[2];
    const qreal incidentY = x * ubn[3] + y * ubn[4] + z * ubn[5];
    const qreal incidentZ = x * ubn[6] + y * ubn[7] + z * ubn[8];

    // Calculate direction cosines of diffracted beams
    const qreal opqX = o * ubn[0] + p * ubn[1] + q * ubn[2];
    const qreal opqY = o * ubn[3] + p * ubn[4] + q * ubn[5];
    const qreal opqZ = o * ubn[6] + p * ubn[7] + q * ubn[8];

    const qreal diffractedX = qSin(twotheta) * opqX - qCos(twotheta) * incidentX;
    const qreal diffractedY = qSin(twotheta) * opqY - qCos(twotheta) * incidentY;
    const qreal diffractedZ = qSin(twotheta) * opqZ - qCos(twotheta) * incidentZ;

    return As::RealVector({incidentX, diffractedX, incidentY, diffractedY, incidentZ, diffractedZ});
}



