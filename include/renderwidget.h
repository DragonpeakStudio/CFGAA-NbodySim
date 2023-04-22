#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#pragma once
#include <ngl/NGLInit.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QLabel>
#include <QOpenGLWidget>
#include <ngl/Obj.h>
#include <random>
#include <chrono>
#include "nbodyparticlesystem.h"
#include "particlerenderer.h"
#include "camera.h"

class RenderWidget : public QOpenGLWidget
{
  Q_OBJECT
  public:
    RenderWidget();
    ~RenderWidget();
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

    NBodyParticleSystem *particleSystem() const;

  public slots:
    void setFrame(int _newFrame);
    void setFps(int _newFps);
    void resetSim();
    void setDampCoeff(float _dampCoeff);
    void setSpringCoeff(float _springCoeff);

    
  private:
    std::chrono::_V2::high_resolution_clock::time_point m_lastFrameTime;
    std::unique_ptr<ParticleRenderer> m_particleRenderer;
    std::unique_ptr<NBodyParticleSystem> m_particleSystem;
    virtual void mouseMoveEvent(QMouseEvent *_event) override;
    virtual void mousePressEvent(QMouseEvent *_event) override;
    virtual void wheelEvent(QWheelEvent *_event) override;
    const float m_panScale = .08f;
    const float m_orbitScale = .3f;
    const float m_zoomScale = .08f;
    QLabel *m_deltaAndFps;
    Camera m_camera;
    QPointF m_lastMousePos;
    int m_currentFrame = 0;
    int m_fps = 24;
    
};

#endif