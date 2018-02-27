/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: FileOpen.h
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

#ifndef FILEOPEN_H
#define FILEOPEN_H

#include "pqLoadDataReaction.h"

class FileOpen: public QWidget
{
	Q_OBJECT
	public:
		QString buildFileOpenTrigger( );
		QList<pqPipelineSource*> loadData();
};

#endif //FILEOPEN_H
