#include "frameslider.h"

FrameSlider::FrameSlider(QWidget *_parent) : QWidget(_parent)
{
  setLayout(new QHBoxLayout(this));

  m_frameNum = new QSpinBox(this);
  m_frameNum->setValue(0);
  m_frameNum->setMaximum(240);
  layout()->addWidget(m_frameNum);

  m_slider = new QSlider(Qt::Orientation::Horizontal, this);
  m_slider->setMinimum(0);
  m_slider->setMaximum(240);
  m_slider->setTickPosition(QSlider::TicksBelow);
  m_slider->setTickInterval(1);
  layout()->addWidget(m_slider);

  m_frameEnd = new QSpinBox(this);
  m_frameEnd->setMaximum(9999999);
  m_frameEnd->setValue(240);
  layout()->addWidget(m_frameEnd);

  connect(m_frameEnd, &QSpinBox::valueChanged, m_slider, &QSlider::setMaximum);
  connect(m_frameEnd, &QSpinBox::valueChanged, m_frameNum, &QSpinBox::setMaximum);

  connect(m_frameNum, &QSpinBox::valueChanged, m_slider, &QSlider::setValue);
  connect(m_slider, &QSlider::valueChanged, m_frameNum, &QSpinBox::setValue);



}

FrameSlider::~FrameSlider()
{

}
