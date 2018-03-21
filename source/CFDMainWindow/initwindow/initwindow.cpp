/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: initwindow.cpp
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

#include "initwindow.h"
#include "QDir"

initWindow::initWindow(QWidget *parent) :
    QMainWindow(parent)
{
    workPath = QString( "C:\\Users\\hhk\\Desktop" ) ;
}