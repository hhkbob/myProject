/*================================C++========================================*\
      Create: 2018/02/22
      Creator: Huang Huakun
      File Name: MainWindow.cpp
      Platform: windows/linux
      Version: v1.0

      Describe: The MainWindow.

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include "CFDbeta.h"

void MainWindow::buildMainToolBar(  ) 
{
	//  add the blockMesh to the MainWindow
	 ToolBar = new QToolBar("ToolBar");
	 blockMesh = new QAction("blockMesh", ToolBar);
	 blockMesh->setIcon(QIcon(":/Resource/512_512/grid.png"));
	 ToolBar->addAction(blockMesh);
	 //  connect the slot
	 connect(blockMesh, SIGNAL(triggered()), this, SLOT(BlockMeshTriggered()));
	 
	 wmake = new QAction("wmake", ToolBar); 
	 wmake->setIcon(QIcon(":/Resource/256_256/wmake.png"));
	 ToolBar->addAction(wmake);
	 connect(wmake, SIGNAL(triggered()), this, SLOT(WmakeTriggered()));
	 
	 run = new QAction("run", ToolBar );
	 run->setIcon(QIcon(":/Resource/256_256/run.png"));
	 ToolBar->addAction(run);
	 connect(run, SIGNAL(triggered()), this, SLOT(RunTriggered()));
	 
	 pyRun = new QAction("pyRun", ToolBar);
	 pyRun->setIcon(QIcon(":/Resource/256_256/pyRun.png"));
	 ToolBar->addAction(pyRun);
	 connect(pyRun, SIGNAL(triggered()), this, SLOT(pyRunTriggered()));
	 
	 clear = new QAction("clear",ToolBar);
	 clear->setIcon(QIcon(":/Resource/512_512/clear.png"));
	 ToolBar->addAction(clear);
	 connect(clear, SIGNAL(triggered()), this, SLOT(ClearTriggered()));
	 //  add the ToolBar
	 addToolBar(ToolBar);
	 ToolBar->setStyleSheet("background-color:rgb(240, 240, 240);");
}
void MainWindow::BlockMeshTriggered( )
{
	RunModel->CurrentPath = CurrentPath;
	RunModel->Command(LINUXRUNNING, "blockMesh", OPENFOAM_COMMAND);
}

void MainWindow::WmakeTriggered( )
{
	RunModel->CurrentPath = CurrentPath;
	RunModel->Command(LINUXRUNNING, "wmake", OPENFOAM_COMMAND);
}

int MainWindow::RunTriggered( )
{
      // search the application name in constant
      FILE *controlDict ;
      QString FileName = CurrentPath + "/system/controlDict";
      controlDict = fopen( FileName.toLocal8Bit().data(), "r" ) ;
    
      char application[100] ;
      if(controlDict==NULL)
      {
          cout<< "cannot find the controlDict in the system file" <<endl;
          return 0;
      }
      lookup( "application", application, controlDict ) ;
      fclose( controlDict ) ;
      RunModel->CurrentPath = CurrentPath ;
      RunModel->Command(LINUXRUNNING, application, OPENFOAM_COMMAND);
}

int MainWindow::pyRunTriggered( )
{
     FILE *controlDict ;
     QString FileName = CurrentPath + "/system/controlDict";
     controlDict = fopen( FileName.toLocal8Bit().data(), "r" ) ;
    
    char application[100] ;
    char Command[500];
    char CaseName[100] ;
    if(controlDict==NULL)
    {
        cout<< "cannot find the controlDict in system file" <<endl ;
        exit(0);
    }

    lookup( "application", application, controlDict ) ;
    fclose( controlDict ) ;
    
    //  UpperPath
    int MarkPath = 0;
    int len = 0;
    char UpperPath[200];
    for (int i = 0; i < strlen(CurrentPath.toLocal8Bit().data()); i++)
    {				
        if (CurrentPath.toLocal8Bit().data()[i] == '/')
	    MarkPath = len;
	len++;
    }
    for (int i = 0; i <= MarkPath; i++)
    {
	UpperPath[i] = CurrentPath.toLocal8Bit().data()[i];
    }
    UpperPath[MarkPath ] = '\0';
	//  test the UpperPath
	QFileInfo tmpFileInfo(UpperPath);
	if (!tmpFileInfo.isDir())
	{
		cout
			<< "\nFATAL ERROR\n\n"
			<< "    The pyFoam command cannot run in this folder\n"
			<< "    The Upper Path is not existed\n\n"
			<< "Exit\n";
		return 0;
	}
	char command[500];
    QString Case = CurrentPath.section('/',-1) ;
    sprintf( CaseName, Case.toLocal8Bit().data() ) ;
    sprintf( command, "pyFoamPlotRunner.py --clear %s -case %s", application, CaseName ) ;
    RunModel->CurrentPath = UpperPath ;
    RunModel->Command(LINUXRUNNING, command, OPENFOAM_COMMAND);
}

void MainWindow::ClearTriggered( )
{
    //  write the clean file
    FILE *clean ;
    QString FileName = CurrentPath + "/Allclean" ;
    clean = fopen( FileName.toLocal8Bit().data(), "r" );
    if( clean==NULL )
    {
		ofstream stream(FileName.toLocal8Bit().data());
        stream << "#!/bin/sh\n"
               << "cd ${0%/*} || exit 1 #run from this directory\n"
               << ". $WM_PROJECT_DIR/bin/tools/CleanFunctions\n"
               << "cleanCase\n";
		stream.close();
     }
     if( clean!=NULL )
        fclose( clean ) ;
     //  run this file
     RunModel->CurrentPath = CurrentPath ;
     RunModel->Command(LINUXRUNNING, "./Allclean", OPENFOAM_COMMAND);
}

void MainWindow::lookup( char name[], char buffer[], FILE *file )
{
    buffer[0] = fgetc( file) ;
    while( !feof( file ) )
    {
        ungetc( buffer[0], file ) ;
        fscanf( file, "%s", buffer ) ;
        if( strcmp( buffer, name )== NULL )
        {
            fscanf( file, "%s", buffer ) ;
            break ;
        }
        buffer[0] = fgetc( file ) ;
    }
    rewind( file ) ;

    // delete the ";", get the right application name
    for(unsigned int i=0; i<strlen( buffer ); i++ )
    {
        if( buffer[i] == ';' || buffer[i] == ' ' )
        {
            buffer[i] = '\0' ;
            break;
        }
     }
}
    
void MainWindow::deleteMainToolBar()
{
	delete blockMesh;
	delete wmake;
	delete run;
	delete pyRun;
	delete clear;
	delete ToolBar;
}
	

