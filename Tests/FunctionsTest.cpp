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

#include <QVector>

#include "catch.hpp"

#include "Functions.hpp"

TEST_CASE( "As::Functions", "[As::Functions]" )
{
    SECTION("As::Sqr function") {
        QVector<qreal> value{ -10., -1., 0., 1., 2.,  4., 10.2,    20.001   };
        QVector<qreal> answer{100.,  1., 0., 1., 4., 16., 104.04, 400.040001};
        for (int i = 0; i < value.size(); ++i)
            CHECK(As::Sqr(value[i]) == Approx(answer[i])); }

    SECTION("As::ToMainAngularRange function") {
        QVector<qreal> value{ -360., -200., -180., -90., 0., 90., 180.,  200., 360., 380., 740.};
        QVector<qreal> answer{   0.,  160., -180., -90., 0., 90., 180., -160.,   0.,  20.,  20.};
        for (int i = 0; i < value.size(); ++i)
            CHECK(As::ToMainAngularRange(value[i]) == Approx(answer[i])); }
}

