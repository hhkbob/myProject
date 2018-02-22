/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: initMainWindowAction.cpp
      Platform: windows
      Version: v1.0

      Describe: Init the MainWindow

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include "CFDbeta.h"
#include "QLabel"

void MainWindow::initMainWindowAction( )
{
    //  set the statusbar using the label widget
    labelStatusBar = new QLabel;
    labelStatusBar = new QLabel;
    labelStatusBar->setMinimumSize(200, 25);
    labelStatusBar->setFrameShape(QFrame::WinPanel);
    labelStatusBar->setFrameShadow(QFrame::Sunken);
    labelStatusBar ->setText( QString( "FlowCD v1.0" ) );
    initConsoleWindow( ) ;

    //  config the window, request the openfoam dir
    bool successful = ConfigWindow.ConfigurationFileExist( ) ;
    if( !successful )
	ConfigWindow.exec( ) ;
	RunModel = new RunCommand(CurrentPath, ConfigWindow.CFDbetaFOAMDir);
}








