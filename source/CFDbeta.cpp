/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: CFDbeta.cpp
      Platform: windows/linux
      Version: v1.0

      Describe: The software main source file.

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include "CFDbeta.h"
#include "ui_CFDbeta.h"
#include <QTextEdit>
#include <QDockWidget>
#include <QLabel>
#include <QFontDialog>
#include "CFDMainWindow/dir.h"
#include <qgridlayout.h>
#include "QSplitter"

#ifdef WIN32
#include <windows.h>
#endif

#include "pqMainControlsToolbar.h"

MainWindow::MainWindow( QWidget *parent ) :QMainWindow(parent),
   ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
    //  define the platform
    RunningPlatForm = WINDOWRUNNING;
    initMainWindowAction( ) ;  

    //  init the QVTK widget and tabWidget
    initTabAndQVTKWidget( ) ;
    setUpDir();
    ui->statusBar->addWidget(labelStatusBar); 

   
    connect( ui->lineCommand,SIGNAL( returnPressed() ), 
             this, SLOT( LineEditRun( ) ) );
	
	textBrowser = new QTextBrowser;
	EditorTab = new TabWidget;
	tabEditor = ui->EditorManager->EditorWidget;
	tabEditor->setTabsClosable(true);
	//ui->EditorLayout->setMargin(0);
	//ui->EditorLayout->setSpacing(0);
	//ui->EditorLayout->addWidget(tabEditor);
	//DockManager->hide();
	//tabEditor->hide();


	conf = new Config;
	editorUI = new myUI(conf, tabEditor, ui->treeView, textBrowser,
		&initWin);
     //  add the frame for the group and program widget
     ui->FolderWindow->setStyleSheet( "border: 1px solid black;" );
     ui->treeView->setStyleSheet("border-bottom:0px ; border-top: 0px;");
     ui->lineCommand->setStyleSheet("border-top: 1px solid black;");
     ui->groupBox->setStyleSheet("border-bottom: 0px; border-right: 0px;\
                                  border-left: 0px;");
     textBrowser->setStyleSheet("border-top: 0px; border-bottom: 0px;" );

	 //  add the blockMesh to the mainToolBar
	 mainToolBar = new QToolBar;
	 blockMesh = new QAction("blockMesh", mainToolBar);
	 blockMesh->setIcon(QIcon(":/Resource/512_512/grid.png"));
	 mainToolBar->addAction(blockMesh);
	 //  add the ToolBar
	 addToolBar(mainToolBar);

     //  add the toolbar to the CurrentFolderGroup
     Program = new QToolBar ;
     //  choose the Folder
     main = new QAction("main", Program ) ;
     main->setIcon( QIcon(":/Resource/256_256/main.png" ) ) ;
     main->setToolTip("The main folder" ) ;
     Program->addAction( main ) ;
     Program->setStyleSheet("border: 0px;" ) ;
     ui->ProgramToolLayout->addWidget( Program ) ;
     //  aad Find file
     FindFile = new QAction( "Find", Program ) ;
     FindFile->setIcon( QIcon(":/Resource/256_256/ssearch.png" )) ;
     Program->addAction( FindFile ) ;

     //  add the editor tool bar
     editorToolBar = new QToolBar ;
     //  add the new file
     newfile = new QAction( "newfile", editorToolBar ) ;
     newfile->setIcon( QIcon(":/Resource/512_512/newfile.png" )) ;
     editorToolBar->addAction( newfile ) ;
     //  add the save 
     save = new QAction( "Save", editorToolBar) ;
     save->setIcon( QIcon(":/Resource/512_512/save.png" )) ;
     editorToolBar->addAction( save ) ;
     //  add the editor toolbar to the mainwindow
     addToolBar( editorToolBar ) ;
      

	 //  The File Menu
	 connect(ui->Image_FIle, SIGNAL(triggered()), this, SLOT(OpenFileImageTrigger()));
	 connect(ui->Editor_File, SIGNAL(triggered()), this, SLOT(OpenFileEditorTrigger()));
	
	 vtkView.buildRecentFilesMenu( *ui->menuRecents);
	 
	 //  the toolbar ui
	 mainToolBar->setStyleSheet("background-color:rgb(240, 240, 240);");
	 editorToolBar->setStyleSheet("background-color:rgb(240, 240, 240); ");
	 
	 //  set up the paraview client
	 InitTheMainWindow( );

	 textBrowserMark = 0;
	 connect(ui->FolderView, SIGNAL(clicked()), this, SLOT(ShowTreeView()));
	 connect(ui->Output, SIGNAL(clicked()), this, SLOT(ShowTextBrowser()));

	 //  connect the slot
	 connect(blockMesh, SIGNAL(triggered()), this, SLOT(BlockMeshTrriger()));
}

MainWindow::~MainWindow()
{
        //  note: delete the point step by step, follow the order
	delete ui;
	delete labelStatusBar;
	delete TreeViewMenu;
	delete OpenFileOuterAction; delete item;
	delete model; 
	delete textBrowser;
	delete tabEditor;
    //  delete the Program bar
    delete main;
    delete FindFile;
    delete Program ;
    //  delete the editor toolbar
    delete newfile;
    delete save ;
    delete editorToolBar;
    //  delete the mainToolBar
    delete blockMesh ;
	delete mainToolBar;
        
	
}

int MainWindow::initTabAndQVTKWidget( )
{
    //tabEditor->setMovable(true);
	//tabEditor->setContextMenuPolicy(Qt::CustomContextMenu);
	//tabEditor->setTabsClosable(true);
	return 1 ;
}

void MainWindow::RunnCommand(int mark, QString run, int CommandType )
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
		    runFileInLinux = "bash -c \"" + ConfigWindow.CFDbetaFOAMDir + " && ";
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
    printf("This is running Debug:\n%s\n\n",runFileInLinux.toLocal8Bit().data());
	printf("\n");
#endif
		system( runFileInLinux.toLocal8Bit().data() );
	}
}

void MainWindow::LineEditRun( ) 
{
   //  this lineEdit is only run the linux command
   QString command = ui->lineCommand->text( ) ;
   char path[]={"cd "}; 
   int k = 0 ;
   for( int i=0; i<strlen( path ); i++ )
   {
       if( command.toLocal8Bit().data()[i] != path[i] )
           break ;
       k = k + 1 ;
   }
   if (k == strlen(path))
   {
	   //  refresh the dir
	   //  get the path directory
	   char GetPath[200];
	   int mark = 0;
	   for (int i = 3; i < strlen(command.toLocal8Bit().data()); i++)
	   {
		   if (command.toLocal8Bit().data()[i] == ' ' && mark == 0)
			   continue;
		   else
		   {
			   GetPath[mark] = command.toLocal8Bit().data()[i];
			   mark++;
		   }
	   }
	   GetPath[mark] = '\0';
	   QFileInfo tmpFileInfo(GetPath);
	   if (tmpFileInfo.isDir())
	   {
		   CFDbetaTreeView TreeViewDir(model, item);
		   CurrentPath = QString(GetPath);
		   TreeViewDir.initTheModel(ui->treeView, CurrentPath);
	   }
	   else
	   {
#ifdef _DEBUG
		   printf("\nThe testing information is for developers:\n\n");
		   printf("This is running Debug:\nTesting inpuing is directory\n");
		   printf("The directory is: %s\n", GetPath);
		   printf("It is a invalided directory\n\n");
		   printf("\n");
#endif
		   QString Message = "Try again! use the absolute path.";
		   QString Position = "Input the direcctory command";
		   OutputConsole.OutputMessageInWindow(Message, Position);
	   }
   }
   else
   {
	   RunnCommand(LINUXRUNNING, command, COMMAND_COMMAND );
   }
   QMessageBox box( QMessageBox::Warning, "Warning", "Have Done!") ;
   box.exec() ;
}

void MainWindow::ShowTextBrowser()
{
	ui->treeView->hide();
	if (textBrowserMark == 0)
	{
		ui->CurrentGridLayOut->addWidget(textBrowser);
		textBrowser->setText("this is a test");
		textBrowserMark = 1;
	}
	else
		textBrowser->show();
}

void MainWindow::ShowTreeView()
{
	textBrowser->hide();
	ui->treeView->show();
}

void MainWindow::BlockMeshTrriger()
{
	RunnCommand(LINUXRUNNING, "blockMesh", OPENFOAM_COMMAND);
}

void MainWindow::initTheQVTKWindow()
{

}
void MainWindow::OpenFileImageTrigger()
{
	buildFileOpenTigger.loadData();
	if (ui->MultiViewManager->isHidden())
		ui->MultiViewManager->show();
}
void MainWindow::OpenFileEditorTrigger()
{
	QString FileName = buildFileOpenTigger.buildFileOpenTrigger();
	QFileInfo FileInfo(FileName);
	if (FileInfo.isFile())
	{
		editorUI->openFile(FileName);
		if( tabEditor->isHidden())
		    tabEditor->show();
	}
}

void MainWindow::showHelpForProxy(const QString& groupname,
	const QString& proxyname)
{
#ifdef PARAVIEW_USE_QTHELP
	pqHelpReaction::showProxyHelp(groupname, proxyname);
#endif
}

void MainWindow::InitTheMainWindow()
{

	// Set up the dock window corners to give the vertical docks more room.
	this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

	ui->animationViewDock->hide();
	ui->statisticsDock->hide();
	ui->comparativePanelDock->hide();
	this->tabifyDockWidget(ui->animationViewDock, ui->statisticsDock);
	ui->pipelineBrowserDock->hide();
	ui->proxyTabDock->hide();
	ui->Boundary->hide();

	// Enable help from the properties panel.
	QObject::connect(ui->proxyTabWidget,
		SIGNAL(helpRequested(const QString&, const QString&)), this,
		SLOT(showHelpForProxy(const QString&, const QString&)));

	// Populate application menus with actions.
	// pqParaViewMenuBuilders::buildFileMenu( *ui->menuFIle_F);
	pqParaViewMenuBuilders::buildEditMenu(*ui->menuParaView_Edit);

	// Populate sources menu.
	 pqParaViewMenuBuilders::buildSourcesMenu(*ui->menuSources, this);

	// Populate filters menu.
	pqParaViewMenuBuilders::buildFiltersMenu(*ui->menuFilters, this);

	// Populate Tools menu.
	pqParaViewMenuBuilders::buildToolsMenu(*ui->menuParaViewTools );

	// setup the context menu for the pipeline browser.
	pqParaViewMenuBuilders::buildPipelineBrowserContextMenu(*ui->pipelineBrowser);

	//  setup the tools bar
	vtkView.buildToolBars(*this);
	//pqParaViewMenuBuilders::buildToolbars(*this);

	// Setup the View menu. This must be setup after all toolbars and dockwidgets
	// have been created.
	pqParaViewMenuBuilders::buildViewMenu(*ui->menuView, *this);

	// Setup the menu to show macros.
	pqParaViewMenuBuilders::buildMacrosMenu(*ui->menuMacro);

	// Setup the help menu.
	pqParaViewMenuBuilders::buildHelpMenu(*ui->menuParaview);

	// Final step, define application behaviors. Since we want all ParaView
	// behaviors, we use this convenience method.
	new pqParaViewBehaviors(this, this);
}

void MainWindow::DockWidgetAndToolBarState()
{
	//  the ToolBar state
	if (tabEditor->isHidden())
	{
		//  I do not think about the right reaction
	}
}


