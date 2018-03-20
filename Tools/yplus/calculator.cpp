#include "calculator.h"

calculator::calculator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.Compute, SIGNAL(clicked()), this, SLOT(calculation()));
}

int calculator::calculation()
{
	ui.textBrowser->clear();
	int ok = testInput();
	if (!ok)
		return 0;

	//  calculate the Re and distance
	Re = pho*v*L / nut;
	Cf = 0.026 / pow(Re, 1 / 7);
	shear = Cf*pho*v*v / 2;
	Ufric = sqrt(shear / pho);
	distance = yplus*nut / (Ufric*pho);

	char TEXT[100];
	sprintf(TEXT, "The first grid to wall distance is: %lf\n", distance);
	ui.textBrowser->append(TEXT);
	sprintf(TEXT, "The Reynolds number is: %g\n", Re);
	ui.textBrowser->append(TEXT);

	//  the flux in full developed
	ui.textBrowser->append("In fully developed\n");
	//  turbulence intensity
	return 1;
}

int calculator::testInput()
{
	Velosity = ui.velocity->text();
	Lengh = ui.length->text();
	Viscosity = ui.viscosity->text();
	Density = ui.density->text();
	yPlus = ui.yPlus->text();

	//  string to double
	bool ok;
	pho = Density.toDouble(&ok);
	if (!ok)
	{
		ui.textBrowser->append("The Density input invalid\n");
		return 0;
	}
	nut = Viscosity.toDouble(&ok);
	if (!ok)
	{
		ui.textBrowser->append("The Dynamic viscosity input invalid\n");
		return 0;
	}
	v = Velosity.toDouble(&ok);
	if (!ok)
	{
		ui.textBrowser->append("The velosity input invalid\n");
		return 0;
	}
	L = Lengh.toDouble(&ok);
	if (!ok)
	{
		ui.textBrowser->append("The length input invalid\n");
		return 0;
	}
	yplus = yPlus.toDouble(&ok);
	if (!ok)
	{
		ui.textBrowser->append("The yplus input invalid\n");
		return 0;
	}
	return 1;
}
