#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#pragma once

#include <QWidget>
#include <QPushButton>
#include <vector>
#include <QSpinBox>
#include "particle.h"
#include "vector3dspinbox.h"
/**
 * @brief Provides controls to allow the user to influence the simulation
 * 
 */
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
    void dampCoeffChanged(float _newdampCoeff);
    void springCoeffChanged(float _newSpringCoeff);
  
  private:
    void generateParticles();
    QSpinBox *m_simFps;
    QDoubleSpinBox *m_dampCoeff;
    QDoubleSpinBox *m_springCoeff;
    QPushButton *m_reset;
    QSpinBox *m_particleCount;
    Vector3DSpinBox *m_newPosition;
    QDoubleSpinBox *m_radius;
    Vector3DSpinBox *m_newDirection;
    QDoubleSpinBox *m_speed;
    QDoubleSpinBox *m_mass;
    QColor m_colour = QColor(160,160,160);
    QPushButton *m_colourSelect;
    QDoubleSpinBox *m_size;
    QPushButton *m_addParticles;
};

#endif