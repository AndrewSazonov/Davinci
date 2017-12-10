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

#ifndef AS_TEXTEDITOR_HPP
#define AS_TEXTEDITOR_HPP

#include "codeeditor.h"

class QPaintEvent;

namespace As { //AS_BEGIN_NAMESPACE

class TextEditor : public CodeEditor
{
    Q_OBJECT

public:
    TextEditor();
    ~TextEditor();

    void paintEvent(QPaintEvent* event);

    void setCurrentLineSelections(const QList<QTextEdit::ExtraSelection> &selections);
    QList<QTextEdit::ExtraSelection> currentLineSelections() const;

    void setCurrentScanSelections(const QList<QTextEdit::ExtraSelection> &selections);
    QList<QTextEdit::ExtraSelection> currentScanSelections() const;

    void setFoundTextSelections(const QList<QTextEdit::ExtraSelection> &selections);
    QList<QTextEdit::ExtraSelection> foundTextSelections() const;

public slots:
    void getCursorPosition();
    void setCursorPosition(int lineIndex);

    void highlightCurrentLine();

    void clearAllSelections();

    void setPlainText(const QString &text); // override the default slot

signals:
    void cursorPositionChanged(const int lineIndex); // override the default signal

private:
    void setAllExtraSelections();

    QList<QTextEdit::ExtraSelection> m_currentLineSelections;
    QList<QTextEdit::ExtraSelection> m_currentScanSelections;
    QList<QTextEdit::ExtraSelection> m_foundTextSelections;

};

} //AS_END_NAMESPACE

#endif // AS_TEXTEDITOR_HPP


