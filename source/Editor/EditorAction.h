/*================================C++========================================*\
Create: 2018/02/14
Creator: Huang Huakun
File Name: EditorAction.h
Platform: windows
Version: v1.0

Describe: defined the function of the editor on MainWindow

Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
CopyRight:
*/

#ifndef EDITORACTION_H
#define EDITORACTION_H
#include <QWidget>
#include <iostream>
#include <QTabWidget>
#include "config.h"
#include "initwindow.h"
#include <QTextBrowser>
#include <QTreeView>
#include <QLineEdit>
#ifdef WIN32
#include <windows.h>
#endif

//#define EDITOR   static_cast<CodeEditor *>(tabWidget->currentWidget())
using namespace std ;

class CFDbetaEditorAction : public QWidget
{
	Q_OBJECT
public:
	explicit CFDbetaEditorAction(Config *config, QWidget *parent = 0);
	//  define the editor widget--->tabWidget
	QTabWidget *Container ;
	
	//  define the File Menu
private:
	Config *config ;
	
};
#endif //EDITORACTION_H
