/*================================C++========================================*\
      Create: 2018/03/23
      Creator: Huang Huakun
      File Name: calculate.h
      Platform: windows/linux
      Version: v1.0

      Describe: the yPlus create.

      Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
    CopyRight:
*/


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
