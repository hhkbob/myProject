/*================================C++========================================*\
      Create: 2018/02/09
      Creator: Huang Huakun
      File Name: stringToChange.cpp
      Platform: windows
      Version: v1.0

      Describe: the string and other string change

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/

#include "initwindow.h"

QString initWindow::FilePathChange( QString filePath )
{
    int length = filePath.length() ;
    QString result = "/mnt/" ;
    char temp[200] ;
    int k = 0 ;
    for( int i=0; i<length; i++ )
    {
       if( filePath.toLocal8Bit().data()[i]=='\\')
           temp[k] = '/' ;
       else if( filePath.toLocal8Bit().data()[i] ==':' )
            { temp[k-1]=temp[k-1]+32 ;continue ; }
       else
           temp[k] = filePath.toLocal8Bit().data()[i] ;
       k = k + 1 ;
     }
     temp[k] = '\0' ;
     result += QString( QLatin1String( temp ) ) ;
     return result ;
}
QString initWindow::QtPathToCPath( QString path)
{
    int length = path.length(), key=0 ;
    char *tmp ;
    tmp = new char[500] ;
    for( key=0; key<length; key++ )
    {
        if( path.toLocal8Bit().data()[key]=='/')
            tmp[key] = '\\' ;
        else
            tmp[key] = path.toLocal8Bit().data()[key] ;
    }
    tmp[key] = '\0' ;
    QString re = QString( tmp ) ;
    delete tmp ;
    return re ;
}
QString initWindow::CPathToQtPath( QString path )
{
    int length = path.length(), key=0 ;
    char *tmp ;
    tmp = new char[500] ;
    for( key=0; key<length; key++ )
    {
        if( path.toLocal8Bit().data()[key]=='\\')
            tmp[key] = '/' ;
        else
            tmp[key] = path.toLocal8Bit().data()[key] ;
    }
    tmp[key] = '\0' ;
    QString re = QString( tmp ) ;
    delete tmp ;
    return re ;
}

void initWindow::stringAdd( char str1[], char str2[], char des[] )
{
  /* //for example
     //str1[] = "hello world" ;
     //str2[] = " Welcome" ;
     //des can be "hello world Welcome"
  */
   size_t len_str1 = strlen( str1 ) ;
   size_t len = len_str1 + strlen( str2 ) ;
   size_t i = 0 ;
     int k = 0 ;
    for( i=0; i<len ; i++)
    {
        if( i< len_str1 )
            des[i]=str1[i];
        else
           {
            des[i] = str2[k] ;
                  k = k+1 ;
            }
    }
    des[i] = '\0' ;
 }

//  the string compared 
bool initWindow::StringCompare(char str1[], char str2[] )
{
	//  compared the length
	int length = strlen(str1);
	int len = strlen(str2);
	if (len != length)
		return false;
	else
	{
		for (int i = 0; i < len; i++)
		{
			if (str1[i] != str2[i])
				return false;
		}
	}
	return true;
}
