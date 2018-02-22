
#include "CFDbeta.h"

void MainWindow::buildMeshFunction( )
{
    connect(ui->ReadMesh, SIGNAL(triggered()),this,SLOT(ImportTriggered()));
    connect(ui->Check, SIGNAL(triggered()),this,SLOT(CheckMeshTriggered()));
    connect(ui->Refine, SIGNAL(triggered()),this,SLOT(RefineTriggered()));
}

int MainWindow::ImportTriggered( )
{
	QString Filters = "fluent(*.msh)::All files(*.*)";
    //  use our own fileDialog
    QString FileName = QFileDialog::getOpenFileName(this, 
            tr("Import the mesh file"),CurrentPath, Filters);
    QFileInfo FileInfo(FileName);
    if( !FileInfo.isFile() ) return 0 ;

	//  copy the file to current Folder
	char tmp[500];
#ifdef WIN32
	sprintf(tmp, "copy %s %s/%s", FileName.toLocal8Bit().data(),
		CurrentPath.toLocal8Bit().data(), FileName.section('/', -1).
		toLocal8Bit().data());
	QString file = initWin.QtPathToCPath(QString(tmp));
	sprintf(tmp,"%s", file.toLocal8Bit().data());
#else
	sprintf(tmp, "cp -rf %s %s/%s", FileName.toLocal8Bit().data(),
		CurrentPath.toLocal8Bit().data(), FileName.section('/',-1).
		toLocal8Bit().data() );
#endif
#ifdef _DEBUG
	cout << "The Mesh command is = " << tmp << endl;
#endif
	system( tmp );

    QString type = FileName.section('.', -1 ) ;
    char command[200] ;
	if (initWin.StringCompare(type.toLocal8Bit().data(), "msh"))
	{
		sprintf(command, "fluentMeshToFoam %s", FileName.toLocal8Bit().data());
	}
	else if (initWin.StringCompare(type.toLocal8Bit().data(), "stl"))
	{
		sprintf(command, "snapyHexMesh %s", FileName.toLocal8Bit().data());
	}
	else
	{
		cout << "cannot find the aproper ReadMesh Tool\n";
		return 0;
	}	
    RunModel->CurrentPath = CurrentPath ;
    RunModel->Command( LINUXRUNNING, QString(command), OPENFOAM_COMMAND );  
}
void MainWindow::CheckMeshTriggered( )
{
    RunModel->CurrentPath = CurrentPath ;
    RunModel->Command( LINUXRUNNING, "checkMesh", OPENFOAM_COMMAND ); 
}
void MainWindow::RefineTriggered( )
{
    RunModel->CurrentPath = CurrentPath ;
    RunModel->Command( LINUXRUNNING, "RefineMesh", OPENFOAM_COMMAND ); 
}







