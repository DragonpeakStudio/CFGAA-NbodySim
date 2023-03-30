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

  m_controlBar = new QDockWidget("Controls");
  m_controlBar->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
  m_controlBar->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
  m_controlBar->setWidget(new ControlBar());
  m_controlBar->setMinimumWidth(200);
  addDockWidget(Qt::RightDockWidgetArea, m_controlBar);

  m_frameSlider = new QDockWidget("Frame");
  m_frameSlider->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  m_frameSlider->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
  m_frameSlider->setWidget(new FrameSlider());
  addDockWidget(Qt::BottomDockWidgetArea, m_frameSlider);
  m_fileMenu = menuBar()->addMenu("File");
  m_fileMenu->addAction("Save", [this]()
  {
    QFileDialog d = QFileDialog(this, "Save Simulation");
    d.setAcceptMode(QFileDialog::AcceptSave);
    d.setFileMode(QFileDialog::FileMode::AnyFile);
    d.setOption(QFileDialog::DontUseNativeDialog,true);
    if(QDialog::Accepted == d.exec())
    {
      std::ofstream out(d.selectedFiles()[0].toStdString());
      m_renderWidget->particleSystem()->serialize(out);
      out.close();
    }
  });
  m_fileMenu->addAction("Open");//TODO implement
  m_fileMenu->addAction("Export .geo");//TODO implement

  m_renderWidget = new RenderWidget();
  setCentralWidget(m_renderWidget);
  connect((FrameSlider*)m_frameSlider->widget(), &FrameSlider::frameChanged, m_renderWidget, &RenderWidget::setFrame);
  connect((FrameSlider*)m_frameSlider->widget(), &FrameSlider::fpsChanged, m_renderWidget, &RenderWidget::setFps);
  connect((ControlBar*)m_controlBar->widget(), &ControlBar::resetPress, m_renderWidget, &RenderWidget::resetSim);

}
