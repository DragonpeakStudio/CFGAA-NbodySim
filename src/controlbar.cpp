#include "controlbar.h"
#include <QLabel>
#include <QColorDialog>
#include <random>
#include <QVBoxLayout>

ControlBar::ControlBar(QWidget *_parent) : QWidget(_parent)
{
  setLayout(new QVBoxLayout(this));
  layout()->addWidget(new QLabel("Simulation Parameters:", this));
  layout()->setAlignment(Qt::AlignTop);
  layout()->addWidget(new QLabel("Sim FPS: ", this));
  m_simFps = new QSpinBox(this);
  m_simFps->setValue(24);
  m_simFps->setMaximum(9999);
  connect(m_simFps, &QSpinBox::valueChanged, [this](){emit simFpsChanged(m_simFps->value());});

  layout()->addWidget(m_simFps);
  layout()->addWidget(new QLabel("Damp Coefficient: "));
  m_dampCoeff = new QDoubleSpinBox(this);
  m_dampCoeff->setMinimum(.001);
  m_dampCoeff->setMaximum(30.);
  m_dampCoeff->setValue(.4);
  layout()->addWidget(m_dampCoeff);
  connect(m_dampCoeff, &QDoubleSpinBox::valueChanged, [this](){
    emit dampCoeffChanged(m_dampCoeff->value());
  });
  layout()->addWidget(new QLabel("Spring Coefficient: "));
  m_springCoeff = new QDoubleSpinBox(this);
  m_springCoeff->setMinimum(.001);
  m_springCoeff->setMaximum(100.);
  m_springCoeff->setValue(20.);
  layout()->addWidget(m_springCoeff);
  connect(m_springCoeff, &QDoubleSpinBox::valueChanged, [this](){
    emit springCoeffChanged(m_springCoeff->value());
  });
  m_reset = new QPushButton("Reset Sim", this);
  connect(m_reset, &QPushButton::pressed, [this](){emit resetPress();});
  layout()->addWidget(m_reset);
  layout()->addWidget(new QLabel("Controls:", this));
  layout()->addWidget(new QLabel("Add Particles:", this));
  layout()->addWidget(new QLabel("Amount:", this));

  m_particleCount = new QSpinBox(this);
  m_particleCount->setMinimum(1);
  m_particleCount->setMaximum(128*2000);

  layout()->addWidget(m_particleCount);

  layout()->addWidget(new QLabel("Position:", this));
  m_newPosition = new Vector3DSpinBox(this, -99999, 99999);
  layout()->addWidget(m_newPosition);
  layout()->addWidget(new QLabel("Radius:", this));
  m_radius = new QDoubleSpinBox(this);
  m_radius->setMinimum(.1);
  m_radius->setMaximum(9999);
  m_radius->setValue(50);
  layout()->addWidget(m_radius);
  layout()->addWidget(new QLabel("Direction:", this));
  m_newDirection = new Vector3DSpinBox(this, -1, 1, true);
  layout()->addWidget(m_newDirection);
  layout()->addWidget(new QLabel("Speed:", this));
  m_speed = new QDoubleSpinBox(this);
  m_speed->setMinimum(0);
  m_speed->setMaximum(9999);
  layout()->addWidget(m_speed);
  layout()->addWidget(new QLabel("Mass:", this));
  m_mass = new QDoubleSpinBox(this);
  m_mass->setMinimum(.01);
  m_mass->setMaximum(100);
  m_mass->setValue(1.5);
  layout()->addWidget(m_mass);
  layout()->addWidget(new QLabel("Colour: ", this));
  m_colourSelect = new QPushButton("Colour", this);
  m_colourSelect->setStyleSheet("background: " + m_colour.name() + ";");
  m_colourSelect->update();
  connect(m_colourSelect, &QPushButton::pressed, [this](){
    m_colour = QColorDialog::getColor(m_colour);
    m_colourSelect->setStyleSheet("background: " + m_colour.name() + ";");
    m_colourSelect->update();
  });
  layout()->addWidget(m_colourSelect);
  m_addParticles = new QPushButton("Add Particles", this);
  connect(m_addParticles, &QPushButton::pressed, this, &ControlBar::generateParticles);
  layout()->addWidget(new QLabel("Size: "));
  m_size = new QDoubleSpinBox(this);
  m_size->setMinimum(.01);
  m_size->setMaximum(100);
  m_size->setValue(1.);
  layout()->addWidget(m_size);
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
    particles.emplace_back(Particle{ngl::Vec4(p.m_x, p.m_y, p.m_z, 1.), m_newDirection->getValue()*m_speed->value(), (float)m_size->value(), ngl::Vec3(m_colour.redF(),m_colour.greenF(),m_colour.blueF()), (float)m_mass->value()});
  }
  emit addParticles(particles);
}
