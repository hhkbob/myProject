/*================================C++========================================*\
      Create: 2018/03/23
      Creator: Huang Huakun
      File Name: ToolTipsForCode.cpp
      Platform: windows/linux
      Version: v1.0

      Describe: create the api.ini file for code tips

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

FILE *tips;

void addTips( char tip[] )
{
    fprintf( tips, "%s\n", tip );
}

int main( )
{
    tips = fopen( "api.ini", "r" );
    if( tips==NULL )
    {
        tips = fopen( "api.ini", "w" );
        addTips( "Foam" );
        addTips( "internalField" );
        addTips( "patch" );
    }
    fclose( tips );
    return 0;
}