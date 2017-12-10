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

#include "catch.hpp"

#include "RealVector.hpp"

TEST_CASE( "As::Vector Class", "[As::Vector]" )
{
    /// Arrays of the variables
    QVector<As::RealVector> vector;
    QVector<As::RealVector> vectorReversed;
    QVector<qreal> min;
    QVector<qreal> max;
    QVector<qreal> sum;
    QVector<qreal> sumSqr;
    QVector<qreal> mean;
    QVector<qreal> range;
    QVector<qreal> middle;
    QVector<qreal> step;

    /// Add test vector of the positive values
    vector.append        (QVector<qreal>{2., 3., 1., 5., 4.});
    vectorReversed.append(QVector<qreal>{4., 5., 1., 3., 2.});
    min.append   (1.);
    max.append   (5.);
    sum.append   (15.);
    sumSqr.append(55.);
    mean.append  (3.);
    range.append (4.);
    middle.append(3.);
    step.append  (1.);

    /// Add test vector of the negative values
    vector.append        (QVector<qreal>{-2., -3., -1., -5., -4.});
    vectorReversed.append(QVector<qreal>{-4., -5., -1., -3., -2.});
    min.append   (-5.);
    max.append   (-1.);
    sum.append  (-15.);
    sumSqr.append(55.);
    mean.append  (-3.);
    range.append  (4.);
    middle.append(-3.);
    step.append   (1.);

    /// Add test vector of the positive and negative values
    vector.append        (QVector<qreal>{5.5, -3., 0.,  5.5, -4., 0.5});
    vectorReversed.append(QVector<qreal>{0.5, -4., 5.5, 0.,  -3., 5.5});
    min.append   (-4.);
    max.append    (5.5);
    sum.append    (4.5);
    sumSqr.append(85.75);
    mean.append   (0.75);
    range.append  (9.5);
    middle.append (0.75);
    step.append   (1.9);

    SECTION("min() method") {
        for (int i = 0; i < vector.size(); ++i)
            CHECK(vector[i].min() == min[i]); }

    SECTION("max() method") {
        for (int i = 0; i < vector.size(); ++i)
            CHECK(vector[i].max() == max[i]); }

    SECTION("mean() method") {
        for (int i = 0; i < vector.size(); ++i)
            CHECK(vector[i].mean() == mean[i]); }

    SECTION("range() method") {
        for (int i = 0; i < vector.size(); ++i)
            CHECK(vector[i].range() == range[i]); }

    SECTION("middle() method") {
        for (int i = 0; i < vector.size(); ++i)
            CHECK(vector[i].middle() == middle[i]); }

    SECTION("step() method") {
        for (int i = 0; i < vector.size(); ++i)
            CHECK(vector[i].step() == step[i]); }

    SECTION("inv() method") {
        for (int i = 0; i < vector.size(); ++i) {
            for (int k = 0; k < vector[i].size(); ++k)
                CHECK(vector[i].reverse()[k] == vectorReversed[i][k]); } }
}

