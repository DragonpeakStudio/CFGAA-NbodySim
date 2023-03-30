
#include "appwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QSurfaceFormat fmt;
  fmt.setSamples(4);
  fmt.setMajorVersion(4);
  fmt.setMinorVersion(5);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  fmt.setDepthBufferSize(24);
  QSurfaceFormat::setDefaultFormat(fmt);
  AppWindow mainWindow("NBody Particle Simulator");
  mainWindow.show();  
  return app.exec();
}
