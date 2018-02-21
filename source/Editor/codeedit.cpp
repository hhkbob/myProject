/*================================C++========================================*\
Create: 2018/02/09
Creator: Huang Huakun
File Name: codeeditor.cpp
Platform: windows
Version: v1.0

Describe: The header of the software main source file.

Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
CopyRight:
*/

#include <QtWidgets>
#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), 
		    this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), 
		    this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), 
		    this, SLOT(highlightCurrentLine()));
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;  
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), 
		lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), 
		           lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(240, 240, 240).lighter(100);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(),  QColor(240, 240, 240));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
	int top;
	top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::gray);
            painter.drawText(0, top, lineNumberArea->width(),
				             fontMetrics().height(),Qt::AlignRight, number);                 
        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int CodeEditor::search(QString str, bool backward, bool matchCase, bool regExp)
{
    QTextDocument::FindFlags options;

    if (backward)
        options = QTextDocument::FindBackward;

    if (matchCase)
        options |= QTextDocument::FindCaseSensitively;

    QTextCursor cursor = textCursor();

    cursor =
            regExp ?
                document()->find(QRegExp(str), cursor, options) :
                document()->find(str, cursor, options);

    if (cursor.isNull())
        return false;

    setTextCursor(cursor);
    return true;
}

void CodeEditor::replace(QString str1, QString str2, bool backward, 
	                     bool matchCase, bool regExp)
{
    QTextCursor cursor = textCursor();

    if (!cursor.hasSelection())
    {
        search(str1, backward, matchCase, regExp);
    }
    else
    {
		int pos;
        pos = textCursor().position() - textCursor().selectedText().length();
        cursor.beginEditBlock();
        cursor.insertText(str2);
        cursor.endEditBlock();
        if (backward)
        {
            cursor.setPosition(pos, QTextCursor::MoveAnchor);
            setTextCursor(cursor);
        }
    }
}

void CodeEditor::replaceAll(QString str1, QString str2, bool matchCase,
                         bool regExp)
{
    QTextCursor cursor = textCursor();

    cursor.setPosition(0, QTextCursor::MoveAnchor);
    setTextCursor(cursor);

    while (search(str1, false, matchCase, regExp))
        replace(str1, str2, false, matchCase, regExp);
}

void CodeEditor::gotoLine(int lineNumber)
{
    QTextCursor cursor = textCursor();
    QTextBlock block = document()->findBlockByLineNumber(lineNumber - 1);
    cursor.setPosition(block.position());
    setTextCursor(cursor);
}

void CodeEditor::contentsChange(int pos, int, int)
{
    QTextBlock block = textCursor().block();

    if (block.userState() == Empty)
        block.setUserState(block.previous().userState() | Rehighligh);

    block = document()->findBlock(pos);

    int startBlockNum = block.blockNumber();

    while (block.isValid())
    {
        int previousState = block.userState();
        int state = setBlockState(block);

        if (!(previousState & Error) && !(state & Error) && previousState & Comment
                && state & Comment)
        {

            QTextBlock next = block.next();

            int nextBlockState = next.userState();
            setBlockState(next);
            next.setUserState(nextBlockState);
            state = block.userState();
        }

        if (block.blockNumber() > startBlockNum && state & Begin
                && !(state & Nested))
        {
            break;
        }

        block.setUserState(state | Rehighligh);

        block = block.next();
    }
}

void CodeEditor::ensureCursorVisible()
{
    QTextCursor cursor = textCursor();

    QTextBlock block = document()->findBlock(cursor.position());

    if (!block.isVisible())
    {

        while (!(block = block.previous()).isVisible())
            ;

        cursor.setPosition(block.position(), QTextCursor::MoveAnchor);
        setTextCursor(cursor);
    }
}

void CodeEditor::toUpperCase()
{
    convertCase(true );

}

void CodeEditor::toLowerCase()
{
    convertCase(false);
} 

void CodeEditor::convertCase(bool toUpper )
{
    textCursor().insertText(
                toUpper ?
                    textCursor().selectedText().toUpper() :
                    textCursor().selectedText().toLower());
}

void CodeEditor::blockCountChanged(int count)
{
    lineNumDigits = qMax(2, QString::number(count).length());
} 

int CodeEditor::setBlockState(QTextBlock &block)
{
    int previousBlockState = block.previous().userState();

    if (previousBlockState & Error)
        previousBlockState = 0;

    bool inString = previousBlockState & String;

    int previousBraceDepth = previousBlockState >> StateShift;

    int braceDepth = previousBraceDepth;

    QString txt = block.text();

    int state = 0;
    int i = 0;

    while (txt[i].isSpace())
        ++i;

    if (txt[i] == ';')
    {

        if (!previousBraceDepth || previousBlockState & Comment)
        {
            state |= Comment;
            if (previousBlockState & Comment)
            {

                if (braceDepth)
                {
                    braceDepth--;
                }
                else
                {
                    previousBraceDepth = 1;
                    previousBlockState=(previousBlockState &End ?Nested :Begin)
                            | (previousBlockState & Folded)
                            | (previousBlockState & Rehighligh);
                    previousBlockState |= (previousBraceDepth << StateShift)
                            | Comment;

                    block.previous().setUserState(previousBlockState);
                }

                state |= End;
            }
        }

    }
    else
    {
        if (previousBlockState &Comment &&previousBlockState &(Nested | Begin))
        {
            previousBraceDepth = 0;
            braceDepth = 0;
            previousBlockState = previousBlockState & Nested ? End : 0;
            previousBlockState |= (previousBraceDepth << StateShift) | Comment;
            block.previous().setUserState(previousBlockState);
        }

        if (txt[i].isNull() && !previousBraceDepth)
        {
            state = Empty;
        }
        else if (txt[i] != '(' && !previousBraceDepth)
        {
            braceDepth = i;
            state |= Error;
        }
        else
        {
            while (i < txt.length())
            {

                if (txt[i] == '"' && !(i && txt[i - 1] == '\\'))
                {
                    inString = !inString;
                }
                else if (!inString)
                {

                    if (txt[i] == ';')
                        break;

                    if (txt[i] == '(')
                        braceDepth++;
                    else if (txt[i] == ')')
                        braceDepth--;

                    if (braceDepth == -1)
                    {
                        braceDepth = i;
                        state |= Error;
                        break;
                    }
                }

                i++;
            }
        }
    }

    if (!(state & (Error | Comment)))
    {

        if (inString)
            state |= String;

        if (braceDepth > previousBraceDepth || !previousBraceDepth)
            state |= Begin;

        if (braceDepth < previousBraceDepth || !braceDepth)
            state |= End;

        if (previousBraceDepth && braceDepth)
            state |= Nested;
    }

    state |= (braceDepth << StateShift);

    int previousState = block.userState();

    if (previousState != Empty)
        state |= (previousState & Folded) | (previousState & Rehighligh);

    block.setUserState(state);

    return state;
}

void CodeEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction *act = menu.addAction(tr("&Undo"), this, SLOT(undo())); 
    act->setEnabled(document()->isUndoAvailable());
    act = menu.addAction(tr("&Redo"), this, SLOT(redo()));  
    act->setEnabled(document()->isRedoAvailable());
    menu.addSeparator();
    act = menu.addAction(tr("Cut"), this, SLOT(cut()));     
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("Copy"), this, SLOT(copy())); 
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("Paste"), this, SLOT(paste())); 
    act->setEnabled(canPaste());
    act = menu.addAction(tr("Delete"), this, SLOT(deleteSelected())); 
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("Select All"), this, SLOT(selectAll())); 
    act->setEnabled(!document()->isEmpty());
    menu.addSeparator();
    act = menu.addAction(tr("Upper"), this, SLOT(toUpperCase()));
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("Lower"), this, SLOT(toLowerCase()));
    act->setEnabled(textCursor().hasSelection());
    menu.exec(event->globalPos());
}
