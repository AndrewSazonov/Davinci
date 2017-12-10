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

#include "RealMatrix9.hpp"

TEST_CASE( "As::RealMatrix9 Class", "[As::RealMatrix9]" )
{
    /// Arrays of the variables
    QVector<As::RealMatrix9> matrix;
    QVector<As::RealMatrix9> matrixTransposed;
    QVector<As::RealMatrix9> matrixInversed;
    QVector<qreal> determinant;

    /// Add the test matrix with non-zero determinant
    matrix.append          (As::RealMatrix9{ 2.,  1., -2.,  3., 2.,   4.,  2., 1., -1.});
    matrixTransposed.append(As::RealMatrix9{ 2.,  3.,  2.,  1., 2.,   1., -2., 4., -1.});
    matrixInversed.append  (As::RealMatrix9{-6., -1.,  8., 11., 2., -14., -1., 0.,  1.});
    determinant.append(1.);

    /*
    /// Add the test matrix with zero determinant
    matrix.append(RealMatrix9{1., 2., 3., 4., 5., 6., 7., 8., 9.});
    matrixTransposed.append(RealMatrix9{1., 4., 7., 2., 5., 8., 3., 6., 9.});
    matrixInversed.append(RealMatrix9{0., 0., 0., 0., 0., 0., 0., 0., 0.});
    determinant.append(0.);
    */

    SECTION("det method") {
        for (int i = 0; i < matrix.size(); ++i)
            CHECK(matrix[i].det() == determinant[i]); }

    SECTION("trans method") {
         for (int i = 0; i < matrix.size(); ++i)
             CHECK(matrix[i].trans() == matrixTransposed[i]); }

    SECTION("inv method") {
         for (int i = 0; i < matrix.size(); ++i) {
             for (int k = 0; k < matrix[i].size(); ++k)
                 CHECK(matrix[i].inv()[k] == matrixInversed[i][k]); } }

    SECTION("operator== method") {
        As::RealMatrix9 m1{2., 1., -2., 3., 0., 4., 2., 1., -1.};
        As::RealMatrix9 m2{2., 1., -2., 3., 0., 4., 2., 1., -1.};
        for (int i = 0; i < m1.size(); ++i) {
            CHECK(m1[i] == m2[i]); } }
}

