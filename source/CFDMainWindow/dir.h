/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: dir.h
      Platform: windows
      Version: v1.0

      Describe: The header file for a treeview widget. it includes many 
	            functions. Refreshing, openFile, Rename, Delete, Create

				Two class in this file
				CFDbetaTreeView : define the treeView action
				MainWindow: setUp the folder view

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

//  the strcmp function is not recommanded, use own function of StringCompare

/*
    CopyRight:
*/

//  define the CFDbetaTreeView

#define NO_RENAME 0
#define IS_RENAME 1

#include "QTreeView"
#include "QMouseEvent"
#include "qdebug.h"
#include "iostream"
#include "QString"


class CFDbetaTreeView :public QTreeView
{
public:
	CFDbetaTreeView(QStandardItemModel *, QStandardItem **);
	~CFDbetaTreeView();
	void iterateOverItems(  );
	QList<QStandardItem*> returnTheItems(  );
	void mouseDoubleClickEvent(QMouseEvent *event);
	bool initTheModel(QTreeView *treeView, QString path );
	bool ShowTheFileLIsts(QStandardItem *items, QString path );
	void SetItemIcon(QStandardItem *items, QString fileName );

	// add the items ;
	QStandardItem **item;
	QStandardItemModel *model;
	initWindow initWin;
private:

};

CFDbetaTreeView::CFDbetaTreeView(QStandardItemModel *model1, QStandardItem **item1)
{
	model = model1;
	item = item1;
}

CFDbetaTreeView::~CFDbetaTreeView()
{

}
QList<QStandardItem*> CFDbetaTreeView::returnTheItems(  )
{
	return model->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);
}

void CFDbetaTreeView::iterateOverItems(  )
{
	QList<QStandardItem*> list = returnTheItems(  );
	foreach(QStandardItem* item, list)
	{
		qDebug() << item->text();
	}
}

void CFDbetaTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		QModelIndex index0 = currentIndex();
		qDebug() << index0.data().toString();
	}
}
bool CFDbetaTreeView::ShowTheFileLIsts(QStandardItem *items, QString path)
{
	return true;
}

bool CFDbetaTreeView::initTheModel( QTreeView *treeView, QString path )
{
	int isFolder = 0;
	int isFile = 0;
	QDir ViewPath( path );
	ViewPath.setSorting(QDir::DirsFirst);
	QStringList string;
	string << "*";
	QFileInfoList list = ViewPath.entryInfoList(string);

	for (unsigned int i = 0; i<list.count(); i++)
	{
		QFileInfo tmpFileInfo = list.at(i);
		if ( tmpFileInfo.fileName() == ".")
		{
			continue;
		}
		if (tmpFileInfo.fileName() == ".." )
		{
			continue;
		}
		if (tmpFileInfo.isDir())
		{
			isFolder++;
		}
		else if (tmpFileInfo.isFile())
		{
			isFile++ ;
		}
	}
	int ItemNumber = isFolder+ isFile ;
	if (model != NULL)
	{
		delete model;
	}
	if (item != NULL)
		delete item;
	ItemNumber = ItemNumber + 1;
	model = new QStandardItemModel( ItemNumber, 1);
	model->setHeaderData(0, Qt::Horizontal, tr("Program"));
	item = new QStandardItem *[ ItemNumber ];
	
	// set the first item which is back to upper path
	item[0] = new QStandardItem("..");
	item[0]->setIcon(QIcon(":/Resource/512_512/upper.png"));
	model->setItem(0, 0, item[0]);

	int itemNumCount = 1;
	QModelIndex parent;
	for (int i = 0; i < list.count(); i++)
	{
		QFileInfo tmpFileInfo = list.at(i);
		if (tmpFileInfo.fileName() == "." )
		{
			continue;
		}
		if (tmpFileInfo.fileName() == "..")
		{
			continue;
		}
		if (tmpFileInfo.isDir())
		{
			QString fileName = tmpFileInfo.fileName();
			item[itemNumCount] = new QStandardItem(fileName);
			item[itemNumCount]->setIcon( QIcon(":/Resource/512_512/folder.png") );
			model->setItem(itemNumCount,0, item[itemNumCount]);
			itemNumCount++;
		}
		else if (tmpFileInfo.isFile())
		{
			QString fileName = tmpFileInfo.fileName();
			item[itemNumCount] = new QStandardItem(fileName);
			model->setItem(itemNumCount, 0, item[itemNumCount]);
			SetItemIcon(item[itemNumCount], fileName);
			itemNumCount++;
		}
	}
	treeView->setModel(model); 
	return true;
}

void CFDbetaTreeView::SetItemIcon(QStandardItem *items, QString fileName )
{
	//  set txt file icon
	if( initWin.StringCompare(fileName.section('.', -1).
		toLocal8Bit().data(), "txt") )
	{
		items->setIcon(QIcon(":/Resource/file_txt.png"));
	}
}


//-------------------- over define the CFDbetaTreeView ------------------

void MainWindow::dirFilePath(char controlDict[],const QModelIndex &index )
{
    QString str ;
    QModelIndex secondItem = index ;
    QModelIndex temp = index ;
    char path1[2000] ;
    controlDict[0] = '\0' ;
    while( temp.parent().isValid() )
    {
        secondItem = temp.parent( ) ;
        str = QString( "/%1" ).arg( temp.parent().data().toString() ) ;
        initWin.stringAdd( str.toLocal8Bit().data(), controlDict, path1 ) ;
        strcpy(controlDict, path1 ) ;
        temp = secondItem ;
    }
    int i =0 ;
    for(unsigned int k =0; k<strlen(path1); k++ )
    {
        if(k==0) ;
        else
        {
            controlDict[i] = path1[k] ;
            i = i+ 1 ;
        }
    }
    controlDict[i] = '\0' ;
}
void MainWindow::on_treeView_doubleClicked(const QModelIndex &index) 
{
	QString FileName;
	//  isFoleder or isFile
    FileName += QStringLiteral("%1").arg(index.data().toString());
	FileName = CurrentPath + "/" + FileName;
    QFileInfo FileInfo(FileName);
	if (FileInfo.isDir())
	{
		// the Upper path
		QString file = FileName.section('/', -1);
		if (initWin.StringCompare(file.toLocal8Bit().data(), ".."))
		{
			// get the upper path
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
			UpperPath[MarkPath] = '\0';
			int Dir = strlen(UpperPath);
			//  show the UpperPath in the treeView Widget			 
#if _DEBUG
			printf("This is a test information for developer\n\n");
			printf("The Current Path is :\n");
			printf("The Length = %d, %s\n", Dir, UpperPath);
			printf("The old path is:\n");
			printf("%s\n\n", CurrentPath.toLocal8Bit().data());
			printf("Debug exit\n\n");
#endif
			QFileInfo DirInfo(UpperPath);
			if (DirInfo.isDir())
			{
				if (Dir==2||Dir==1)  // "*:" and "*"
				{
					cout << "cannot Open directory\n";
				}
				else
				{
					CurrentPath = QString(UpperPath);
					CFDbetaTreeView TreeView(model, item);
					TreeView.initTheModel(ui->treeView, QString(CurrentPath));
					Watcher.addPath(CurrentPath);
				}
			}
		}
		else  // not the upper folder
		{
			CurrentPath = FileName;
			CFDbetaTreeView TreeView(model, item);
			TreeView.initTheModel(ui->treeView, FileName);
			Watcher.addPath(CurrentPath);
		}
	}
	else // is file
	{
		QString runFileBash;
		runFileBash = FileName.section('.', -1);
		if (initWin.StringCompare(runFileBash.toLocal8Bit().data(), "bat"))
		{
			RunModel->CurrentPath = CurrentPath;
			RunModel->Command( WINDOWRUNNING, FileName, COMMAND_COMMAND );
		}
		else if (initWin.StringCompare(runFileBash.toLocal8Bit().data(), "sh"))
		{
			RunModel->CurrentPath = CurrentPath;
			RunModel->Command( LINUXRUNNING, FileName, OPENFOAM_COMMAND );
		}
		else
		{
			editorUI->openFile(FileName);
			if( tabEditor->isHidden())
				  tabEditor->show();				
			//  change the StatusBar
			labelStatusBar->setText(FileName);
		}
	} 
}

QString MainWindow::setEditorData( QWidget *editor,
                                   const QModelIndex &index)const
{
    QString value=index.model()->data(index,Qt::EditRole).toString();
    QLineEdit *TextBox=static_cast<QLineEdit*>(editor);
    TextBox->setText(value);

    //  get the file information
    QFileInfo tmpInfo(value);
    QString startExt=tmpInfo.suffix();
    return startExt ;
}

bool MainWindow::setModelData( QWidget *editor, QDirModel *model,
                               const QModelIndex &index )const
{
   /* QLineEdit *TextBox=static_cast<QLineEdit*>(editor);
   / QString value=TextBox->text();
	
    //  get the file information
    QFileInfo tmpInfo(value);
    QString endExt=tmpInfo.suffix();

    // if the file change, give the warning
    if(setEditorData( editor, index )!=endExt)
    {
      QMessageBox::StandardButton res= QMessageBox::warning(NULL,
                    "warning", "Please confirm the change！",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       if(res==QMessageBox::No)
            return false;
       if(res==QMessageBox::Yes)
            model->setData(index, value, Qt::EditRole);
     }*/
     return true ;
}
void MainWindow::dirRename()
{
	QModelIndex index = ui->treeView->currentIndex();
	ui->treeView->edit( index );
}

void MainWindow::dirRefresh()
{
    
	CFDbetaTreeView TreeView( model, item );
	TreeView.initTheModel(ui->treeView, CurrentPath);
}
void MainWindow::dirNewFile()
{
    char filepath[2000] ;
    QModelIndex index=ui->treeView->currentIndex();
    dirFilePath( filepath, index ) ;
	
    // create the file in the current folder
    strcat( filepath, "/new.C" ) ;
    FILE *file ;
    file = fopen( filepath, "w" ) ;
    fclose( file ) ;
    dirRefresh() ;
    labelStatusBar->setText( tr("Create file \"new.C\"") ) ;
}

bool MainWindow::dirDelete()
{
    char filepath[2000] ;
    QString filename ;
    QModelIndex index=ui->treeView->currentIndex();
	
	//  get the file name
    filename += QStringLiteral("/%1").arg(index.data().toString() ) ; 
    dirFilePath( filepath, index ) ;
    strcat( filepath, filename.toLocal8Bit().data() ) ;
	
    //  the information
    QMessageBox::StandardButton res= QMessageBox::warning(NULL,
                 "warning", "Please confirm the delete oporation",
                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(res==QMessageBox::No)
          return false;
    if(res==QMessageBox::Yes)
    {
       FILE *file ;
       if( (file=fopen(filepath,"r"))) 
       {
           fclose( file ) ;
           remove( filepath ) ;
           dirRefresh() ;
		   QString tmp = "delete the file \"." + filename + "\"";
           labelStatusBar->setText(  tmp ) ;
       }
       else
            QMessageBox::warning
		    (
		        NULL, "warning", 
		        "The file is occupied and the operation failed",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes
		    );                                                                   
    }
    return true ;
}
void MainWindow::dirOpenNotePad()
{
    QString filename ;
    QModelIndex index=ui->treeView->currentIndex();
    filename = QStringLiteral("%1").arg(index.data().toString() ) ;
	filename = CurrentPath + "/" + filename ;
    QFileInfo info( filename ) ;
    if( info.isDir() )
    {
        QString Message, Position;
	Message = "This is a Folder and it cannot be open";
	Position = "Open File in Outer Editor";
	OutputConsole.OutputMessageInWindow(Message, Position);
    }
    else{
    int mark ;
	char COMMAND[500] = { '\0' };
	initWin.stringAdd(Notepad, "  ", COMMAND );
	initWin.stringAdd(COMMAND, filename.toLocal8Bit().data(), COMMAND);
    mark = system(COMMAND ) ;
    if( mark == 0 ) ;
    else
    {
	QString Message, Position;
	Message = "Cannot find the Outer Editor Path";
	Position = "Open File in Outer Editor";
	OutputConsole.OutputMessageInWindow(Message, Position);
    }}
}

void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos) 
{
	FILE *ReadNotepad;
        QString file = QCoreApplication::applicationDirPath() + "/notepad.ini";
	ReadNotepad = fopen(file.toLocal8Bit().data(), "r");
	if (TreeViewMenu != NULL)
		delete TreeViewMenu;
	if (OpenFileOuterAction != NULL)
		delete OpenFileOuterAction;
	TreeViewMenu = new QMenu;
	OpenFileOuterAction = new QAction( ui->treeView ) ;
	OpenFileOuterAction->setText("Open File in Outter");
	TreeViewMenu->addAction( OpenFileOuterAction );
	connect(OpenFileOuterAction, SIGNAL(triggered(bool)),
		    this, SLOT(dirOpenNotePad()));
	if (ReadNotepad == NULL)
	{
		OpenFileOuterAction->setEnabled(false);
	}
	else
	{
		fscanf(ReadNotepad, "%s", Notepad);
		fclose(ReadNotepad);
	}
	TreeViewMenu->exec(QCursor::pos());
}
void MainWindow::RenameItem()
{
	QModelIndex index = ui->treeView->currentIndex();
	QString FileName = QStringLiteral("%1").arg(index.data().toString());
	FileName = CurrentPath + "/" + FileName;
}

void MainWindow::setUpDir( )
{
	model = NULL;
	item = NULL;
	OpenFileOuterAction = NULL;
	TreeViewMenu = NULL;
	CFDbetaTreeView TreeViewDir( model, item ) ;
	RenameDir = NO_RENAME ;
        CurrentPath = QCoreApplication::applicationDirPath() ;
	ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	TreeViewDir.initTheModel(ui->treeView, CurrentPath );

	//  Watch the directory 
        connect(&Watcher, SIGNAL(directoryChanged(QString)),
         	this, SLOT( dirRefresh())); 
	connect(&Watcher, SIGNAL(fileChanged(QString)), this, SLOT(dirRefresh()));
	
	Watcher.addPath( CurrentPath );

	//  set the outer editor path for windows
	FILE *ReadNotepad;
        QString file = QCoreApplication::applicationDirPath() + "/notepad.ini" ;
	ReadNotepad = fopen( file.toLocal8Bit().data(), "r");
	if (ReadNotepad == NULL)
	{
                
		ReadNotepad = fopen( file.toLocal8Bit().data(), "w");
		#ifdef WIN32
			fprintf(ReadNotepad, "C:/windows/system32/notepad.exe");
                #else 
                        fprintf( ReadNotepad, "gedit" ) ;
                #endif 
		fclose(ReadNotepad);
	}
}
