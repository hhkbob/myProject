/*================================C++========================================*\
      Create: 2018/02/11
      Creator: Huang Huakun
      File Name: QVTKView.cpp
      Platform: windows
      Version: v1.0

      Describe: Show the model in the soft
	            Using OpenGL and VTK.

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include "QVTKView.h"

#ifdef PARAVIEW_USE_QTHELP
#include "pqHelpReaction.h"
#endif
#include "pqParaViewBehaviors.h"
#include "pqParaViewMenuBuilders.h"

#include "pqRecentFilesMenu.h"


QVTKView::QVTKView()
{
	
}
int QVTKView :: ShowModel(QVTKWidget *QVTK )
{
  return 1;
}

void QVTKView::InitTheMainWindow(Ui::MainWindow *ui )
{

  // Set up the dock window corners to give the vertical docks more room.
  this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

  ui->animationViewDock->hide();
  ui->statisticsDock->hide();
  ui->comparativePanelDock->hide();
  this->tabifyDockWidget(ui->animationViewDock, ui->statisticsDock);
  this->tabifyDockWidget(ui->FolderWindow, ui->pipelineBrowserDock);
  ui->pipelineBrowserDock->hide();
  ui->proxyTabDock->hide();

  // Enable help from the properties panel.
  QObject::connect(ui->proxyTabWidget,
    SIGNAL(helpRequested(const QString&, const QString&)), this,
    SLOT(showHelpForProxy(const QString&, const QString&)));

  // Populate application menus with actions.
 // pqParaViewMenuBuilders::buildFileMenu(*ui->menu_File);
  //pqParaViewMenuBuilders::buildEditMenu(*ui->menu_Edit);

  // Populate sources menu.
 // pqParaViewMenuBuilders::buildSourcesMenu(*ui->menuSources, this);

  // Populate filters menu.
  //pqParaViewMenuBuilders::buildFiltersMenu(*ui->menuFilters, this);

  // Populate Tools menu.
  //pqParaViewMenuBuilders::buildToolsMenu(*ui->menuTools);

  // setup the context menu for the pipeline browser.
  pqParaViewMenuBuilders::buildPipelineBrowserContextMenu(*ui->pipelineBrowser);

  pqParaViewMenuBuilders::buildToolbars(*this);

  // Setup the View menu. This must be setup after all toolbars and dockwidgets
  // have been created.
  //pqParaViewMenuBuilders::buildViewMenu(*ui->menu_View, *this);

  // Setup the menu to show macros.
  //pqParaViewMenuBuilders::buildMacrosMenu(*ui->menu_Macros);

  // Setup the help menu.
  //pqParaViewMenuBuilders::buildHelpMenu(*ui->menu_Help);

  // Final step, define application behaviors. Since we want all ParaView
  // behaviors, we use this convenience method.
  new pqParaViewBehaviors(this, this);
}

void QVTKView::buildRecentFilesMenu(QMenu &RecentFiles)
{
	
	new pqRecentFilesMenu(RecentFiles, 0 );
}

void QVTKView::buildToolBars(QMainWindow &MainWindow)
{
	QToolBar* mainToolBar = new pqMainControlsToolbar(&MainWindow)
		<< pqSetName("MainControlsToolbar");
	mainToolBar->layout()->setSpacing(0);
	MainWindow.addToolBar(Qt::RightToolBarArea, mainToolBar);

	QToolBar* vcrToolbar = new pqVCRToolbar(&MainWindow) 
		<< pqSetName("VCRToolbar");
	vcrToolbar->layout()->setSpacing(0);
	MainWindow.addToolBar(Qt::TopToolBarArea, vcrToolbar);

	QToolBar* timeToolbar = new pqAnimationTimeToolbar(&MainWindow)
		<< pqSetName("currentTimeToolbar");
	timeToolbar->layout()->setSpacing(0);
	MainWindow.addToolBar(Qt::TopToolBarArea, timeToolbar);

	QToolBar* cameraToolbar = new pqCameraToolbar(&MainWindow) 
		<< pqSetName("cameraToolbar");
	cameraToolbar->layout()->setSpacing(0);
	MainWindow.addToolBar(Qt::RightToolBarArea, cameraToolbar);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	