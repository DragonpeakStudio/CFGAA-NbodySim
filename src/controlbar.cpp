#include "controlbar.h"
ControlBar::ControlBar(QWidget *_parent) : QWidget(_parent)
{
  setLayout(new QVBoxLayout(this));
  layout()->setAlignment(Qt::AlignTop);
  m_reset = new QPushButton("Reset Sim", this);
  connect(m_reset, &QPushButton::pressed, [this](){emit resetPress();});
  layout()->addWidget(new QLabel("Simulation Parameters:", this));
  layout()->addWidget(m_reset);
  layout()->addWidget(new QLabel("Controls:", this));
  layout()->addWidget(new QLabel("Add Particles:", this));
  layout()->addWidget(new QLabel("Amount:", this));

  m_particleCount = new QSpinBox(this);
  m_particleCount->setMinimum(128);
  m_particleCount->setMaximum(128*200);

  layout()->addWidget(m_particleCount);

  layout()->addWidget(new QLabel("Position:", this));
  m_newPosition = new Vector3DSpinBox(this, -99999, 99999);
  layout()->addWidget(m_newPosition);
  layout()->addWidget(new QLabel("Radius:", this));
  m_radius = new QDoubleSpinBox(this);
  m_radius->setMinimum(0);
  m_radius->setMaximum(9999);
  layout()->addWidget(m_radius);
  layout()->addWidget(new QLabel("Direction:", this));
  m_newDirection = new Vector3DSpinBox(this, -1, 1, true);
  layout()->addWidget(m_newDirection);
  layout()->addWidget(new QLabel("Speed:", this));
  m_speed = new QDoubleSpinBox(this);
  m_speed->setMinimum(0);
  m_speed->setMaximum(9999);
  layout()->addWidget(m_speed);

  m_addParticles = new QPushButton("Add Particles", this);
  connect(m_addParticles, &QPushButton::pressed, this, &ControlBar::generateParticles);
  layout()->addWidget(m_addParticles);

}

ControlBar::~ControlBar()
{

}

void ControlBar::generateParticles()
{
  std::vector<Particle> particles;
  particles.reserve(m_particleCount->value());
  std::mt19937 rnd(std::random_device{}());
  std::uniform_real_distribution<float> pos(-1, 1);
  std::uniform_real_distribution<float> col(0,1);

  for(size_t i = 0; i < m_particleCount->value(); i++)
  {
    ngl::Vec3 p = ngl::Vec3(1,1,1);
    while(p.lengthSquared() > 1.)
    {
      p = ngl::Vec3{pos(rnd), pos(rnd), pos(rnd)};
    }
    p=p*m_radius->value()+m_newPosition->getValue();
    particles.emplace_back(Particle{ngl::Vec4(p.m_x, p.m_y, p.m_z, 1.), m_newDirection->getValue()*m_speed->value(), 1., ngl::Vec3(.5,.5,.5), 1.});
  }
  emit addParticles(particles);
}
