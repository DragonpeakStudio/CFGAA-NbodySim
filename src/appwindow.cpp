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

void AppWindow::addParticles(const std::vector<Particle> &_particles)
{
  m_renderWidget->particleSystem()->addToFrame(((FrameSlider*)m_frameSlider->widget())->getFrame(), _particles);
  m_renderWidget->update();
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

  m_frameSlider = new QDockWidget("Playback");
  m_frameSlider->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  m_frameSlider->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
  m_frameSlider->setWidget(new FrameSlider());
  addDockWidget(Qt::BottomDockWidgetArea, m_frameSlider);
  m_fileMenu = menuBar()->addMenu("File");
  m_fileMenu->addAction("Open", [this]()
  {
    QFileDialog d = QFileDialog(this, "Load Simulation");
    d.setAcceptMode(QFileDialog::AcceptOpen);
    d.setFileMode(QFileDialog::FileMode::ExistingFile);
    d.setOption(QFileDialog::DontUseNativeDialog,true);
    if(QDialog::Accepted == d.exec())
    {
      QMessageBox message(QMessageBox::Icon::Information, "Loading...", "Loading...");
      message.show();

      std::ifstream in(d.selectedFiles()[0].toStdString());
      m_renderWidget->particleSystem()->deserialize(in);
      in.close();

      message.close();

    }
    m_renderWidget->update();
  });
  m_fileMenu->addAction("Save", [this]()
  {
    QFileDialog d = QFileDialog(this, "Save Simulation");
    d.setAcceptMode(QFileDialog::AcceptSave);
    d.setFileMode(QFileDialog::FileMode::AnyFile);
    d.setOption(QFileDialog::DontUseNativeDialog,true);
    if(QDialog::Accepted == d.exec())
    {
      QMessageBox message(QMessageBox::Icon::Information, "Saving...", "Saving...");
      message.show();
      
      std::ofstream out(d.selectedFiles()[0].toStdString());
      m_renderWidget->particleSystem()->serialize(out);
      out.close();

      message.close();
    }
  });
  m_fileMenu->addAction("Export .geo", [this]()
  {
    QFileDialog d = QFileDialog(this, "Export Simulation");
    d.setAcceptMode(QFileDialog::AcceptSave);
    d.setFileMode(QFileDialog::FileMode::Directory);
    d.setOption(QFileDialog::DontUseNativeDialog,true);
    if(QDialog::Accepted == d.exec())
    {
      QMessageBox message(QMessageBox::Icon::Information, "Saving...", "Saving...");
      message.show();
      
      for(size_t i = 0; i < m_renderWidget->particleSystem()->currentFrameCount(); i++)
      {
        std::stringstream path;
        path << d.selectedFiles()[0].toStdString() << "/" << std::setfill('0') << std::setw(5) << i <<".geo";
        std::ofstream out(path.str());
        m_renderWidget->particleSystem()->serializeToGeo(out, i);
        out.close();
      }

      message.close();
    }
  });

  m_renderWidget = new RenderWidget();
  setCentralWidget(m_renderWidget);
  connect((FrameSlider*)m_frameSlider->widget(), &FrameSlider::frameChanged, m_renderWidget, &RenderWidget::setFrame);
  connect((ControlBar*)m_controlBar->widget(), &ControlBar::simFpsChanged, m_renderWidget, &RenderWidget::setFps);
  connect((ControlBar*)m_controlBar->widget(), &ControlBar::resetPress, m_renderWidget, &RenderWidget::resetSim);
  connect((ControlBar*)m_controlBar->widget(), &ControlBar::addParticles, this, &AppWindow::addParticles);
  connect((ControlBar*)m_controlBar->widget(), &ControlBar::dampCoeffChanged, m_renderWidget, &RenderWidget::setSpringCoeff);
  connect((ControlBar*)m_controlBar->widget(), &ControlBar::springCoeffChanged, m_renderWidget, &RenderWidget::setdampCoeff);


}
