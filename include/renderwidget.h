#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#pragma once
#include <ngl/NGLInit.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <ngl/Obj.h>

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
    
  private:
    std::unique_ptr<ParticleRenderer> m_particleRenderer;
    std::unique_ptr<NBodyParticleSystem> m_particleSystem;
    virtual void mouseMoveEvent(QMouseEvent *_event) override;
    virtual void mousePressEvent(QMouseEvent *_event) override;
    virtual void wheelEvent(QWheelEvent *_event) override;
    const float m_panScale = .008f;
    const float m_orbitScale = .3f;
    const float m_zoomScale = .008f;


    Camera m_camera;
    QPointF m_lastMousePos;
};

#endif