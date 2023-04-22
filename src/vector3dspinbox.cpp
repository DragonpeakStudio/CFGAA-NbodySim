#include "vector3dspinbox.h"
#include <QHBoxLayout>

Vector3DSpinBox::Vector3DSpinBox(QWidget *_parent, double _min, double _max, bool _normalize) : QWidget(_parent), m_normalize(_normalize)
{
  setLayout(new QHBoxLayout(this));
  m_x = new QDoubleSpinBox(this);
  m_x->setMaximum(_max);
  m_x->setMinimum(_min);
  layout()->addWidget(m_x);

  m_y = new QDoubleSpinBox(this);
  m_y->setMaximum(_max);
  m_y->setMinimum(_min);
  layout()->addWidget(m_y);

  m_z = new QDoubleSpinBox(this);
  m_z->setMaximum(_max);
  m_z->setMinimum(_min);
  layout()->addWidget(m_z);
  connect(m_x, &QDoubleSpinBox::valueChanged, this, &Vector3DSpinBox::valueUpdate);
  connect(m_y, &QDoubleSpinBox::valueChanged, this, &Vector3DSpinBox::valueUpdate);
  connect(m_z, &QDoubleSpinBox::valueChanged, this, &Vector3DSpinBox::valueUpdate);

}

ngl::Vec3 Vector3DSpinBox::getValue()
{
  return ngl::Vec3(m_x->value(), m_y->value(), m_z->value());
}

void Vector3DSpinBox::valueUpdate()
{
  ngl::Vec3 val = getValue();
  if(m_normalize)
  {
    val.normalize();
    m_x->setValue(val.m_x);
    m_y->setValue(val.m_y);
    m_z->setValue(val.m_z);
  }
  emit valueChanged(val);
}