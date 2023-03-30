#ifndef FRAMESLIDER_H
#define FRAMESLIDER_H

#pragma once

#include <QWidget>  
#include <QSlider>
#include <QHBoxLayout>
#include <QSpinBox>

class FrameSlider : public QWidget
{
  Q_OBJECT
  public:
    FrameSlider(QWidget *_parent=nullptr);
    ~FrameSlider();
  
  public slots:
    void setFrame(int _frame);
    void setEnd(int _end);
    void setFps(int _fps);

  signals:
    void frameChanged(int _newFrame);
    void endFrameChanged(int _newEndFrame);
    void fpsChanged(int _newFps);
  private:
    QSlider *m_slider;
    QSpinBox *m_frameEnd;
    QSpinBox *m_frameNum;
    QSpinBox *m_fps;



};

#endif