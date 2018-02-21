/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: initConsole.cpp
      Platform: windows
      Version: v1.0

      Describe: init the consol window.
	            including name, input method, position, width and length.

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include "CFDbeta.h"
#ifdef WIN32
#include <Windows.h>
#endif

int MainWindow:: initConsoleWindow( )
{

//  windows platform
#ifdef WIN32 
	//  set the console properties
	SetConsoleTitle("CFDbeta");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);	
	//  set the console position , width and length
	SMALL_RECT rc;
	rc.Left = 0;
	rc.Top = 0;
	rc.Right = 80;
	rc.Bottom = 24;

	//  the console length is 90*24
	SetConsoleWindowInfo(hOut, TRUE, &rc);
        //  forbiddon the input fuction
	system( "Chcp 437");
#endif 
	//  the soft information shows
	printf( "CFDbeta is a free soft. And this version is tesing\n\n");
	printf( "The current version is v1.0\n\n" ) ;
	
	return 1 ;
}
