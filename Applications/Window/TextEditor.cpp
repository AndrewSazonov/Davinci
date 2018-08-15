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

#include <QDebug>
#include <QFont>
#include <QPaintEvent>
#include <QSettings>
#include <QTextBlock>

#include "Colors.hpp"
#include "Functions.hpp"
#include "Macros.hpp"

#include "TextEditor.hpp"

/*!
    \class As::TextEditor

    \brief The TextEditor is a custom class based on the CodeEditor.

    \inmodule Main
*/

/*!
    Constructs a text editor.
*/
As::TextEditor::TextEditor() {
    setReadOnly(true);
    setTextInteractionFlags(textInteractionFlags() | Qt::TextSelectableByKeyboard);
    setCenterOnScroll(true);

    //auto font = QSettings().value("TextSettings/font").value<QFont>();
    const QString family = QSettings().value("TextSettings/fontFamily").toString();
    const int size = QSettings().value("TextSettings/fontSize").toInt();
    if (!family.isEmpty() AND size > 0) {
        setFont(QFont(family, size)); }

    auto isTextWraped = QSettings().value("TextSettings/wrapText", false).toBool();
    setLineWrapMode(QPlainTextEdit::NoWrap);
    if (isTextWraped) {
        setLineWrapMode(QPlainTextEdit::WidgetWidth); }

    connect(this, &CodeEditor::cursorPositionChanged,
            this, &As::TextEditor::getCursorPosition);
    connect(this, &CodeEditor::cursorPositionChanged,
            this, &As::TextEditor::highlightCurrentLine); }

/*!
    Reimplements the paintEvent for highlighting
*/
void As::TextEditor::paintEvent(QPaintEvent* event) {
    CodeEditor::paintEvent(event); }

/*!
    Destroys the text editor.
*/
As::TextEditor::~TextEditor() {
    ADESTROYED; }

/*!
    ...
*/
bool equalSelections(const QList<QTextEdit::ExtraSelection>& first,
                     const QList<QTextEdit::ExtraSelection>& second) {
    if (first.size() != second.size()) {
        return false; }

    for (int i = 0; i < first.size(); ++i)
        if (first[i].cursor != second[i].cursor) {
            return false; }

    return true; }

/*!
    ...
*/
void As::TextEditor::setCurrentLineSelections(const QList<QTextEdit::ExtraSelection>& selections) {
    //ADEBUG << "selections.size():" << selections.size();

    if (selections.size() < 1) {
        return; }

    if (equalSelections(selections, m_currentLineSelections)) {
        return; }

    m_currentLineSelections = selections;
    setAllExtraSelections(); }

/*!
    ...
*/
QList<QTextEdit::ExtraSelection> As::TextEditor::currentLineSelections() const {
    return m_currentLineSelections; }

/*!
    ...
*/
void As::TextEditor::setCurrentScanSelections(const QList<QTextEdit::ExtraSelection>& selections) {
    //ADEBUG << "selections.size():" << selections.size();

    if (selections.size() < 1) {
        return; }

    if (equalSelections(selections, m_currentScanSelections)) {
        return; }

    m_currentScanSelections = selections;
    m_currentLineSelections.clear();
    setAllExtraSelections(); }

/*!
    ...
*/
QList<QTextEdit::ExtraSelection> As::TextEditor::currentScanSelections() const {
    return m_currentScanSelections; }

/*!
    ...
*/
void As::TextEditor::setFoundTextSelections(const QList<QTextEdit::ExtraSelection>& selections) {
    //ADEBUG << "selections.size():" << selections.size();

    if (equalSelections(selections, m_foundTextSelections)) {
        return; }

    m_foundTextSelections = selections;
    setAllExtraSelections(); }

/*!
    ...
*/
QList<QTextEdit::ExtraSelection> As::TextEditor::foundTextSelections() const {
    return m_foundTextSelections; }

/*!
    ...
*/
void As::TextEditor::setAllExtraSelections() {
    //ADEBUG;

    setExtraSelections(m_currentLineSelections + m_currentScanSelections + m_foundTextSelections); }

/*!
    ...
*/
void As::TextEditor::getCursorPosition() {
    //ADEBUG;

    const int lineIndex = textCursor().blockNumber() + 1;
    emit cursorPositionChanged(lineIndex); }

/*!
    ...
*/
void As::TextEditor::setCursorPosition(int lineIndex) {
    //ADEBUG << "lineIndex:" << lineIndex;

    const int cursorPos = document()->findBlockByLineNumber(lineIndex - 1).position();

    QTextCursor cursor = textCursor();
    cursor.setPosition(cursorPos);

    setTextCursor(cursor); }

/*!
    ...
*/
void As::TextEditor::highlightCurrentLine() {
    //ADEBUG;

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(As::Color(As::blueLightTransparent));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();

    QList<QTextEdit::ExtraSelection> selections;
    selections.append(selection);

    setCurrentLineSelections(selections); }

/*!
    ...
*/
void As::TextEditor::clearAllSelections() {
    m_currentLineSelections.clear();
    m_currentScanSelections.clear();
    m_foundTextSelections.clear(); }

/*!
    ...
*/
void As::TextEditor::setPlainText(const QString& text) {
    clearAllSelections();

    CodeEditor::setPlainText(text); }

