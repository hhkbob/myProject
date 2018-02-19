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


#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>

class QVTKView::pqInternals : public Ui::pqClientMainWindow
{
};

QVTKView::QVTKView()
{
	
}
int QVTKView :: ShowModel(QVTKWidget *QVTK )
{
	// This creates a polygonal cylinder model with eight circumferential facets
  // (i.e, in practice an octagonal prism).
  vtkSmartPointer<vtkCylinderSource> cylinder =
    vtkSmartPointer<vtkCylinderSource>::New();
  cylinder->SetResolution(8);

  // The mapper is responsible for pushing the geometry into the graphics library.
  // It may also do color mapping, if scalars or other attributes are defined.
  vtkSmartPointer<vtkPolyDataMapper> cylinderMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

  // The actor is a grouping mechanism: besides the geometry (mapper), it
  // also has a property, transformation matrix, and/or texture map.
  // Here we set its color and rotate it around the X and Y axes.
  vtkSmartPointer<vtkActor> cylinderActor =
    vtkSmartPointer<vtkActor>::New();
  cylinderActor->SetMapper(cylinderMapper);
  cylinderActor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
  cylinderActor->RotateX(30.0);
  cylinderActor->RotateY(-45.0);

  // The renderer generates the image
  // which is then displayed on the render window.
  // It can be thought of as a scene to which the actor is added
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(cylinderActor);
  
  renderer->SetBackground(0.1, 0.2, 0.4);
  // Zoom in a little by accessing the camera and invoking its "Zoom" method.
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Zoom(1.5);


  // The render window is the actual GUI window
  // that appears on the computer screen
  QVTK->GetRenderWindow()->AddRenderer(renderer);
  vtkRenderWindowInteractor *iren;
  iren = vtkRenderWindowInteractor::New();
  iren = QVTK->GetInteractor();

  iren->SetRenderWindow(QVTK->GetRenderWindow());

  // The render window interactor captures mouse events
  // and will perform appropriate camera or actor manipulation
  // depending on the nature of the events.

  //renderWindowInteractor->SetRenderWindow(renderWindow);

  // This starts the event loop and as a side effect causes an initial render.
  //renderWindowInteractor->Start();
  

  return EXIT_SUCCESS;
}

void QVTKView::InitTheMainWindow( )
{
  this->Internals = new pqInternals();
  this->Internals->setupUi(this);

  // Setup default GUI layout.

  // Set up the dock window corners to give the vertical docks more room.
  //this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  //this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

  //this->Internals->animationViewDock->hide();
  //this->Internals->statisticsDock->hide();
  //this->Internals->comparativePanelDock->hide();
  //this->tabifyDockWidget(this->Internals->animationViewDock, this->Internals->statisticsDock);

  // Enable help from the properties panel.
  QObject::connect(this->Internals->proxyTabWidget,
    SIGNAL(helpRequested(const QString&, const QString&)), this,
    SLOT(showHelpForProxy(const QString&, const QString&)));

  // Populate application menus with actions.
  pqParaViewMenuBuilders::buildFileMenu(*this->Internals->menu_File);
  pqParaViewMenuBuilders::buildEditMenu(*this->Internals->menu_Edit);

  // Populate sources menu.
  pqParaViewMenuBuilders::buildSourcesMenu(*this->Internals->menuSources, this);

  // Populate filters menu.
  pqParaViewMenuBuilders::buildFiltersMenu(*this->Internals->menuFilters, this);

  // Populate Tools menu.
  pqParaViewMenuBuilders::buildToolsMenu(*this->Internals->menuTools);

  // setup the context menu for the pipeline browser.
  pqParaViewMenuBuilders::buildPipelineBrowserContextMenu(*this->Internals->pipelineBrowser);

  pqParaViewMenuBuilders::buildToolbars(*this);

  // Setup the View menu. This must be setup after all toolbars and dockwidgets
  // have been created.
  pqParaViewMenuBuilders::buildViewMenu(*this->Internals->menu_View, *this);

  // Setup the menu to show macros.
  pqParaViewMenuBuilders::buildMacrosMenu(*this->Internals->menu_Macros);

  // Setup the help menu.
  pqParaViewMenuBuilders::buildHelpMenu(*this->Internals->menu_Help);

  // Final step, define application behaviors. Since we want all ParaView
  // behaviors, we use this convenience method.
  new pqParaViewBehaviors(this, this);
}
	
void QVTKView::showHelpForProxy(const QString& groupname, const QString& proxyname)
{
#ifdef PARAVIEW_USE_QTHELP
  pqHelpReaction::showProxyHelp(groupname, proxyname);
#endif
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	