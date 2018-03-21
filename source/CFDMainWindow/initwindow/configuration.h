#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include <stdio.h>

namespace Ui {
class configuration;
}

class configuration : public QDialog
{
    Q_OBJECT

public:
    explicit configuration(QWidget *parent = 0);
    ~configuration();

    //  the directory
    QString CFDbetaFOAMDir ;
    bool ConfigurationFileExist( );

private slots:
    int SaveClick( ) ;
    int UnsaveClick( ) ;

private:
    Ui::configuration *ui;
};

#endif // CONFIGURATION_H
