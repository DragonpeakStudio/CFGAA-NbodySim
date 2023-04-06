#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>
#include <random>
#include <QSpinBox>
#include "particle.h"
#include "vector3dspinbox.h"
class ControlBar : public QWidget
{
  Q_OBJECT
  public:
    ControlBar(QWidget *_parent=nullptr);
    ~ControlBar();
  
  signals:
    void resetPress();
    void addParticles(const std::vector<Particle> &_particles);
    void simFpsChanged(int _newFps);
  
  private:
    void generateParticles();
    QSpinBox *m_simFps;
    QPushButton *m_reset;
    QSpinBox *m_particleCount;
    Vector3DSpinBox *m_newPosition;
    QDoubleSpinBox *m_radius;
    Vector3DSpinBox *m_newDirection;
    QDoubleSpinBox *m_speed;
    QPushButton *m_addParticles;
};

#endif