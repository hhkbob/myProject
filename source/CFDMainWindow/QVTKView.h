/*================================C++========================================*\
      Create: 2018/02/11
      Creator: Huang Huakun
      File Name: QVTKView.h
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

#ifndef QVTKVIEW_H
#define QVTKVIEW_H

#include "QVTKWidget.h"
#include "QMainWindow"

class QVTKView :public QMainWindow
{
	Q_OBJECT
public:
	QVTKView();
//  public function	
public:
    //  show the model
	int ShowModel(QVTKWidget *QVTK ) ;
	void InitTheMainWindow( ) ;
private:
    class pqInternals;
    pqInternals* Internals;
private slots:
    void showHelpForProxy(const QString& groupname, const QString& proxyname);
	
};

#endif // QVTKVIEW_H	
	
