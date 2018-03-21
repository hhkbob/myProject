/*================================C++========================================*\
      Create: 2018/02/22
      Creator: Huang Huakun
      File Name: RunCommand.h
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
#ifndef WINDOWRUNNING 
#define WINDOWRUNNING 1
#endif
#ifndef LINUXRUNNING
#define LINUXRUNNING 2
#endif
#ifndef OPENFOAM_COMMAND
#define OPENFOAM_COMMAND 90
#endif
#ifndef COMMAND_COMMAND
#define COMMAND_COMMAND  50
#endif
#ifdef WIN32
#include <windows.h>
#endif

#ifndef RUNCOMMAND_H
#define RUNCOMMAND_H

#include <stdio.h>
#include <iostream>
#include <QMainWindow>
#include "initwindow.h"


class RunCommand
{
public:
    RunCommand(QString Path, QString Dir );
    void Command( int mark, QString run, int CommandType );
public:
    QString CurrentPath ;
	QString source ;
	initWindow initWin;
};
#endif
	