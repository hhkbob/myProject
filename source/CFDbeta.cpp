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
#include "qtabwidget.h"


#ifdef WIN32
#include <windows.h>
#endif

#include "pqMainControlsToolbar.h"
#include "pqApplicationCore.h"


MainWindow::MainWindow( QWidget *parent ) :QMainWindow(parent),
   ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setAcceptDrops(true);
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
    connect(tabEditor, SIGNAL(tabCloseRequested(int)), this, SLOT(fileClose(int)));
	

    //  add the Mesh GUI
    QAction *MeshGUI = new QAction("Mesh Gui", ui->menuMesh_M);
    ui->menuMesh_M->addAction(MeshGUI);
    connect(MeshGUI, SIGNAL(triggered()), this, SLOT(MeshGUITrigger()));
	   
    //  add the frame for the group and program widget
    ui->FolderWindow->setStyleSheet( "border: 1px solid black;" );
    ui->treeView->setStyleSheet("border-bottom:0px ; border-top: 0px;");
    ui->lineCommand->setStyleSheet("border-top: 1px solid black;");
    ui->groupBox->setStyleSheet("border-bottom: 0px; border-right: 0px;\
                                  border-left: 0px;");
    textBrowser->setStyleSheet("border-top: 0px; border-bottom: 0px;" );

    //  add the mainToolBar
    buildMainToolBar(  ) ;

    //  add the toolbar to the CurrentFolderGroup
    Program = new QToolBar ;
    //  choose the Folder
    main = new QAction("main", Program ) ;
    main->setIcon( QIcon(":/Resource/256_256/main.png" ) ) ;
    main->setToolTip("The main folder" ) ;
    Program->addAction( main ) ;
    Program->setStyleSheet("border: 0px;" ) ;
    ui->ProgramToolLayout->addWidget( Program ) ;
    connect(main, SIGNAL(triggered()), this, SLOT(ReturnMainPath()));
    //  aad Find file
    FindFile = new QAction( "Find", Program ) ;
    FindFile->setIcon( QIcon(":/Resource/256_256/ssearch.png" )) ;
    Program->addAction( FindFile ) ;

    //  build the editor toolbar
    editorToolBar = new QToolBar("Editor Tool Bar");
    addToolBar(editorToolBar);

    FileToolBar = new QToolBar("File Tool Bar");
    newNumber = 0;
    //  add the new file
    newfile = new QAction("newfile", FileToolBar);
    newfile->setIcon(QIcon(":/Resource/512_512/newfile.png"));
    FileToolBar->addAction(newfile);
    //  add the save 
    savetrigger = new QAction("Save", FileToolBar);
    savetrigger->setIcon(QIcon(":/Resource/512_512/save.png"));
    FileToolBar->addAction(savetrigger);
    connect(savetrigger, SIGNAL(triggered()), this, SLOT(Save()));
    connect(newfile, SIGNAL(triggered()), this, SLOT(newFile()));
    addToolBar(FileToolBar);
    codeEditor = new FoamEditor(this);
      
    //  The File Menu
    connect(ui->Image_FIle, SIGNAL(triggered()), this, SLOT(OpenFileImageTrigger()));
    connect(ui->Editor_File, SIGNAL(triggered()), this, SLOT(OpenFileEditorTrigger()));	
    vtkView.buildRecentFilesMenu( *ui->menuRecents);

    //  The Mesh Function
    buildMeshFunction();
	 
    //  the toolbar ui
    editorToolBar->setStyleSheet("background-color:rgb(240, 240, 240); ");
	 
    //  set up the paraview client
    InitTheMainWindow( );

    textBrowserMark = 0;
    connect(ui->FolderView, SIGNAL(clicked()), this, SLOT(ShowTreeView()));
    connect(ui->Output, SIGNAL(clicked()), this, SLOT(ShowTextBrowser()));
    connect(ui->actionYPlus_Estimate, SIGNAL( triggered() ),
	        this, SLOT(yPlusEstimate()));
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
    deleteMainToolBar() ;
    delete RunModel;

        
	
}

int MainWindow::initTabAndQVTKWidget( )
{
	return 1 ;
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
	       CurrentPath = initWin.CPathToQtPath(GetPath);
	       TreeViewDir.initTheModel(ui->treeView, CurrentPath);
	       Watcher.addPath(CurrentPath);
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
	   RunModel->CurrentPath = CurrentPath;
	   RunModel->Command(LINUXRUNNING, command, COMMAND_COMMAND );
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
		OpenFile(FileName);
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
	ui->colorMapEditorDock->hide();

	// Enable help from the properties panel.
	QObject::connect(ui->proxyTabWidget,
		SIGNAL(helpRequested(const QString&, const QString&)), this,
		SLOT(showHelpForProxy(const QString&, const QString&)));

	// setup color editor
	/// Provide access to the color-editor panel for the application.
	pqApplicationCore::instance()->registerManager(
		"COLOR_EDITOR_PANEL", ui->colorMapEditorDock);

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

void MainWindow::yPlusEstimate( )
{
	system("calculator.exe");
}

void MainWindow::MeshGUITrigger()
{
	
	Meshgui.show();
}

//-----------------------------the code editor------------------------------

void MainWindow::newFile( )
{
	QString fileName = tr("united%1.txt").arg(++newNumber);
	openedFiles << fileName;
	int current = tabEditor->addTab(codeEditor->newEditor(), fileName);
	tabEditor->setCurrentIndex(current);
}
bool MainWindow::Save()
{
	int index = tabEditor->currentIndex();
	curFile = openedFiles.at(index);
	cout << curFile.toLocal8Bit().data() << endl;
	if (!curFile.contains("/") || !curFile.contains("\\")) {
		return saveAs();
	}
	else {
	    return saveFile(curFile);
	}
}
bool MainWindow::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << codeEditor->textEdit->text();
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("save as"),CurrentPath,tr("Plain text Files(*.cpp, *.c);; All Files(*)"));
	if (fileName.isEmpty())
		return false;
	int index = tabEditor->currentIndex();
	openedFiles.replace(index, fileName);
	return saveFile(fileName);
}
void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	codeEditor->textEdit->setModified(false);
	setWindowModified(false);

	QString shownName;
	if (curFile.isEmpty())
		shownName = "untitled.txt";
	else
	{
		shownName = strippedName(fileName);
		int index = tabEditor->currentIndex();
		tabEditor->setTabText(index, shownName);
		tabEditor->setCurrentIndex(index);
	}

}
QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void MainWindow::fileClose(int index)
{
	{
		bool mark = maybeSave(index);
#if _DEBUG
		printf("\nThis a information for developer\n\n");
		printf("In myui.cpp, the fileClose maybeSave function\n");
		printf("The close index = %d\n", index);
		printf("the files count = 1\n");
		if (mark)
			printf("The bool value is 1\n\n");
		else
			printf("The bool value is 0\n\n");
		printf("Debug Exit\n");
#endif
		if (mark)
		{
			if (openedFiles.count() == 1)
			{
				openedFiles.removeAt(0);
				tabEditor->removeTab(0);
			}
			else
			{
#if _DEBUG
				printf("\nThis ia a information for developer\n\n");
				printf("In muui.cpp, the fileClose function\n");
				printf("the close index =%d\n", index);
				printf("the files count =%d\n\n", openedFiles.count());
				printf("Debug Exit\n");
#endif
				openedFiles.removeAt(index);
				tabEditor->removeTab(index);
			}
		}
	}
}
bool MainWindow::maybeSave( int index)
{
	QsciScintilla *text = EDITOR;
	QString fileName = openedFiles.at(index);
	if (text->isModified()) {
		int ret = QMessageBox::warning(this, tr("Application"),
			tr("The document has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::Default,
			QMessageBox::No,
			QMessageBox::Cancel | QMessageBox::Escape);
		if (ret == QMessageBox::Yes)
			return Save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void MainWindow::OpenFile(QString fileName)
{
	openedFiles << fileName;
	codeEditor->newTab(tabEditor, fileName);
	int current = tabEditor->currentIndex();
        tabEditor->setCurrentIndex(current);
	tabEditor->setCurrentWidget(codeEditor->newEditor());
}

void MainWindow::dragEnterEvent(QDragEnterEvent* evt)
{
	evt->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent* evt)
{
	QList<QUrl> urls = evt->mimeData()->urls();
	if (urls.isEmpty())
	{
		return;
	}

	QList<QString> files;

	foreach(QUrl url, urls)
	{
		if (!url.toLocalFile().isEmpty())
		{
			files.append(url.toLocalFile());
		}
	}

	// If we have no file we return
	if (files.empty() || files.first().isEmpty())
	{
		return;
	}
	
	int num = files.size();
	QString fileName;
	for (int i = 0; i < num; i++)
	{
		fileName = files.at(i);
		char f[20];
		strcpy(f,fileName.section('.', -1).toLocal8Bit().data());
		if (strlen(f) == 0 || !strcmp(f, "cpp")
			|| !strcmp(f, "cxx") || !strcmp(f, "h")
			|| !strcmp(f, "H") || !strcmp(f, "txt") || !strcmp(f, "c"))
			OpenFile(fileName);
		else
			pqLoadDataReaction::loadData(files);
	}
}

void MainWindow::ReturnMainPath()
{
    //  return current path
    char path[200];
    strcpy( path, "cd " ) ;
    strcat( path, CurrentPath.toLocal8Bit().data() ) ;
    dirChange( path ) ;
}

