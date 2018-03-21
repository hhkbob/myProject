/*================================C++========================================*\
      Create: 2018/02/22
      Creator: Huang Huakun
      File Name: mainToolBar.cpp
      Platform: windows/linux
      Version: v1.0

      Describe: The mainToolBar.

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

public:
	void buildMainToolBar(  ) ;
	void deleteMainToolBar( );
        void lookup( char name[], char buffer[], FILE *file );
private slots:
    void BlockMeshTriggered( ) ;
	void WmakeTriggered( );
	int RunTriggered( );
	int pyRunTriggered( );
	void ClearTriggered( );
	
public:
    QToolBar *ToolBar ;
	QAction *blockMesh;
	QAction *wmake;
	QAction *run;
	QAction *pyRun;
	QAction *clear;  
