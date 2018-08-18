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

#ifndef AS_SYNTAXHIGHLIGHTER_HPP
#define AS_SYNTAXHIGHLIGHTER_HPP

#include <QSyntaxHighlighter>

class QRegExp;
class QString;
class QTextCharFormat;
class QTextDocument;

namespace As { //AS_BEGIN_NAMESPACE

class Color;

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

  public:
    SyntaxHighlighter(QTextDocument* parent = Q_NULLPTR,
                      const QString& type = QString());

    void load(const QString& type);
    void append(const As::Color& color,
                const QString& pattern);

  protected:
    void highlightBlock(const QString& text) Q_DECL_OVERRIDE;

  private:
    struct HighlightingRule {
        QRegExp m_pattern; //QRegularExpression pattern;
        QTextCharFormat m_format; };
    QVector<HighlightingRule> m_highlightingRules; };

} //AS_END_NAMESPACE

#endif // AS_SYNTAXHIGHLIGHTER_HPP
