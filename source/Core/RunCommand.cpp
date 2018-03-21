/*================================C++========================================*\
      Create: 2018/02/22
      Creator: Huang Huakun
      File Name: RunCommand.cpp
      Platform: windows/linux
      Version: v1.0

      Describe: Run the command acooding the type

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include "RunCommand.h"


RunCommand::RunCommand(QString Path, QString Dir)
{
	CurrentPath = Path ;
	source = Dir ;
}
void RunCommand::Command( int mark, QString run, int CommandType )
{
    if ( mark == WINDOWRUNNING)
	{
         #ifdef WIN32
		 WinExec(run.toLocal8Bit().data(), SW_SHOW );
         #endif
	}
	else if (mark == LINUXRUNNING)
	{
		QString runFileInLinux;
		if( CommandType == OPENFOAM_COMMAND )
		    runFileInLinux = "bash -c \"" + source +" && ";
		else if( CommandType == COMMAND_COMMAND )
			runFileInLinux = "bash -c \"" ;
#ifdef WIN32
		QString pathChange = initWin.FilePathChange(CurrentPath);
		runFileInLinux += "cd  " + pathChange + " && " + run + "\"";
#else
		runFileInLinux += "cd  " + CurrentPath + " && " + run + "\"";
#endif
#ifdef _DEBUG
	    printf("\nThe testing information for is developers:\n\n");
        printf(
		"This is running Debug:\n%s\n\n",runFileInLinux.toLocal8Bit().data());
	    printf("\n");
#endif
		system( runFileInLinux.toLocal8Bit().data() );
	}
}