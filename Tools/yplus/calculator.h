/*==========================c++==================================
     



/*==============================================================*/


#include <QMainWindow>
#include "ui_calculator.h"

class calculator : public QMainWindow
{
	Q_OBJECT

public:
	calculator(QWidget *parent = Q_NULLPTR);
	QString Velosity;
	QString Lengh;
	QString Viscosity;
	QString Density;
	QString yPlus;

	double Re ;
	double Cf ;
	double shear ;
	double Ufric ;
	double distance ;

	double v, L, pho, yplus, nut;

	int testInput();

private:
	Ui::calculatorClass ui;

	private slots:
	int calculation();
};
