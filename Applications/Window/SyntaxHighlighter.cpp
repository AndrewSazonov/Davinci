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

#include <QRegExp>
#include <QString>
#include <QTextCharFormat>
#include <QTextDocument>

#include "Colors.hpp"

#include "SyntaxHighlighter.hpp"

/*!
...
*/
As::SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent,
                                         const QString &type)
    : QSyntaxHighlighter(parent)
{
    load(type);
}

/*!
...
*/
void As::SyntaxHighlighter::load(const QString &type)
{
    //QStringList major, minor, medium, grayed, hidden;
    //Use enum InputFileType here and in SyntaxHighlighter?!

    if (type == "HEiDi DIF4 dat") {
        append(As::blueDark, "[A-Za-z]"); }

    if (type == "HEiDi DIF4 log") {
        append(As::green,    "^ (Orienting|Matrix|\\(A\\* B\\* C\\*\\))[^\n]*");
        append(As::redDark,  "![^\n]*");
        append(As::blueDark, "^ #[^\n]*"); }

    if (type == "POLI NICOS dat") {
        append(As::gray,     "^# .*:.*[^\n]*"); // line start, #, space, any symbol any time, :, any symbol any time, up to line end
        append(As::green,    "^# .*;.*[^\n]*");
        append(As::redDark,  "^###[^\n]*"); // doesn't color 1st line in the multiple opened files
        append(As::blueDark, "(filename|Exp_proposal|Exp_users|Sample_ubmatrix|Sample_rmat)[^\n]*");
        append(As::blueDark, "(fug_value|Pin_value|Pout_value|Fin_value|Fout_value|Ts_value|wavelength_value)[^\n]*");
        append(As::blueDark, "(chi1_value|chi2_value|omega_value|sth_value|twotheta_value|gamma_value|liftingctr_value)[^\n]*"); }

    if (type == "POLI IgorPro log") {
        append(As::gray,     "^  =>[^\n]*");
        append(As::green,    "(WARNING|Error|STOP)[^\n]*");
        append(As::redDark,  "(^(•|�)//|^(•|�)///|///)[^\n]*"); // are brackets needed???
        append(As::blueDark, "^(•|�)[^\n]*"); }

    if (type == "TASP dat") {
        append(As::gray,     "^([A-Za-z]|    )[^\n]*");
        append(As::green,    "^PARAM[^\n]*");
        append(As::redDark,  "(LOCAL|EXPNO|FILE|USER|INSTR|COMND|DATE|TITLE)[^\n]*");
        append(As::blueDark, "^ PNT[^\n]*"); }

    if (type == "6T2 BRALEVAN dat") {
        append(As::redDark,  "^FILE[^\n]*");
        append(As::blueDark, "^ANGLES[^\n]*"); }

    if (type == "6T2 NEW xml") {
        append(As::blueDark,      "<[^>]*>"); // [^>]* - everything but '>' as much as possible times
        append(As::grayLightVery, "</[^>]*>");
        append(As::grayLightVery, "(<|>)"); }
}

/*!
...
*/
void As::SyntaxHighlighter::append(const As::Color &color,
                                   const QString &pattern)
{
    QTextCharFormat format;
    format.setForeground(color);

    HighlightingRule rule;
    rule.m_format = format;
    rule.m_pattern = QRegExp(pattern); // rule.pattern = QRegularExpression(pattern);

    m_highlightingRules.append(rule);
}

/*!
...
*/
void As::SyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : m_highlightingRules) {

        QRegExp expression(rule.m_pattern); // QRegularExpression expression(rule.pattern);
        int index = expression.indexIn(text);

        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.m_format);
            index = expression.indexIn(text, index + length); } }

    setCurrentBlockState(0);
}
