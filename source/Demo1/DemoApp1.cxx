#include "myMainWindow.h"
#include "pqApplicationCore.h"
#include <QApplication>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  //pqPVApplicationCore appCore(argc, argv);
  pqApplicationCore appCore(argc, argv);
  myMainWindow window;
  window.show();
  return app.exec();
}
