#include "configuration.h"
#include "ui_configuration.h"

configuration::configuration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configuration)
{
    ui->setupUi(this);

    //  add the connection for save and unsave button
    connect( ui->Save, SIGNAL( clicked() ), this, SLOT( SaveClick( ) ) ) ;
    connect( ui->Unsave, SIGNAL( clicked() ), this, SLOT( UnsaveClick( ) ) ) ;
}

configuration::~configuration()
{
    delete ui;
}

int configuration:: SaveClick( )
{
    //  save the directory and return
    QString CFDbetaOpenFoamDir = ui->OpenFoamDir->text()  ;
    FILE *config ;
    QString file = 
    QCoreApplication::applicationDirPath() + "/configuration.ini" ;              
    config = fopen( file.toLocal8Bit().data(), "w" ) ;
    if( config == NULL )
    {
	printf( "\nFATAL ERROR\n\n" ) ;
        printf( "	Configuration is failed\n" ) ;
        printf( "	Cannot create the file in the running directory\n" ) ;
        printf( "	The aministrator private is requsted\n" ) ; 
        printf( "\nFlowCD Exit\n" ) ;
        return 0 ;
    }
    fprintf( config, "%s", CFDbetaOpenFoamDir.toLocal8Bit().data() ) ;
    fclose( config ) ;
    CFDbetaFOAMDir = CFDbetaOpenFoamDir ;
	accept();
    return 1 ;
}
int configuration:: UnsaveClick( )
{
	accept();
    return 1 ;
}

bool configuration:: ConfigurationFileExist( )
{
        //  test the configuration.ini
    FILE *OpenConfig ;
    QString file = 
    QCoreApplication::applicationDirPath() + "/configuration.ini" ; 
    OpenConfig = fopen( file.toLocal8Bit().data(), "r" ) ;
    if( OpenConfig == NULL )
    {
	printf( "\nFATAL ERROR\n\n" ) ;
        printf( "	ConfigurationFileExist( ) function in is failed\n" ) ;
        printf( "	Cannot find the file in the running directory\n" ) ;
        printf( "	Ask the software provider to solve the problem\n" ) ; 
        printf( "\nFlowCD Exit\n" ) ;         
        return false;
    }
    else
    {
        char CFDbetaOpenFoamDir[ 500 ] ;
        fgets( CFDbetaOpenFoamDir, 500, OpenConfig ) ;
        fclose( OpenConfig ) ;
        CFDbetaFOAMDir = QString( CFDbetaOpenFoamDir ) ;
        return true ;
    }
}

