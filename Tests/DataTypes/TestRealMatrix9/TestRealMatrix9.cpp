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

  private slots:
    void initTestCase();

    void det_data();
    void det();
    void trans_data();
    void trans();
    void inv_data();
    void inv();

  private:
    QVector<const char*> dataNames;

    QVector<RealMatrix9> matrices;

    QVector<RealMatrix9> transposedMatrices;
    QVector<RealMatrix9> inversedMatrices;

    QVector<qreal> determinants;

};

void TestRealMatrix9::initTestCase() {
    dataNames.append("identity matrix");
    matrices.append(RealMatrix9( 1., 0., 0.,   0., 1., 0.,   0., 0., 1. ));
    transposedMatrices.append(RealMatrix9( 1., 0., 0.,   0., 1., 0.,   0., 0., 1. ));
    inversedMatrices.append(RealMatrix9( 1., 0., 0.,   0., 1., 0.,   0., 0., 1. ));
    determinants.append(1.);

    dataNames.append("random integer-like matrix with non-zero determinant");
    matrices.append(RealMatrix9( 2., 1., -2.,   3., 2., 4.,   2., 1., -1. ));
    transposedMatrices.append(RealMatrix9( 2., 3., 2.,   1., 2., 1.,   -2., 4., -1. ));
    inversedMatrices.append(RealMatrix9( -6., -1., 8.,   11., 2., -14.,   -1., 0., 1. ));
    determinants.append(1.); }

void TestRealMatrix9::det_data() {
    QTest::addColumn<RealMatrix9>("input");
    QTest::addColumn<qreal>("result");
    for (int i = 0; i < matrices.size(); ++i) {
        QTest::newRow(dataNames[i]) << matrices[i] << determinants[i]; } }

void TestRealMatrix9::det() {
    QFETCH(RealMatrix9, input);
    QFETCH(qreal, result);
    QCOMPARE(input.det(), result); }

void TestRealMatrix9::trans_data() {
    QTest::addColumn<RealMatrix9>("input");
    QTest::addColumn<RealMatrix9>("result");
    for (int i = 0; i < matrices.size(); ++i) {
        QTest::newRow(dataNames[i]) << matrices[i] << transposedMatrices[i]; } }

void TestRealMatrix9::trans() {
    QFETCH(RealMatrix9, input);
    QFETCH(RealMatrix9, result);
    QCOMPARE(input.trans(), result); }

void TestRealMatrix9::inv_data() {
    QTest::addColumn<RealMatrix9>("input");
    QTest::addColumn<RealMatrix9>("result");
    for (int i = 0; i < matrices.size(); ++i) {
        QTest::newRow(dataNames[i]) << matrices[i] << inversedMatrices[i]; } }

void TestRealMatrix9::inv() {
    QFETCH(RealMatrix9, input);
    QFETCH(RealMatrix9, result);
    QCOMPARE(input.inv(), result); }

} //AS_END_NAMESPACE

QTEST_APPLESS_MAIN(As::TestRealMatrix9)
#include "TestRealMatrix9.moc"
