#include "appwindow.h"

AppWindow::AppWindow(const QString &_title, QWidget *_parent) : QMainWindow(_parent)
{
  setWindowTitle(_title);
  setupUI();
}

AppWindow::AppWindow(QWidget *_parent) : QMainWindow(_parent)
{
  setupUI();

}

AppWindow::~AppWindow()
{

}

void AppWindow::setupUI()
{
  resize(QSize(1280,720));

  m_fileMenu = menuBar()->addMenu("File");
  m_fileMenu->addAction("Save");
  m_fileMenu->addAction("Open");
  m_fileMenu->addAction("Export .geo");//TODO implement

  m_controlBar = new QDockWidget("Controls");
  m_controlBar->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
  m_controlBar->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
  m_controlBar->setWidget(new ControlBar());
  m_controlBar->setMinimumWidth(400);
  addDockWidget(Qt::RightDockWidgetArea, m_controlBar);

  m_renderWidget = new RenderWidget();
  setCentralWidget(m_renderWidget);

}
