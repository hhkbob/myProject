#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QTextBlock>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_FORWARD_DECLARE_CLASS (QContextMenuEvent)

class LineNumberArea;


class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    //block 类型
    enum BlockState
    {
        End = 1,
        Begin = 2,
        String = 4,
        Comment = 8,
        Nested = 16,
        Folded = 32,
        Error = 64,
        Rehighligh = 128,
        Empty = -1
    };
    const static int StateShift = 8;

public:
	CodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    int lineNumDigits;        
    void convertCase(bool toUpper ) ;   


protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    int search(QString, bool, bool, bool); 
    void replace(QString, QString, bool, bool, bool);   
    void replaceAll(QString, QString, bool, bool);  
    void gotoLine(int lineNumber);  
    void toUpperCase() ;

    void toLowerCase() ;

private slots:
    void deleteSelected()
    {	textCursor().removeSelectedText();}    

    void blockCountChanged(int count) ;

    void contentsChange(int, int, int); 
    void ensureCursorVisible(); 


private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;
    int setBlockState(QTextBlock&); 
    void contextMenuEvent(QContextMenuEvent *);
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;


};


#endif

