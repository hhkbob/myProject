#ifndef PERFERENCEDIALOG_H
#define PERFERENCEDIALOG_H

#include <QDialog>
#include "ui_perferencedialog.h"
#include "config.h"
//#include "notepad.h"

class PerferenceDialog: public QDialog, private Ui::PerferenceDialog
{
    Q_OBJECT
public:
    explicit PerferenceDialog(Config *config, QWidget *parent = 0);
	void closeEvent(QCloseEvent *e);
    
signals:

public slots:
private slots:
	void acceptDialog();
    void apply();
   
private:
	Config *config;
};

#endif // PERFERENCEDIALOG_H
