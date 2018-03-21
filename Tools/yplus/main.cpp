#include "calculator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	calculator w;
	w.show();
	return a.exec();
}
