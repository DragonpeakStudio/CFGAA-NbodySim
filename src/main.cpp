// Copyright (c) 2023 Rowan Cruse Howse
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include "appwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  AppWindow mainWindow("NBody Particle Simulator ");
  mainWindow.show();  
  return app.exec();
}