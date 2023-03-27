#include "appwindow.h"

AppWindow::AppWindow(const QString &_title, QWidget *_parent) : QMainWindow(_parent)
{
  setWindowTitle(_title);
}

AppWindow::AppWindow(QWidget *_parent) : QMainWindow(_parent)
{
}

AppWindow::~AppWindow()
{

}