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

    QVector<const char*> m_info;

    QVector<As::RealVector> m_vectors;

    QVector<As::RealVector> m_reversed;

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

    void reverse_data();
    void reverse();

    void min_data();
    void min();
    void max_data();
    void max();
    void sum_data();
    void sum();
    void sumSqr_data();
    void sumSqr();
    void mean_data();
    void mean();
    void range_data();
    void range();
    void middle_data();
    void middle();
    void step_data();
    void step();

};

// init

void TestRealVector::initTestCase() {
    m_info.append("random integer-like vector with positive values only; from QVector<qreal>");
    m_vectors.append(QVector<qreal> { 2., 3., 1., 5., 4. });
    m_reversed.append(QVector<qreal> { 4., 5., 1., 3., 2. });
    m_min.append(1.);
    m_max.append(5.);
    m_sum.append(15.);
    m_sumSqr.append(55.);
    m_mean.append(3.);
    m_range.append(4.);
    m_middle.append(3.);
    m_step.append(1.);

    m_info.append("random integer-like vector with negative values only; from QString");
    m_vectors.append(QString("-2. -3. -1. -5. -4."));
    m_reversed.append(QString("-4. -5. -1. -3. -2."));
    m_min.append(-5.);
    m_max.append(-1.);
    m_sum.append(-15.);
    m_sumSqr.append(55.);
    m_mean.append(-3.);
    m_range.append(4.);
    m_middle.append(-3.);
    m_step.append(1.);

    m_info.append("random vector with both positive and negative values; from QString");
    m_vectors.append(QString("5.5 -3. 0. 5.5 -4. 0.5"));
    m_reversed.append(QString("0.5 -4. 5.5 0. -3. 5.5"));
    m_min.append(-4.);
    m_max.append(5.5);
    m_sum.append(4.5);
    m_sumSqr.append(85.75);
    m_mean.append(0.75);
    m_range.append(9.5);
    m_middle.append(0.75);
    m_step.append(1.9); }

// test next method

void TestRealVector::reverse_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<RealVector>("reverse");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_reversed[i]; } }

void TestRealVector::reverse() {
    QFETCH(RealVector, vector);
    QFETCH(RealVector, reverse);
    QCOMPARE(vector.reverse(), reverse); }

// test next method

void TestRealVector::min_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<qreal>("min");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_min[i]; } }

void TestRealVector::min() {
    QFETCH(RealVector, vector);
    QFETCH(qreal, min);
    QCOMPARE(vector.min(), min); }

// test next method

void TestRealVector::max_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<qreal>("max");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_max[i]; } }

void TestRealVector::max() {
    QFETCH(RealVector, vector);
    QFETCH(qreal, max);
    QCOMPARE(vector.max(), max); }

// test next method

void TestRealVector::sum_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<qreal>("sum");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_sum[i]; } }

void TestRealVector::sum() {
    QFETCH(RealVector, vector);
    QFETCH(qreal, sum);
    QCOMPARE(vector.sum(), sum); }

// test next method

void TestRealVector::sumSqr_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<qreal>("sumSqr");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_sumSqr[i]; } }

void TestRealVector::sumSqr() {
    QFETCH(RealVector, vector);
    QFETCH(qreal, sumSqr);
    QCOMPARE(vector.sumSqr(), sumSqr); }

// test next method

void TestRealVector::mean_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<qreal>("mean");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_mean[i]; } }

void TestRealVector::mean() {
    QFETCH(RealVector, vector);
    QFETCH(qreal, mean);
    QCOMPARE(vector.mean(), mean); }

// test next method

void TestRealVector::range_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<qreal>("range");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_range[i]; } }

void TestRealVector::range() {
    QFETCH(RealVector, vector);
    QFETCH(qreal, range);
    QCOMPARE(vector.range(), range); }

// test next method

void TestRealVector::middle_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<qreal>("middle");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_middle[i]; } }

void TestRealVector::middle() {
    QFETCH(RealVector, vector);
    QFETCH(qreal, middle);
    QCOMPARE(vector.middle(), middle); }

// test next method

void TestRealVector::step_data() {
    QTest::addColumn<RealVector>("vector");
    QTest::addColumn<qreal>("step");
    for (int i = 0; i < m_vectors.size(); ++i) {
        QTest::newRow(m_info[i]) << m_vectors[i] << m_step[i]; } }

void TestRealVector::step() {
    QFETCH(RealVector, vector);
    QFETCH(qreal, step);
    QCOMPARE(vector.step(), step); }

} //AS_END_NAMESPACE

QTEST_APPLESS_MAIN(As::TestRealVector)
#include "TestRealVector.moc"
