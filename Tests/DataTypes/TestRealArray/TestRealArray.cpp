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

#include <QString>
#include <QVector>

#include <QtTest/QtTest>

#include "RealArray.hpp"

namespace As { //AS_BEGIN_NAMESPACE

class TestRealArray : public QObject {
    Q_OBJECT

  private slots:

    void at_data();
    void at();

    void isEmpty_data();
    void isEmpty();

    void size_data();
    void size();

    void indexOf_data();
    void indexOf();

    void toQVector_data();
    void toQVector();

    void toQString_data();
    void toQString();

};

// test next method

void TestRealArray::at_data() {
    QTest::addColumn<RealArray>("array");
    QTest::addColumn<qreal>("result");

    QTest::newRow("3-equal-elements array") << RealArray(3, 0.1) << 0.1;
    QTest::newRow("5-different-elements array") << RealArray("1 2 3 4 5") << 3.0; }

void TestRealArray::at() {
    QFETCH(RealArray, array);
    QFETCH(qreal, result);

    QCOMPARE(array.at(2), result); }


// test next method

void TestRealArray::isEmpty_data() {
    QTest::addColumn<RealArray>("array");
    QTest::addColumn<bool>("result");

    QTest::newRow("empty array") << RealArray() << true;
    QTest::newRow("non-empty array") << RealArray(3, 0.1) << false; }

void TestRealArray::isEmpty() {
    QFETCH(RealArray, array);
    QFETCH(bool, result);

    QCOMPARE(array.isEmpty(), result); }

// test next method

void TestRealArray::size_data() {
    QTest::addColumn<RealArray>("array");
    QTest::addColumn<int>("result");

    QTest::newRow("empty array") << RealArray() << 0;
    QTest::newRow("3-equal-elements array") << RealArray(1, 0.1) << 1;
    QTest::newRow("2-elements array") << RealArray(2, 0.0) << 2;
    QTest::newRow("100-elements array") << RealArray(100, -0.1) << 100; }

void TestRealArray::size() {
    QFETCH(RealArray, array);
    QFETCH(int, result);

    QCOMPARE(array.size(), result); }

// test next method

void TestRealArray::indexOf_data() {
    QTest::addColumn<RealArray>("array");
    QTest::addColumn<int>("result");

    QTest::newRow("3-equal-elements array, all positions") << RealArray(3, 0.1) << 0;
    QTest::newRow("3-different-elements array, 1st position") << RealArray("0.1 2.0 3.4") << 0;
    QTest::newRow("3-different-elements array, 2nd position") << RealArray("-0.1 0.1 3.4") << 1;
    QTest::newRow("3-different-elements array, 3rd position") << RealArray("100.1 2.0 0.1") << 2;
    QTest::newRow("5-different-elements array, no occurrences") << RealArray("1 2 3 4 5") << -1;
    QTest::newRow("empty array, no occurrences") << RealArray() << -1; }

void TestRealArray::indexOf() {
    QFETCH(RealArray, array);
    QFETCH(int, result);

    QCOMPARE(array.indexOf(0.1), result); }

// test next method

void TestRealArray::toQVector_data() {
    QTest::addColumn<RealArray>("array");
    QTest::addColumn<QVector<qreal>>("result");

    QTest::newRow("empty array") << RealArray() << QVector<qreal>();
    QTest::newRow("3-equal-elements array, real values") << RealArray(3, 0.1) << QVector<qreal> {0.1, 0.1, 0.1 };
    QTest::newRow("3-equal-elements array, int values") << RealArray(3, 5) << QVector<qreal> {5, 5, 5 };
    QTest::newRow("5-different-elements array") << RealArray("-1 2 0 4 99") << QVector<qreal> {-1, 2, 0, 4, 99 }; }

void TestRealArray::toQVector() {
    QFETCH(RealArray, array);
    QFETCH(QVector<qreal>, result);

    QCOMPARE(array.toQVector(), result); }

// test next method

void TestRealArray::toQString_data() {
    QTest::addColumn<RealArray>("array");
    QTest::addColumn<QString>("result");

    QTest::newRow("empty array") << RealArray() << "";
    QTest::newRow("3-equal-elements array, real values") << RealArray(3, 0.1) << "0.1 0.1 0.1";
    QTest::newRow("3-equal-elements array, int values") << RealArray(3, 5) << "5 5 5";
    QTest::newRow("5-different-elements array") << RealArray("-1 2 0 4 99") << "-1 2 0 4 99"; }

void TestRealArray::toQString() {
    QFETCH(RealArray, array);
    QFETCH(QString, result);

    QCOMPARE(array.toQString(), result); }

} //AS_END_NAMESPACE

QTEST_APPLESS_MAIN(As::TestRealArray)
#include "TestRealArray.moc"
