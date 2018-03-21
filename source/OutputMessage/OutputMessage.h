/*================================C++========================================*\
      Create: 2018/02/10
      Creator: Huang Huakun
      File Name: OutputMessage.h
      Platform: windows
      Version: v1.0

      Describe: OutputMessage uses the QTextBrowser

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/


#ifndef OUTPUTMESSAGE_H
#define OUTPUTMESSAGE_H

#include <QTextBrowser>
#include <QMainWindow>
#include <stdio.h>


class OutputMessage : public QMainWindow
{
	Q_OBJECT
	
public:
    explicit OutputMessage( QWidget *parent = 0) ;
	~OutputMessage( ) ;
	
	QTextBrowser *TextBrowser ;
	QString test ;
	void OutputMessageInWindow(QString Message, QString Position);
        void fileCloseMaybeSaveMessage(int index);
} ;

#endif //OUTPUTMESSAGE_H
	
	
	
	
	
	
	
	
	
	
	
