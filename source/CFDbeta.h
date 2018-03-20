/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: CFDbeta.cpp
      Platform: windows
      Version: v1.0

      Describe: The header of the software main source file.

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#define WINDOWRUNNING 1
#define LINUXRUNNING  2
#define OPENFOAM_COMMAND     90
#define COMMAND_COMMAND      50
#ifndef CFDBETA_H
#define CFDBETA_H

#include <QMainWindow>
#include <stdlib.h>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include <QStatusBar> 
#include <QMessageBox>
#include <QTreeView>
#include <QDirModel>
#include <QStandardItemModel>
#include <QApplication>
#include <QLineEdit>
#include <QFileDialog> 
#include <QFileSystemWatcher>
#include <QMenu>
#include <QToolBar>

#include "initwindow.h"
#include "OutputMessage.h"
#include "configuration.h"
#include "QVTKView.h"
#include "Editor.h"
#include "Mesh/BlockMesh.h"
//#include "myui.h"

#include "pqLoadDataReaction.h"
#include "pqActiveObjects.h"
#include "pqParaViewBehaviors.h"
#include "pqParaViewMenuBuilders.h"
#include "pqHelpReaction.h"
#include "CFDMainWindow/FileOpen.h"
#include "TabWidget.h"
#include "RunCommand.h"
#include <fstream>
#include <qfile.h>
#include <QTextStream>

//#include "ToolBar/calculator.h"

using namespace std ;

#define  FlowCD MainWindow

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
    typedef QMainWindow Superclass;
    #define EDITOR   static_cast<QsciScintilla *>(tabEditor->currentWidget())
public:
	explicit MainWindow(QWidget *parent=0 );
	~MainWindow();
	int RunningPlatForm;

	initWindow initWin ;
	OutputMessage OutputConsole ;
	configuration ConfigWindow ;
	QVTKView vtkView ;
	RunCommand *RunModel;

	QLabel *labelStatusBar ;
	QFileSystemWatcher Watcher;
	QString CurrentPath;
	
	//  the editor outter path
	char Notepad[ 200 ];
	TabWidget *EditorTab;


	//myUI *editorUI;
	//Config *conf;
	QPushButton *CFD;
	QPushButton *editor;
    QToolBar  *editorToolBar;
	QToolBar  *FileToolBar;
	QPushButton *Mesh;

	//  mainToolBar
    #include "ToolBar/mainToolBar.h"
    //  Mesh Menu
    #include "CFDMainWindow/MeshFunction.h"

	//  menu function and trigger
	FileOpen buildFileOpenTigger;
	
//  CFDbeta.cpp fuction
        
	//  init the tabWindget and QVTKWidget
	int initTabAndQVTKWidget( ) ;
	void initTheQVTKWindow();
	void InitTheMainWindow();
	void DockWidgetAndToolBarState();
		
	
//  dir.h function
	
		
	//  choose the file path for software
	void dirFilePath(char controlDict[],const QModelIndex &index ) ;
	//  set Editor data
    QString setEditorData( QWidget *editor, const QModelIndex &index)const;
	//  change the file name in treeview widget
	bool setModelData( QWidget *editor, QDirModel *model,
		                   const QModelIndex &index )const ;
	//  setup the dir function
	void setUpDir( ) ;

	//  rename mark
	int RenameDir;
	QString OldFileName;
	QStandardItemModel *model;
	QStandardItem **item;
	QMenu *TreeViewMenu;
	QAction *OpenFileOuterAction;
	QTextBrowser *textBrowser;
    QToolBar *Program;
    QAction *main;
    QAction *FindFile;

	//  FileMenu
                
//  EditorToolBar
   // QAction *save ;
   // QAction *newfile;
 
//  editor widget
	QTabWidget *tabEditor;
		
//  CFDMainWindow/initWindow/initConsole.cpp function

    int initConsoleWindow( ) ;   
//  CFDMainWindow/initWindow/initMainWindowAction.cpp function     		
	void initMainWindowAction( ) ;
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	void OpenFile(QString filename);
private slots:
 
//  dir.h slots 
	//  open a file by double clicking file in treeview widget
    void on_treeView_doubleClicked( const QModelIndex &index ) ;
	//  add the command to the treeview widget by left button clicking
    void on_treeView_customContextMenuRequested(const QPoint &pos) ;
    //  refresh the treeview widget
    void dirRefresh( ) ;
    //  create the new file
    void dirNewFile( ) ;
    //  rename for a file
    void dirRename( ) ;
    //  delete a file
    bool dirDelete( ) ;
    //  open a file by outer editor
    void dirOpenNotePad( ) ;
    // rename the item and file
    void RenameItem( );

//  CFDbeta.cpp
    void LineEditRun( ) ;
	void ShowTreeView();
	void ShowTextBrowser();
	void OpenFileImageTrigger();
	void OpenFileEditorTrigger();
	void showHelpForProxy(const QString& groupname,const QString& proxyname);
	void yPlusEstimate();
	void MeshGUITrigger();
	void newFile();
	bool Save();
	bool saveFile(const QString &fileName);
	bool saveAs();
	void fileClose(int);
	bool maybeSave(int index);
	

private:
	Q_DISABLE_COPY(MainWindow)
	Ui::MainWindow *ui;
	int textBrowserMark;
	pqLoadDataReaction *Loadata;
	BlockMesh Meshgui;
	FoamEditor *codeEditor;

	QAction *newfile;
	QAction *savetrigger;
	int newNumber;
	QStringList openedFiles;
	QString curFile;

};

#endif //CFDBETA_H

