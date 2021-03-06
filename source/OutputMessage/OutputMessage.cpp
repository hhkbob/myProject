/*================================C++========================================*\
      Create: 2018/02/10
      Creator: Huang Huakun
      File Name: OutputMessage.cpp
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

#include "OutputMessage.h"

OutputMessage::OutputMessage( QWidget *parent ) :
      QMainWindow( parent )
{
	TextBrowser = NULL ;
	test = "This is a test version" ;
}

OutputMessage::~OutputMessage( )
{
    delete TextBrowser ;
}	

void OutputMessage::OutputMessageInWindow( QString Message, QString Position )
{
	//  use the console output the fatal error
	printf("\nFATAL ERROR\n\n");
	printf("       The ERROR occurs in %s\n", Position.toLocal8Bit().data());
	printf("       %s\n", Message.toLocal8Bit().data());
	printf("\nFlowCD Exit\n\n");
}
void OutputMessage::fileCloseMaybeSaveMessage(int index)
{
        printf("\nThis a information for developer\n\n" );
        printf("In myui.cpp, the fileClose function\n" ) ;
        printf("The close index = %d\n", index ) ;
        printf("the files count = 1\n" );
        printf("Try to remove the Tab 1\n\n" ) ;
        printf("Debug Exit\n" ) ;
}
























