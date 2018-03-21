/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: initwindow.h
      Platform: windows
      Version: v1.0

      Describe: init the window

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#ifndef INITWINDOW_H
#define INITWINDOW_H

#include <QMainWindow>
#include <stdlib.h>
#include <iostream>

using namespace std ;

class initWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit initWindow(QWidget *parent = 0);
	
	//  current work path
	QString workPath ;
	
//  stringToChange.cpp

	//  windows to linux path changes 
    QString FilePathChange( QString filePath ) ; 
    //  Qt path to c path	
    QString QtPathToCPath( QString path) ;
    //  C path to Qt Path 	
    QString CPathToQtPath( QString path ) ; 
    //  string add	
    void stringAdd( char str1[], char str2[], char des[] ) ;

	// string compare
	bool StringCompare(char str1[], char str2[]);

signals:

public slots:
public:

};

#endif // INITWINDOW_H
