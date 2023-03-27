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

  private:
    QSlider *m_slider;
    QSpinBox *m_frameEnd;
    QSpinBox *m_frameNum;



};

#endif