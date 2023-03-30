#include "renderwidget.h"

RenderWidget::RenderWidget()
{

}

RenderWidget::~RenderWidget()
{

}

void RenderWidget::initializeGL()
{

  const std::string drawShader = "particleDraw";
  const std::string updateProcessShader = "computeProcess";

  ngl::NGLInit::initialize();

  glEnable(GL_DEPTH_TEST);
  glClearColor(.1,.1,.1,1.);

  ngl::ShaderLib::createShaderProgram(drawShader);
  ngl::ShaderLib::attachShader("particleDrawVert", ngl::ShaderType::VERTEX);
  ngl::ShaderLib::attachShader("particleDrawFrag", ngl::ShaderType::FRAGMENT);
  ngl::ShaderLib::loadShaderSource("particleDrawVert", "shaders/particle.vert");
  ngl::ShaderLib::compileShader("particleDrawVert");
  ngl::ShaderLib::loadShaderSource("particleDrawFrag", "shaders/particle.frag");
  ngl::ShaderLib::compileShader("particleDrawFrag");
  ngl::ShaderLib::attachShaderToProgram(drawShader, "particleDrawVert");
  ngl::ShaderLib::attachShaderToProgram(drawShader, "particleDrawFrag");
  ngl::ShaderLib::linkProgramObject(drawShader);

  ngl::ShaderLib::createShaderProgram(updateProcessShader);
  ngl::ShaderLib::attachShader("updateProcessShaderComp", ngl::ShaderType::COMPUTE);
  ngl::ShaderLib::loadShaderSource("updateProcessShaderComp", "shaders/updateprocess.comp");
  ngl::ShaderLib::compileShader("updateProcessShaderComp");
  ngl::ShaderLib::attachShaderToProgram(updateProcessShader, "updateProcessShaderComp");
  ngl::ShaderLib::linkProgramObject(updateProcessShader);

  {//scope to make sure we cannot access moved tmp var
    auto a = std::make_unique<ngl::Obj>("geo/particle.obj");
    a->createVAO();
    m_particleRenderer = std::make_unique<ParticleRenderer>(drawShader, std::move(a));
  }
  m_particleSystem = std::make_unique<NBodyParticleSystem>(updateProcessShader);

  m_camera.m_pos = ngl::Vec3(2,2,2);
  m_particleSystem->addToFrame(0, std::vector<Particle>{Particle{ngl::Vec4(0,0,0,1), ngl::Vec3(0,0,0), ngl::Vec3(1.,.5,.5), 1,1},Particle{ngl::Vec4(1,0,0,1), ngl::Vec3(0,0,0), ngl::Vec3(0.,1.,.5), 1,1}});
}

void RenderWidget::paintGL()
{
  glViewport(0,0, size().width(), size().height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_particleRenderer->drawParticles(m_particleSystem->getFrame(0), m_camera.projMat((float)size().width()/(float)size().height())*m_camera.viewMat());
}

void RenderWidget::resizeGL(int w, int h)
{
}
void RenderWidget::mouseMoveEvent(QMouseEvent *_event)
{
  auto pos = _event->position();
  auto diff = pos-m_lastMousePos;
  if(_event->modifiers() == Qt::ShiftModifier)
  {
    m_camera.pan(ngl::Vec2(diff.x(), diff.y())*m_panScale);
  }
  else if(_event->modifiers() == Qt::ControlModifier)
  {
    m_camera.zoom(diff.y()*m_zoomScale);
  }
  else
  {
    m_camera.orbit(ngl::Vec2(diff.x(), diff.y())*m_orbitScale);
  }
  
  update();
  m_lastMousePos = pos;
}

void RenderWidget::mousePressEvent(QMouseEvent *_event)
{
  m_lastMousePos = _event->position();
}

void RenderWidget::wheelEvent(QWheelEvent *_event)
{
  m_camera.zoom(_event->angleDelta().y()*m_zoomScale);
  update();
}