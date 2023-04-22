#ifndef VECTOR3DSPINBOX_H
#define VECTOR3DSPINBOX_H

#pragma once

#include <QWidget>
#include <QDoubleSpinBox>
#include <ngl/Vec3.h>
class Vector3DSpinBox : public QWidget
{
  Q_OBJECT
  public:
    Vector3DSpinBox(QWidget * _parent, double _min =-1, double _max = 1., bool _normalize = false);
    ngl::Vec3 getValue();

  private slots:
    void valueUpdate();

  signals:
    void valueChanged(ngl::Vec3 newValue);
  private:
    QDoubleSpinBox *m_x;
    QDoubleSpinBox *m_y;
    QDoubleSpinBox *m_z;
    bool m_normalize;

};

#endif