#include <QtWidgets>

#include "codeeditor.h"



CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    
	//  set the highlight
    
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

    int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;     //已改

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
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

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
    painter.fillRect(event->rect(),  QColor(85, 85, 127));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::white);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

//查找
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

//替换
void CodeEditor::replace(QString str1, QString str2, bool backward, bool matchCase,
                      bool regExp)
{
    QTextCursor cursor = textCursor();

    if (!cursor.hasSelection())
    {
        search(str1, backward, matchCase, regExp);
    }
    else
    {
        int pos = textCursor().position() - textCursor().selectedText().length();

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

//替换所有
void CodeEditor::replaceAll(QString str1, QString str2, bool matchCase,
                         bool regExp)
{
    QTextCursor cursor = textCursor();

    cursor.setPosition(0, QTextCursor::MoveAnchor);
    setTextCursor(cursor);

    while (search(str1, false, matchCase, regExp))
        replace(str1, str2, false, matchCase, regExp);
}

//转到行
void CodeEditor::gotoLine(int lineNumber)
{
    QTextCursor cursor = textCursor();
    QTextBlock block = document()->findBlockByLineNumber(lineNumber - 1);
    cursor.setPosition(block.position());
    setTextCursor(cursor);
}

//文本内容发生改变触发的槽
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

//光标可见
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

}    //转为大写
void CodeEditor::toLowerCase()
{
    convertCase(false);
}   //转为小写

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
} //文本的段落数发生改变时触发的槽

//设置段落状态
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

                    previousBlockState = (previousBlockState & End ? Nested : Begin)
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
        if (previousBlockState & Comment && previousBlockState & (Nested | Begin))
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

    QAction *act = menu.addAction(tr("&撤消"), this, SLOT(undo())); //, QKeySequence::Undo);
    act->setEnabled(document()->isUndoAvailable());
    act = menu.addAction(tr("&重做"), this, SLOT(redo()));  //, QKeySequence::Redo);
    act->setEnabled(document()->isRedoAvailable());
    menu.addSeparator();
    act = menu.addAction(tr("剪切"), this, SLOT(cut()));     //, QKeySequence::Cut);
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("复制"), this, SLOT(copy()));  //, QKeySequence::Copy);
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("粘贴"), this, SLOT(paste())); //, QKeySequence::Paste);
    act->setEnabled(canPaste());
    act = menu.addAction(tr("删除"), this, SLOT(deleteSelected())); //, QKeySequence::Delete);
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("全选"), this, SLOT(selectAll())); //, QKeySequence::SelectAll);
    act->setEnabled(!document()->isEmpty());
    menu.addSeparator();
    act = menu.addAction(tr("大写转换"), this, SLOT(toUpperCase()));
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("小写转换"), this, SLOT(toLowerCase()));
    act->setEnabled(textCursor().hasSelection());

    menu.exec(event->globalPos());
}
