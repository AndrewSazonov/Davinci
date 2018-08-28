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

#include "RealVector.hpp"

namespace As { //AS_BEGIN_NAMESPACE

class TestRealVector : public QObject {
    Q_OBJECT

    QVector<const char*> m_dataNames;

    QVector<As::RealVector> m_input;

    QVector<As::RealVector> m_reverse;

    QVector<qreal> m_min;
    QVector<qreal> m_max;
    QVector<qreal> m_sum;
    QVector<qreal> m_sumSqr;
    QVector<qreal> m_mean;
    QVector<qreal> m_range;
    QVector<qreal> m_middle;
    QVector<qreal> m_step;

  private slots:

    void initTestCase();

    //void reverse_data();
    //void reverse();

    void min_data();
    void min();
    /*void max_data();
    void max();
    void sum_data();
    void sum();
    void sumSqrt_data();
    void sumSqrt();
    void mean_data();
    void mean();
    void range_data();
    void range();
    void middle_data();
    void middle();
    void step_data();
    void step();*/

};

void TestRealVector::initTestCase() {
    m_dataNames.append("random integer-like vector with positive values only; from QVector<qreal>");
    m_input.append(QVector<qreal>{ 2., 3., 1., 5., 4. });
    m_reverse.append(QVector<qreal>{ 4., 5., 1., 3., 2. });
    m_min.append(10.9999911111);
    m_max.append(5.);
    m_sum.append(15.);
    m_sumSqr.append(55.);
    m_mean.append(3.);
    m_range.append(4.);
    m_middle.append(3.);
    m_step.append(1.);

 }

void TestRealVector::min_data() {
    QTest::addColumn<RealVector>("input");
    QTest::addColumn<qreal>("result");
    for (int i = 0; i < m_input.size(); ++i) {
        QTest::newRow(m_dataNames[i]) << m_input[i] << m_min[i]; } }

void TestRealVector::min() {
    QFETCH(RealVector, input);
    QFETCH(qreal, result);
    QCOMPARE(input.min(), result); }


} //AS_END_NAMESPACE

QTEST_APPLESS_MAIN(As::TestRealVector)
#include "TestRealVector.moc"
