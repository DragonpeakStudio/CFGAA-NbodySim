
#include "appwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  AppWindow mainWindow("NBody Particle Simulator ");
  mainWindow.show();  
  return app.exec();
}