/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: FileOpen.cpp
      Platform: windows/linux
      Version: v1.0

      Describe: To be Open File.

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include "FileOpen.h"
#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqCoreUtilities.h"
#include "pqFileDialog.h"
#include "pqObjectBuilder.h"
#include "pqPipelineSource.h"
#include "pqSelectReaderDialog.h"
#include "pqServer.h"
#include "pqStandardRecentlyUsedResourceLoaderImplementation.h"
#include "pqUndoStack.h"
#include "vtkSMProxy.h"
#include "vtkSMProxyManager.h"
#include "vtkSMReaderFactory.h"
#include "vtkStringList.h"

#include "QFileDialog"


#include <QDebug>

QString FileOpen::buildFileOpenTrigger()
{
	QString FileName = QFileDialog::getOpenFileName(this, tr("Open File in the editor area"),
		".", tr("All Files(*.*)"));
	return FileName;
}

QList<pqPipelineSource*> FileOpen::loadData()
{
	pqServer* server = pqActiveObjects::instance().activeServer();
	vtkSMReaderFactory* readerFactory = vtkSMProxyManager::
		GetProxyManager()->GetReaderFactory();
	QString filters = readerFactory->GetSupportedFileTypes(server->session());
	if (!filters.isEmpty())
	{
		filters += ";;";
	}
	filters += "All files (*)";

	//  use our own fileDialog
	QString FileName = QFileDialog::getOpenFileName(this, tr("Open File for a data view"),
		".", filters);
	QFileInfo FileInfo(FileName);
	QList<pqPipelineSource*> sources;
	if (!FileInfo.isFile()) return sources;

	QStringList files;
	files.append(FileName.toLocal8Bit().data());
	pqPipelineSource* source = pqLoadDataReaction::loadData( files );
	if (source)
	{
			sources << source;
	}
	return sources;
}