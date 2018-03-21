/*================================C++========================================*\
Create: 2018/02/09
Creator: Huang Huakun
File Name: notepad.cpp
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

#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include "config.h"

QT_BEGIN_NAMESPACE

QT_FORWARD_DECLARE_CLASS(QEvent)
QT_FORWARD_DECLARE_CLASS(QPaintEvent)
QT_FORWARD_DECLARE_CLASS(QKeyEvent)
QT_FORWARD_DECLARE_CLASS(QMouseEvent)
QT_FORWARD_DECLARE_CLASS(QResizeEvent)
QT_FORWARD_DECLARE_CLASS(QContextMenuEvent)
QT_FORWARD_DECLARE_CLASS(QAction)

QT_END_NAMESPACE

#define FOLDBOXRECT(cy) QRect(lineNumWidth + foldBoxIndent, cy - foldBoxWidth / 2, foldBoxWidth, foldBoxWidth)
#define FULLRESIZE      resizeEvent(new QResizeEvent(QSize(0, 0), size()))
#define FONTWIDTH       fontMetrics().width(QLatin1Char('X'))

class NotePad : public QPlainTextEdit
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
	NotePad(Config *, QWidget *parent = 0);
	~NotePad();

	public slots:
	int search(QString, bool, bool, bool); 
	void replace(QString, QString, bool, bool, bool);   
	void replaceAll(QString, QString, bool, bool);  
	void gotoLine(int lineNumber);  
private slots:
	void deleteSelected()
	{
		textCursor().removeSelectedText();
	}
	void toUpperCase()
	{
		convertCase();
	}   
	void toLowerCase()
	{
		convertCase(false);
	}   

	void blockCountChanged(int count)
	{
		lineNumDigits = qMax(2, QString::number(count).length());
	} 
	void contentsChange(int, int, int); 
	void ensureCursorVisible();

	void reconfig(int = (Config::Init | Config::Editor));   

private:
	void resizeEvent(QResizeEvent *);
	void paintEvent(QPaintEvent *);

	void keyPressEvent(QKeyEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void contextMenuEvent(QContextMenuEvent *);

	bool eventFilter(QObject *, QEvent *);
	void extraAreaPaintEvent();
	void extraAreaMouseEvent(QMouseEvent *);

	int setBlockState(QTextBlock&); 
	QTextBlock findBlockByY(int); 

	void convertCase(bool toUpper = true)
	{
		textCursor().insertText(
			toUpper ?
			textCursor().selectedText().toUpper() :
			textCursor().selectedText().toLower());
	}

	QTextBlock pointedBlock;

	QWidget *extraArea;   
	int lineNumDigits;   
	int lineNumWidth;    
	int foldBoxWidth;
	int foldBoxIndent;
	int foldBoxLength;

	Config *config;
};

#endif // NOTEPAD_H
