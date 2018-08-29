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

#include <QVector>
#include <QtTest/QtTest>

#include "RealMatrix9.hpp"

namespace As { //AS_BEGIN_NAMESPACE

class TestRealMatrix9 : public QObject {
    Q_OBJECT

    QVector<const char*> m_info;

    QVector<RealMatrix9> m_matrices;

    QVector<RealMatrix9> m_transposed;
    QVector<RealMatrix9> m_inversed;

    QVector<qreal> m_determinants;

  private slots:
    void initTestCase();

    void det_data();
    void det();
    void trans_data();
    void trans();
    void inv_data();
    void inv();

};

// init

void TestRealMatrix9::initTestCase() {
    m_info.append("identity matrix");
    m_matrices.append(RealMatrix9( 1., 0., 0.,   0., 1., 0.,   0., 0., 1. ));
    m_transposed.append(RealMatrix9( 1., 0., 0.,   0., 1., 0.,   0., 0., 1. ));
    m_inversed.append(RealMatrix9( 1., 0., 0.,   0., 1., 0.,   0., 0., 1. ));
    m_determinants.append(1.);

    m_info.append("random integer-like matrix with non-zero determinant");
    m_matrices.append(RealMatrix9( 2., 1., -2.,   3., 2., 4.,   2., 1., -1. ));
    m_transposed.append(RealMatrix9( 2., 3., 2.,   1., 2., 1.,   -2., 4., -1. ));
    m_inversed.append(RealMatrix9( -6., -1., 8.,   11., 2., -14.,   -1., 0., 1. ));
    m_determinants.append(1.); }

// test next method

void TestRealMatrix9::det_data() {
    QTest::addColumn<RealMatrix9>("matrix");
    QTest::addColumn<qreal>("det");
    for (int i = 0; i < m_matrices.size(); ++i) {
        QTest::newRow(m_info[i]) << m_matrices[i] << m_determinants[i]; } }

void TestRealMatrix9::det() {
    QFETCH(RealMatrix9, matrix);
    QTEST(matrix.det(), "det"); }

// test next method

void TestRealMatrix9::trans_data() {
    QTest::addColumn<RealMatrix9>("matrix");
    QTest::addColumn<RealMatrix9>("trans");
    for (int i = 0; i < m_matrices.size(); ++i) {
        QTest::newRow(m_info[i]) << m_matrices[i] << m_transposed[i]; } }

void TestRealMatrix9::trans() {
    QFETCH(RealMatrix9, matrix);
    QTEST(matrix.trans(), "trans"); }

// test next method

void TestRealMatrix9::inv_data() {
    QTest::addColumn<RealMatrix9>("matrix");
    QTest::addColumn<RealMatrix9>("inv");
    for (int i = 0; i < m_matrices.size(); ++i) {
        QTest::newRow(m_info[i]) << m_matrices[i] << m_inversed[i]; } }

void TestRealMatrix9::inv() {
    QFETCH(RealMatrix9, matrix);
    QTEST(matrix.inv(), "inv"); }

} //AS_END_NAMESPACE

QTEST_APPLESS_MAIN(As::TestRealMatrix9)
#include "TestRealMatrix9.moc"
